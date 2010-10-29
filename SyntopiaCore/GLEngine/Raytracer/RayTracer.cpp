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

		
		RayTracer::RayTracer(EngineWidget* engine, ProgressBox* progressBox) {
			this->progressBox = progressBox;
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
	
			progressiveRender = true;

			foreach (Command c, engine->getRaytracerCommands()) {
				QString arg = c.arg;
				arg = arg.remove("[");
				arg = arg.remove("]");
				setParameter(c.command, arg);
			}
			rt.backgroundColor = engine->getBackgroundColor();		
			this->engine = engine;
		}

		void RayTracer::startJobs(ProgressBox* progress) {
			completedUnits.setValue(0);
			nextUnit.setValue(0);

			INFO(QString("Starting %1 threads...").arg(maxThreads));
			for (int i = 0; i < maxThreads; i++) {
				threads[i]->start();
			}
			
			int c = completedUnits.value();
			int oldC = 0;
			
			int screenUpdates=0;
				
			QTime lastTime = QTime::currentTime().addMSecs(-2000);
			while (c<maxUnits) {
				bool s = completedUnits.wait(1000); // Wait to see if a new unit is completed.
								
				if	(progress && progress->wasCanceled()) {
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
				if (progress && !progress->wasCanceled()) progress->setValue((c*100)/maxUnits);
					
				if (lastTime.msecsTo(QTime::currentTime())>1000) {
					//Debug(QString::number(c));
				
					lastTime =  QTime::currentTime();

					if (oldC != c) {
						screenUpdates++;    
						oldC = c;
						engine->setImage(progressiveOutput->getImage());
					}
				
					qApp->processEvents();
				}

			};
			INFO(QString("Screen updates: %1").arg(screenUpdates));
		}
			

		QImage RayTracer::calculateImage(int w, int h) {
			
			if (progressiveRender) {
				maxUnits = rt.aaSamples*rt.aaSamples;
				rt.sampler = new ProgressiveStratifiedSampler(&rt.rg);
				((ProgressiveStratifiedSampler*)rt.sampler)->setAAOrder(rt.rg.getRandomIndices(maxUnits));
				
			} else {
				maxUnits = w;
				rt.sampler = new StratifiedSampler(&rt.rg);
			}

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
			if (progressBox) progressBox->start();

			QTime start = QTime::currentTime();
			QImage im(w,h, QImage::Format_RGB32);

			if (rt.lightPos == Vector3f(0,0,0)) {
				GLdouble sx1, sy1, sz1;				
				gluUnProject((float)-200, windowHeight, 0.0f, modelView, projection, viewPort, &sx1, &sy1 ,&sz1);				
				rt.lightPos = Vector3f((GLfloat)sx1, (GLfloat)sy1, (GLfloat)sz1);
			}

			rt.aaSamples=abs(rt.aaSamples);
			rt.setCounters(&nextUnit, &completedUnits, maxUnits);
			progressiveOutput =  new ProgressiveOutput(w,h);
			rt.progressiveOutput = progressiveOutput;
			if (progressiveRender) {
				rt.setTask(RenderThread::RaytraceProgressive);	
			} else {
				rt.setTask(RenderThread::Raytrace);
			}
			threads.append(&rt);
			for (int i = 1; i < maxThreads; i++) {
				threads.append(new RenderThread(rt));	
			}

			// To avoid coherence we will seed the threads differently.
			// (This was actually very visible near the left of the window).
			for (int i = 0; i < maxThreads; i++) {
				threads[i]->seed(rg.getInt());
			}
			
			startJobs(progressBox);
			double renderTime = start.msecsTo(QTime::currentTime())/1000.0;
			
			im = progressiveOutput->getImage();
			
			for (int i = 0; i < maxThreads; i++) {
				if (i!=0) delete(threads[i]);
			}
			
			INFO(QString("Time: %1s for %2 pixels.").arg(renderTime).arg(w*h));
			
			if (progressBox) progressBox->dismiss();
			delete (progressiveOutput);
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
			} else if (param == "progressive") {
				MiniParser(value, ',').getBool(progressiveRender);
				INFO(QString("Progressive Render: %3").arg(progressiveRender ? "true" : "false"));
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

