#include <QThread>


#include "RayTracer.h"

#include "SyntopiaCore/Math/Vector3.h"
#include "SyntopiaCore/Logging/Logging.h"
#include "SyntopiaCore/Misc/MiniParser.h"

using namespace SyntopiaCore::Math;
using namespace SyntopiaCore::Misc;

namespace SyntopiaCore {
	namespace GLEngine {

		using namespace SyntopiaCore::Logging;


		namespace {

			
		}

		
		RayTracer::RayTracer(EngineWidget* engine) {
			for (int i = 0; i < 16; i++) modelView[i] = engine->getModelViewCache()[i];
			for (int i = 0; i < 16; i++) projection[i] = engine->getProjectionCache()[i];
			for (int i = 0; i < 16; i++) viewPort[i] = engine->getViewPortCache()[i];

			sizeX = 0;
			sizeY = 0;
			userCancelled = false;
			Vector3f from;
			Vector3f to;
			engine->getBoundingBox(from,to);
			rt.accelerator = new VoxelStepper(from,to, 35);
			windowHeight = engine->height();
			windowWidth = engine->width();
			objects = engine->getObjects();
			for (int i = 0; i < objects.count(); i++) rt.accelerator->registerObject(objects[i]);

			maxThreads = QThread::idealThreadCount();
	
			foreach (Command c, engine->getRaytracerCommands()) {
				QString arg = c.arg;
				arg = arg.remove("[");
				arg = arg.remove("]");
				setParameter(c.command, arg);
			}
			rt.backgroundColor = engine->getBackgroundColor();

		
		}


		namespace {
			int sqr(int a) { return a*a; } 

			float cdist(Vector3f c1, Vector3f c2) {
				return (c1-c2).sqrLength();
			}

		}



		void RayTracer::startJobs(QProgressDialog& progress) {
			completedUnits.setValue(0);
			nextUnit.setValue(0);

			INFO(QString("Starting %1 threads...").arg(maxThreads));
			for (int i = 0; i < maxThreads; i++) {
				threads[i]->start();
			}
			
			
			int c = completedUnits.value();
			while (c<maxUnits) {
				completedUnits.wait();
				progress.setValue((c*100)/maxUnits);
				//qApp->processEvents();

				if	(progress.wasCanceled()) {
					// in order to cancel, we will swallow
					// all pending job units...
					userCancelled = true;
					int newUnit = nextUnit.increase();
					while (newUnit <= maxUnits) {
						completedUnits.increase();
						newUnit = nextUnit.increase();
					}
				}
				 c = completedUnits.value();
			};
		}
			

		QImage RayTracer::calculateImage(int w, int h) {
			
			// Setup dimensions
			if (w == 0) w = sizeX;
			if (h == 0) h = sizeY;

			if (w==0 && h==0) {
				w = windowWidth;
				h = windowHeight;
			} else if (w==0) {
				w = h * (windowWidth / (float)windowHeight);
			} else if (h==0) {
				h = w * (windowHeight / (float)windowWidth);
			}

			INFO(QString("Rendering size: %3x%4").arg(w).arg(h));

			rt.alloc(w,h);
			maxUnits = w;
			
			// Calculate viewport
			GLdouble ox1, oy1, oz1;				
			gluUnProject(0, windowHeight, 0.0f, modelView, projection, viewPort, &ox1, &oy1 ,&oz1);
			rt.frontStart = Vector3f(ox1,oy1,oz1);
			gluUnProject(windowWidth, windowHeight, 0.0f, modelView, projection, viewPort, &ox1, &oy1 ,&oz1);
			rt.frontX = Vector3f(ox1,oy1,oz1)-rt.frontStart;
			gluUnProject(0, 0, 0.0f, modelView, projection, viewPort, &ox1, &oy1 ,&oz1);
			rt.frontY = Vector3f(ox1,oy1,oz1)-rt.frontStart;

			gluUnProject(0, windowHeight, 1.0f, modelView, projection, viewPort, &ox1, &oy1 ,&oz1);
			rt.backStart = Vector3f(ox1,oy1,oz1);
			gluUnProject(windowWidth, windowHeight, 1.0f, modelView, projection, viewPort, &ox1, &oy1 ,&oz1);
			rt.backX = Vector3f(ox1,oy1,oz1)-rt.backStart;
			gluUnProject(0, 0, 1.0f, modelView, projection, viewPort, &ox1, &oy1 ,&oz1);
			rt.backY = Vector3f(ox1,oy1,oz1)-rt.backStart;

			// Setup progress dialog.
			QProgressDialog progress("progress", "Cancel", 0, 100);
			progress.setLabelText("Ray tracing");
			progress.setMinimumDuration(1000);
			progress.setWindowModality(Qt::WindowModal);

			QTime start = QTime::currentTime();
			QImage im(w,h, QImage::Format_RGB32);

			// RD

			// Find a suitable light position. TODO: CHANGE!
			GLdouble sx1, sy1, sz1;				
			gluUnProject((float)-200, windowHeight, 0.0f, modelView, projection, viewPort, &sx1, &sy1 ,&sz1);				
			rt.lightPos = Vector3f((GLfloat)sx1, (GLfloat)sy1, (GLfloat)sz1);

			aaPixels = 0;
			
			bool debugAA = rt.aaSamples<0;
			rt.aaSamples=abs(rt.aaSamples);
			
			rt.setCounters(&nextUnit, &completedUnits, maxUnits);
			rt.setTask(RenderThread::RayTrace);
			threads.append(&rt);
			for (int i = 1; i < maxThreads; i++) {
				threads.append( new RenderThread(rt));	
			}

			startJobs(progress);
			
			double renderTime = start.msecsTo(QTime::currentTime())/1000.0;
			start = QTime::currentTime();


			progress.setLabelText("Ambient Occlusion (Step 2/3)...");

			int step = rt.occlusionSampleStepSize;
			if ( rt.ambMaxRays>0 && step>0) {
				double tr = 0.99;

				for (int x = 0; x < w; x=x+1) {
					for (int y = 0; y < h; y=y+1) {	
						bool sample = false;
						if ((y % step == 0) && (x % step == 0)) { sample = true; } 	
						else if ((x > 0) && 
							(rt.objs[x+y*w]!=rt.objs[x+y*w-1] || Vector3f::dot(rt.normals[x+y*w],rt.normals[x+y*w-1])<tr)) { sample = true; }
						else if ((y > 0) && 
							(rt.objs[x+y*w]!=rt.objs[x+y*w-w] || Vector3f::dot(rt.normals[x+y*w],rt.normals[x+y*w-w])<tr)) { sample = true; }
						else if ((y < h-1) && 
							(rt.objs[x+y*w]!=rt.objs[x+y*w+w] || Vector3f::dot(rt.normals[x+y*w],rt.normals[x+y*w+w])<tr)) { sample = true; }
						else if ((x < w-1) && 
							(rt.objs[x+y*w]!=rt.objs[x+y*w+1] || Vector3f::dot(rt.normals[x+y*w],rt.normals[x+y*w+1])<tr)) { sample = true; }

						if (!sample || rt.depths[x+y*w] > 1) { 
							rt.aoMap[x+y*w] = 1;
							continue;
						}
					}
				}
	
					
                // Call threads....
				for (int i = 0; i < maxThreads; i++) threads[i]->setTask(RenderThread::AmbientOcclusion);
				startJobs(progress);

				for (int i = 0; i<w*h; i++) if (rt.depths[i]>1) rt.aoMap[i] = 1;

				double* aoMap2 = new double[w*h];
				for (int i = 0; i<w*h; i++) aoMap2[i] = rt.aoMap[i];

				// Fill with simple
				for (int y = 0; y < h; y=y+step) {
					for (int x = 0; x < w; x=x+step) {
						double d = rt.aoMap[x+y*w];;
						for (int yy = 0; yy < step; yy++) {
							for (int xx = 0; xx < step; xx++) {
								if ((x+xx>=w) || (y+yy>=h)) continue;
								if ((xx == 0 && yy == 0) || rt.aoMap[xx+x+(y+yy)*w]>=0) 
									d = rt.aoMap[xx+x+(y+yy)*w];
								aoMap2[xx+x+(y+yy)*w] =  d;
							}
						}	
					}
				}


				tr = 0.9;
				const int f = 10;
				for (int i = 0; i< rt.ambSmooth; i++) {
					for (int y = 1 ; y < h-1; y++) {
						for (int x = 1 ; x < w-1; x++) {
							int c = rt.aoMap[x+y*w+1]<0 ? 1 : f;
							double d = c*aoMap2[x+y*w];
							if (rt.objs[x+y*w]==rt.objs[x+y*w+1] && Vector3f::dot(rt.normals[x+y*w],rt.normals[x+y*w+1])>tr) { int n = rt.aoMap[x+y*w+1]<0 ? 1 : f ; c=c+n; d+=n*aoMap2[x+y*w+1]; };
							if (rt.objs[x+y*w]==rt.objs[x+y*w+w] && Vector3f::dot(rt.normals[x+y*w],rt.normals[x+y*w+w])>tr) { int n = rt.aoMap[x+y*w+w]<0 ? 1 : f ; c=c+n; d+=n*aoMap2[x+y*w+w]; };
							if (rt.objs[x+y*w]==rt.objs[x+y*w-w] && Vector3f::dot(rt.normals[x+y*w],rt.normals[x+y*w-w])>tr) { int n = rt.aoMap[x+y*w-w]<0 ? 1 : f ; c=c+n; d+=n*aoMap2[x+y*w-w]; };
							if (rt.objs[x+y*w]==rt.objs[x+y*w-1] && Vector3f::dot(rt.normals[x+y*w],rt.normals[x+y*w-1])>tr) { int n = rt.aoMap[x+y*w-1]<0 ? 1 : f ; c=c+n; d+=n*aoMap2[x+y*w-1]; };
							d = d/c;
							aoMap2[x+y*w] = d;	
						}
					}


				}


				for (int i = 0; i<w*h; i++) rt.aoMap[i] = aoMap2[i];
				delete[] aoMap2;



			} else {
				for (int i = 0; i<w*h; i++) rt.aoMap[i] = 1;

			}

			if	(progress.wasCanceled()) {
				for (int i = 0; i<w*h; i++) if (rt.aoMap[i] == -1) rt.aoMap[i] = 1.0;
			}
		
			double aoTime = start.msecsTo(QTime::currentTime())/1000.0;
			start = QTime::currentTime();

			progress.setLabelText("Raytracing Anti-Alias (Step 3/3)...");

			int aaPixels = 0;
			if (rt.aaSamples>0) {
				const float threshold = 0.1f*0.1f;

				// Determine which parts to anti-alias
				// As of now, only color information is used,
				// but we also have normals, depths, objects and so on,
				// if we need more fine-grained control.
				for (int x = 1; x+1 < w; x++) {
					for (int y = 1; y+1 < h; y++) {
						Vector3f c1 = rt.colors[x+y*w]*rt.aoMap[x+y*w];
						
						if (rt.dofFalloff !=0 || 
							cdist(c1,rt.colors[x+1+y*w]*rt.aoMap[x+1+y*w]) > threshold ||
							cdist(c1,rt.colors[x-1+y*w]*rt.aoMap[x-1+y*w]) > threshold ||
							cdist(c1,rt.colors[x+(y+1)*w]*rt.aoMap[x+(y+1)*w]) > threshold ||
							cdist(c1,rt.colors[x+(y-1)*w]*rt.aoMap[x+(y-1)*w]) > threshold)
						{
							rt.depths[x+y*w] = 0;
							aaPixels++;
						} else {
							rt.depths[x+y*w] = 1;
						}
					}
				}

				/// Multithread the actual anti-aliasing
				for (int i = 0; i < maxThreads; i++) threads[i]->setTask(RenderThread::AntiAlias);
				if (!debugAA) startJobs(progress);
			}


			for (int x = 0; x < w; x++) {
				for (int y = 0; y < h; y++) {
					rt.colors[x+y*w]=rt.colors[x+y*w]*rt.aoMap[x+y*w];
					Vector3f c = rt.colors[x+y*w];
					im.setPixel(x,y,qRgb(c.x()*255, c.y()*255, c.z()*255));
					if (debugAA && rt.depths[x+y*w]==0) {
						im.setPixel(x,y,qRgb(255,0,0));
					}
				}
			}

			for (int i = 0; i < maxThreads; i++) {
				//INFO(threads[i]->isFinished() ? "isFinished" : "isNot");
				if (i!=0) delete(threads[i]);
			}
			double aaTime = start.msecsTo(QTime::currentTime())/1000.0;

			INFO(QString("Time: %1s (rendering %2s, ambient occlusion %3s, anti-alias %4s)")
				.arg(renderTime+aoTime+aaTime).arg(renderTime).arg(aoTime).arg(aaTime));
			//INFO(QString("Pixels: %1, Object checks: %2, Objects checked per pixel: %3").
			//	arg(pixels).arg(checks).arg(checks/(float)pixels));
			INFO(QString("Pixels: %1, Anti-aliased pixels: %2 (percentage: %3%)").
				arg(w*h).arg(aaPixels).arg((100.0*aaPixels)/(float)(w*h),0,'f',2));


			progress.close();


			return im;
		}



		void RayTracer::setParameter(QString param, QString value) {
			param=param.toLower();

			if (param == "ambient-occlusion") {
				// Min rays, Max rays, Precision...		
				MiniParser(value, ',').getInt(rt.occlusionSampleStepSize).getInt(rt.ambMaxRays).getInt(rt.ambSmooth);
				INFO(QString("Occlusion Sample Step Size: %1, Max Rays: %2, Smoothening Steps: %3 ")
					.arg(rt.occlusionSampleStepSize).arg(rt.ambMaxRays).arg(rt.ambSmooth));
			} else if (param == "anti-alias") {
				// Min rays, Max rays, Precision...		
				MiniParser(value, ',').getInt(rt.aaSamples);
			} else if (param == "dof") {
				MiniParser(value, ',').getDouble(rt.dofCenter).getDouble(rt.dofFalloff);
			} else if (param == "shadows") {
				MiniParser(value, ',').getBool(rt.useShadows);
				INFO(QString("Shadows: %3").arg(rt.useShadows ? "true" : "false"));
			} else if (param == "max-threads") {
				MiniParser(value, ',').getInt(maxThreads);
				if (maxThreads<=0) maxThreads = QThread::idealThreadCount();
				INFO(QString("Max threads: %3").arg(maxThreads));
			} else if (param == "size") {
				MiniParser(value, 'x').getInt(sizeX).getInt(sizeY);
				INFO(QString("size: %3, %4").arg(sizeX).arg(sizeY));
			} else {
				WARNING("Unknown parameter: " + param);
			}
		}





	}
}

