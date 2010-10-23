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
			rt.setObjects(objects.count());
			for (int i = 0; i < objects.count(); i++) objects[i]->setObjectID(i);
			
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
				bool s = completedUnits.wait(1000);
				progress.setValue((c*100)/maxUnits);
				if (!s) qApp->processEvents();

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


			// Initialize rng loopup tables 
			RandomNumberGenerator rg;
			rg.getUniform2DFromTable();
			rg.getUniform3DFromTable();
			
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

			if (rt.lightPos == Vector3f(0,0,0)) {
				GLdouble sx1, sy1, sz1;				
				gluUnProject((float)-200, windowHeight, 0.0f, modelView, projection, viewPort, &sx1, &sy1 ,&sz1);				
				rt.lightPos = Vector3f((GLfloat)sx1, (GLfloat)sy1, (GLfloat)sz1);
			}

			
			rt.aaSamples=abs(rt.aaSamples);
			rt.setCounters(&nextUnit, &completedUnits, maxUnits);
			rt.setTask(RenderThread::Raytrace);
			threads.append(&rt);
			for (int i = 1; i < maxThreads; i++) {
				threads.append( new RenderThread(rt));	
			}

			// To avoid coherence we will seed the threads differently.
			// (This was actually very visible near the left of the window).
			for (int i = 0; i < maxThreads; i++) {
				threads[i]->seed(rg.getInt());
			}
			
			for (int i = 0; i < maxThreads; i++) threads[i]->setTask(RenderThread::Raytrace);
			startJobs(progress);
			double renderTime = start.msecsTo(QTime::currentTime())/1000.0;
			
			for (int x = 0; x < w; x++) {
				for (int y = 0; y < h; y++) {
					Vector3f c = rt.colors[x+y*w];
					im.setPixel(x,y,qRgb(c.x()*255, c.y()*255, c.z()*255));
				}
			}

			for (int i = 0; i < maxThreads; i++) {
				if (i!=0) delete(threads[i]);
			}
			
			INFO(QString("Time: %1s")
				.arg(renderTime));
			INFO(QString("Pixels: %1.").
				arg(w*h));

			progress.close();
			return im;
		}



		void RayTracer::setParameter(QString param, QString value) {
			param=param.toLower();

			if (param == "ambient-occlusion-samples") {	
				MiniParser(value, ',').getInt(rt.aoSamples);
				INFO(QString("Ambient Occlusion samples: %3x%4 ")
					.arg(rt.aoSamples).arg(rt.aoSamples));
			} else if (param == "samples") {
				MiniParser(value, ',').getInt(rt.aaSamples);
				INFO(QString("Samples per pixel (anti-alias or DOF): %1x%2 ")
					.arg(rt.aaSamples).arg(rt.aaSamples));
			} else if (param == "dof") {
				MiniParser(value, ',').getDouble(rt.dofCenter).getDouble(rt.dofFalloff);
				INFO(QString("Depth-of-field: center %1, falloff %2 ")
					.arg(rt.dofCenter).arg(rt.dofFalloff));
			} else if (param == "shadows") {
				MiniParser(value, ',').getBool(rt.useShadows);
				INFO(QString("Shadows: %3").arg(rt.useShadows ? "true" : "false"));
			} else if (param == "max-threads") {
				MiniParser(value, ',').getInt(maxThreads);
				if (maxThreads<=0) maxThreads = QThread::idealThreadCount();
				INFO(QString("Max threads: %3").arg(maxThreads));
			} else if (param == "size") {
				MiniParser(value, 'x').getInt(sizeX).getInt(sizeY);
				INFO(QString("Size: %1, %2").arg(sizeX).arg(sizeY));
			} else if (param == "light") {
				MiniParser(value, ',').getFloat(rt.lightPos.x()).getFloat(rt.lightPos.y()).getFloat(rt.lightPos.z());
				INFO(QString("Light position: %1, %2, %3").arg(rt.lightPos.x())
					  .arg(rt.lightPos.y()).arg(rt.lightPos.z()));
			} else {
				WARNING("Unknown parameter: " + param);
			}
		}





	}
}

