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

			int cdist(QRgb c1, QRgb c2) {
				return sqr(qGreen(c1)-qGreen(c2))+sqr(qRed(c1)-qRed(c2))+sqr(qBlue(c1)-qBlue(c2));
			}

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

			QVector<RenderThread*> threads;
			AtomicCounter nextUnit;
			AtomicCounter completedUnits;
			int maxUnits = w;
			
			
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
			/*
			for (int x = 0; x < w; x++) {
				float fx = x/(float)(w);
				if (x % 7 == 0) {
					progress.setValue((x*100)/w);
					//qApp->processEvents();
					if	(progress.wasCanceled()) {
						userCancelled = true;
						break;
					}
				}
				//
			}
			*/
			//rt.render();
			rt.setCounters(&nextUnit, &completedUnits, maxUnits);
			
			threads.append(&rt);
			for (int i = 1; i < maxThreads; i++) {
				threads.append( new RenderThread(rt));
			}

			INFO(QString("Starting %1 threads...").arg(maxThreads));
			for (int i = 0; i < maxThreads; i++) {
				threads[i]->start();
			}
			
			
			while (completedUnits.value()<maxUnits) {
				completedUnits.wait();
				//RenderThread::msleep(50); // couldn't we wait for some mutex instead?
				//INFO(QString("%1").arg(completedUnits.value()));
				//qApp->processEvents();
			};
			
			double renderTime = start.msecsTo(QTime::currentTime())/1000.0;
			start = QTime::currentTime();
/*
			progress.setLabelText("Ambient Occlusion (Step 2/3)...");

			int step = occlusionSampleStepSize;
			if ( ambMaxRays>0 && step>0) {
				double tr = 0.99;

				QVector<Vector3f> dirs;
				for (int i = 0; i < ambMaxRays; i++) {
					Vector3f random ;
					do {
						random = Vector3f(rg.getDouble(-1,1),rg.getDouble(-1,1),rg.getDouble(-1,1));
					} while (random.sqrLength() > 1);
					random.normalize();
					dirs.append(random);
				}


				for (int x = 0; x < w; x=x+1) {

					//float fx = x/(float)w;
					if (x % 7 == 0) {
						progress.setValue((x*100)/w);
						//qApp->processEvents();
						if	(progress.wasCanceled()) {
							userCancelled = true;
							break;
						}
					}
					for (int y = 0; y < h; y=y+1) {	

						bool sample = false;
						if ((y % step == 0) && (x % step == 0)) { sample = true; } 	
						else if ((x > 0) && 
							(objs[x+y*w]!=objs[x+y*w-1] || Vector3f::dot(normals[x+y*w],normals[x+y*w-1])<tr)) { sample = true; }
						else if ((y > 0) && 
							(objs[x+y*w]!=objs[x+y*w-w] || Vector3f::dot(normals[x+y*w],normals[x+y*w-w])<tr)) { sample = true; }
						else if ((y < h-1) && 
							(objs[x+y*w]!=objs[x+y*w+w] || Vector3f::dot(normals[x+y*w],normals[x+y*w+w])<tr)) { sample = true; }
						else if ((x < w-1) && 
							(objs[x+y*w]!=objs[x+y*w+1] || Vector3f::dot(normals[x+y*w],normals[x+y*w+1])<tr)) { sample = true; }

						if (!sample) continue;


						
						//Vector3f c = colors[x+y*w];
						//c.y() =1;
						//colors[x+y*w] = Vector3f(0,1,0);

						//float fy = y/(float)h;
						if (depths[x+y*w] > 1) {
							aoMap[x+y*w] = 1;
							continue;
						}
						int hits = 0;
						int tests = 0;
						totalAOCasts++;
						for (int ix = 0; ix < ambMaxRays; ix++) {
							tests++;
							// Use monte carlo sampling to obtain random vector.

							Vector3f random ;
							do {
								random = Vector3f(rg.getDouble(-1,1),rg.getDouble(-1,1),rg.getDouble(-1,1));
							} while (random.sqrLength() > 1);
							random.normalize();
							if (Vector3f::dot(random, normals[x+y*w])<0) random = random*-1.0; // Only check away from surface.

							
							//Vector3f random = dirs[ix];
							//if (Vector3f::dot(random, normals[x+y*w])<0) random = random*-1.0; // Only check away from surface.
							


							double maxT = 0;
							QList<Object3D*>* list = accelerator->setupRay(intersections[x+y*w],random, maxT);
							RayInfo ri;
							ri.startPoint = intersections[x+y*w];
							ri.lineDirection = random;
							bool occluded = false;
							while (list != 0 && !occluded) { 
								// check objects
								for (int i = 0; i < list->size(); i++) {
									if (list->at(i) == objs[x+y*w]) continue; // self-shadow? 							
									occluded = list->at(i)->intersectsRay(&ri);
									if (ri.intersection < 1E-5) occluded = false;
									if (occluded) break;								
								}

								if (!occluded) list = accelerator->advance(maxT); 

							}
							if (occluded) hits++;
						}
						double occ = 1-hits/(double)tests;
						aoMap[x+y*w] = occ;
					}
				}


				for (int i = 0; i<w*h; i++) if (depths[i]>1) aoMap[i] = 1;

				double* aoMap2 = new double[w*h];
				for (int i = 0; i<w*h; i++) aoMap2[i] = aoMap[i];

				// Fill with simple
				for (int y = 0; y < h; y=y+step) {
					for (int x = 0; x < w; x=x+step) {
						double d = aoMap[x+y*w];;
						for (int yy = 0; yy < step; yy++) {
							for (int xx = 0; xx < step; xx++) {
								if ((x+xx>=w) || (y+yy>=h)) continue;
								if ((xx == 0 && yy == 0) || aoMap[xx+x+(y+yy)*w]>=0) 
									d = aoMap[xx+x+(y+yy)*w];
								aoMap2[xx+x+(y+yy)*w] =  d;

							}
						}	
					}
				}


				//iterations = 10;
				tr = 0.9;
				const int f = 10;
				for (int i = 0; i< ambSmooth; i++) {
					for (int y = 1 ; y < h-1; y++) {
						for (int x = 1 ; x < w-1; x++) {
							int c = aoMap[x+y*w+1]<0 ? 1 : f;
							double d = c*aoMap2[x+y*w];
							if (objs[x+y*w]==objs[x+y*w+1] && Vector3f::dot(normals[x+y*w],normals[x+y*w+1])>tr) { int n = aoMap[x+y*w+1]<0 ? 1 : f ; c=c+n; d+=n*aoMap2[x+y*w+1]; };
							if (objs[x+y*w]==objs[x+y*w+w] && Vector3f::dot(normals[x+y*w],normals[x+y*w+w])>tr) { int n = aoMap[x+y*w+w]<0 ? 1 : f ; c=c+n; d+=n*aoMap2[x+y*w+w]; };
							if (objs[x+y*w]==objs[x+y*w-w] && Vector3f::dot(normals[x+y*w],normals[x+y*w-w])>tr) { int n = aoMap[x+y*w-w]<0 ? 1 : f ; c=c+n; d+=n*aoMap2[x+y*w-w]; };
							if (objs[x+y*w]==objs[x+y*w-1] && Vector3f::dot(normals[x+y*w],normals[x+y*w-1])>tr) { int n = aoMap[x+y*w-1]<0 ? 1 : f ; c=c+n; d+=n*aoMap2[x+y*w-1]; };
							d = d/c;
							aoMap2[x+y*w] = d;	
						}
					}


				}


				for (int i = 0; i<w*h; i++) aoMap[i] = aoMap2[i];
				delete[] aoMap2;



			} else {
				for (int i = 0; i<w*h; i++) aoMap[i] = 1;

			}

			if	(progress.wasCanceled()) {
				for (int i = 0; i<w*h; i++) aoMap[i] = 1.0;
			}

*/
			for (int x = 0; x < w; x++) {
				for (int y = 0; y < h; y++) {
					Vector3f c = rt.colors[x+y*w]; // REMEMBER THIS: *rt.aoMap[x+y*w];
					//c = Vector3f(1,1,1)*aoMap[x+y*w];
					im.setPixel(x,y,qRgb(c.x()*255, c.y()*255, c.z()*255));
				}
			}


			double aoTime = start.msecsTo(QTime::currentTime())/1000.0;
			start = QTime::currentTime();

/*
			progress.setLabelText("Raytracing Anti-Alias (Step 3/3)...");

			long aaPixels = 0;


			if (aaSamples>0) {
				float xs = (1.0/w);
				float ys = (1.0/h);
				for (int x = 1; x+1 < w; x++) {
					float fx = x/(float)w;

					if (x % 7 == 0) {
						progress.setValue((x*100)/w);
						//qApp->processEvents();
						if (progress.wasCanceled()) {
							userCancelled = true;
							break;
						}
					}

					for (int y = 1; y+1 < h; y++) {
						float fy = y/(float)h;

						QRgb c1 = im.pixel(x,y);
						int tres = 3*50*50;

						if ( cdist(c1,im.pixel(x+1,y)) > tres ||
							cdist(c1,im.pixel(x,y+1)) > tres ||
							cdist(c1,im.pixel(x-1,y)) > tres ||
							cdist(c1,im.pixel(x,y-1)) > tres)

						{
							aaPixels++;
							Vector3f color(0,0,0);

							unsigned int steps = aaSamples;
							double xstepsize = xs/steps;
							double ystepsize = ys/steps;
							for (unsigned int xo = 0; xo < steps; xo++) {
								for (unsigned int yo = 0; yo < steps; yo++) {	
									color = color + rayCastPixel(fx-xs/2.0 +xo*xstepsize+xstepsize/2.0,
										(fy-ys/2.0 +yo*ystepsize+ystepsize/2.0));
								}
							}
							color = color / (steps*steps);
							Vector3f c = color*aoMap[x+y*w];
							im.setPixel(x,y,qRgb(c.x()*255, c.y()*255, c.z()*255));
						}

					}
				}
			}

			

*/
			for (int i = 0; i < maxThreads; i++) {
				INFO(threads[i]->isFinished() ? "isFinished" : "isNot");
				if (i!=0) delete(threads[i]);
			}
			double aaTime = start.msecsTo(QTime::currentTime())/1000.0;

			INFO(QString("Time: %1s (rendering %2s, ambient occlusion %3s, anti-alias %4s)")
				.arg(renderTime+aoTime+aaTime).arg(renderTime).arg(aoTime).arg(aaTime));
			//INFO(QString("Pixels: %1, Object checks: %2, Objects checked per pixel: %3").
			//	arg(pixels).arg(checks).arg(checks/(float)pixels));
			//INFO(QString("Pixels: %1, Anti-aliased pixels: %2 (percentage: %3%)").
			//	arg(pixels).arg(aaPixels).arg((100.0*aaPixels)/(float)pixels,0,'f',2));


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

