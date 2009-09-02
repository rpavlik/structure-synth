#include "RayTracer.h"

#include "SyntopiaCore/Math/Vector3.h"
#include "SyntopiaCore/Logging/Logging.h"

using namespace SyntopiaCore::Math;

namespace SyntopiaCore {
	namespace GLEngine {


		using namespace SyntopiaCore::Logging;


	  /// See here for details about this approach:
		/// http://www.devmaster.net/articles/raytracing_series/part4.php
		/// THIS IS A DUMMY IMPLEMENTATION!
		/*
		class VoxelStepper {
		public:
			VoxelStepper(Vector3f minPos, Vector3f maxPos, unsigned int steps) {};
			void registerObject(Object3D* obj) { list.append(obj); }
			QList<Object3D*>* setupRay(Vector3f pos, Vector3f dir, double& maxT) { return &list; }
			QList<Object3D*>* advance(double& maxT) { return 0; }

		private:
			QList<Object3D*> list;
		};
		*/

		
		/// See here for details about this approach:
		/// http://www.devmaster.net/articles/raytracing_series/part4.php
		class VoxelStepper {
		public:
			VoxelStepper::VoxelStepper(Vector3f minPos, Vector3f maxPos, unsigned int steps) :  steps(steps), minPos(minPos), maxPos(maxPos) {
				Vector3f d = maxPos - minPos;										
				size = d/(double)steps;
				currentT = 0;
				grid = new QList<Object3D*>[steps*steps*steps];
				for (unsigned int i = 0; i < steps*steps*steps; i++) grid[i] = QList<Object3D*>();
			};

			VoxelStepper::~VoxelStepper() { delete[] grid; };

			void VoxelStepper::registerObject(Object3D* obj) {
				// Simple method - check all cells intersecting the objects bounding boxes.

				Vector3f from;
				Vector3f to;
				obj->getBoundingBox(from,to);

				from = from - minPos;
				to = to - minPos;

				int xStart = floor(from.x()/size.x());
				int xEnd = ceil(to.x()/size.x());
				int yStart = floor(from.y()/size.y());
				int yEnd = ceil(to.y()/size.y());
				int zStart = floor(from.z()/size.z());
				int zEnd = ceil(to.z()/size.z());
				if (xStart < 0) xStart = 0;
				if (yStart < 0) yStart = 0;
				if (zStart < 0) zStart = 0;
				if (xEnd > (int)steps) xEnd = steps;
				if (yEnd > (int)steps) yEnd = steps;
				if (zEnd > (int)steps) zEnd = steps;

				for (unsigned int x = xStart; x < (unsigned int)xEnd; x++) {
					for (unsigned int y = yStart; y < (unsigned int)yEnd; y++) {
						for (unsigned int z = zStart; z < (unsigned int)zEnd; z++) {
							if (obj->intersectsAABB(minPos + Vector3f(size.x()*x,size.y()*y,size.z()*z),minPos + Vector3f(size.x()*(x+1),size.y()*(y+1),size.z()*(z+1)) )) {
								grid[x+y*steps+z*steps*steps].append(obj);
							}
						}
					}
				}

			}

			QList<Object3D*>* VoxelStepper::setupRay(Vector3f pos, Vector3f dir, double& maxT) {
				this->pos = pos; 
				this->dir = dir;

				currentT = 0;

				Vector3f ro = pos - minPos;
				cx = floor(ro.x() / size.x());
				cy =  floor(ro.y() / size.y());
				cz =  floor(ro.z() / size.z());


				if ((cx < 0 || cx >= (int)steps) ||
					(cy < 0 || cy >= (int)steps) || 
					(cz < 0 || cz >= (int)steps)) {						
						// we are outside grid.
						// advance ray to inside grid.

						bool found = false;
						double p;
						if (dir.x() > 0) {
							p = (minPos.x()-pos.x())/dir.x();
							cx = 0;
						} else {
							p = (maxPos.x()-pos.x())/dir.x();
							cx = steps-1;								
						}
						Vector3f v = pos + dir*p - minPos;								
						cy = floor(v.y() / size.y());
						cz = floor(v.z() / size.z());
						if ((cy >= 0 && cy < (int)steps) && (cz >= 0 && cz < (int)steps)) {
							found = true;
							pos = v+minPos;
						}

						if (!found) {
							if (dir.y() > 0) {
								p = (minPos.y()-pos.y())/dir.y();
								cy = 0;
							} else {
								p = (maxPos.y()-pos.y())/dir.y();
								cy = steps-1;
							}
							Vector3f v = pos + dir*p - minPos;									
							cx = floor(v.x() / size.x());
							cz = floor(v.z() / size.z());
							if ((cx >= 0 && cx < (int)steps) && (cz >= 0 && cz < (int)steps)) {
								pos = v+minPos;
								found = true;
							}
						}

						if (!found) {
							if (dir.z() > 0) {
								p = (minPos.z()-pos.z())/dir.z();
								cz = 0;
							} else {
								p = (maxPos.z()-pos.z())/dir.z();
								cz = steps-1;
							}
							Vector3f v = pos + dir*p - minPos;									
							cx = floor(v.x() / size.x());
							cy = floor(v.y() / size.y());
							if ((cy >= 0 && cy < (int)steps) && (cx >= 0 && cx < (int)steps)) {
								pos = v+minPos;
								found = true;
							}
						}

						currentT = p;

						// We do not intersect grid.
						if (!found) return false;
					}

					stepX = (dir.x() > 0) ? 1 : -1;
					stepY = (dir.y() > 0) ? 1 : -1;
					stepZ = (dir.z() > 0) ? 1 : -1;

					// TODO: Get rid of the fabs...
					tDeltaX = size.x() / fabs(dir.x());
					tDeltaY = size.y() / fabs(dir.y());
					tDeltaZ = size.z() / fabs(dir.z());

					Vector3f orv = pos- (minPos + Vector3f(size.x()*cx, size.y()*cy, size.z()*cz));
					tMaxX = fabs(orv.x()/dir.x());
					if (stepX>0) tMaxX = tDeltaX - tMaxX;
					tMaxY = fabs(orv.y()/dir.y());
					if (stepY>0) tMaxY = tDeltaY - tMaxY;
					tMaxZ = fabs(orv.z()/dir.z());
					if (stepZ>0) tMaxZ = tDeltaZ - tMaxZ;

					// Now pos is advanced properly.
					// cx,cy,cz contains current cell.
					QList<Object3D*>* list = &grid[cx+cy*steps+cz*steps*steps];

					if (list && (list->count() == 0)) {
						list = advance(maxT);
					} else {
						maxT = currentT + minn(tMaxX, tMaxY, tMaxZ);					
					}

					return list;
			}

			double minn(double a, double b, double c) {
				if (a<b) return (a<c ? a : c);
				return (b<c ? b : c);
			}

			QList<Object3D*>* VoxelStepper::advance(double& maxT) {
				QList<Object3D*>* list = 0;
				do {
					if(tMaxX < tMaxY) {
						if(tMaxX < tMaxZ) {
							cx += stepX;
							if (cx >= (int)steps || cx < 0) return 0;
							tMaxX = tMaxX + tDeltaX;
						} else {
							cz += stepZ;
							if (cz >= (int)steps || cz < 0) return 0;
							tMaxZ = tMaxZ + tDeltaZ;
						} 
					} else {
						if(tMaxY < tMaxZ) {
							cy += stepY;
							if (cy >= (int)steps || cy < 0) return 0;
							tMaxY = tMaxY + tDeltaY;
						} else {
							cz += stepZ;
							if (cz >= (int)steps || cz < 0) return 0;
							tMaxZ = tMaxZ + tDeltaZ;
						}
					}
					list = &grid[cx+cy*steps+cz*steps*steps];

					if (list && (list->count() == 0)) list = 0; // Continue until we find an non-empty list.
				} while(list == 0);

				maxT = currentT + minn(tMaxX, tMaxY, tMaxZ);
				return(list);
			}

		private:
			double currentT;
			double tDeltaX;
			double tDeltaY;
			double tDeltaZ;
			double tMaxX;
			double tMaxY;
			double tMaxZ;
			int stepX;
			int stepY;
			int stepZ;
			int cx;
			int cy;
			int cz;

			Vector3f size;
			Vector3f pos;
			Vector3f dir;
			unsigned int steps;
			Vector3f minPos;
			Vector3f maxPos;		
			QList<Object3D*>* grid;
		};


		RayTracer::RayTracer(EngineWidget* widget) {
			for (int i = 0; i < 16; i++) modelView[i] = widget->getModelViewCache()[i];
			for (int i = 0; i < 16; i++) projection[i] = widget->getProjectionCache()[i];
			for (int i = 0; i < 16; i++) viewPort[i] = widget->getViewPortCache()[i];

			Vector3f from;
			Vector3f to;
			widget->getBoundingBox(from,to);


			accelerator = new VoxelStepper(from,to, 10);
			backgroundColor = Vector3f(0,0,0);
			windowHeight = widget->width();
			windowWidth = widget->height();
			objects = widget->getObjects();
			for (int i = 0; i < objects.count(); i++) accelerator->registerObject(objects[i]);

		}

		Vector3f RayTracer::rayCastPixel(float x, float y) {
			static int rayID = 1;
			rayID++;
			pixels++;
			GLdouble ox1, oy1, oz1;
			GLdouble ox2, oy2, oz2;

			gluUnProject(x, y, 0.0f, modelView, projection, viewPort, &ox1, &oy1 ,&oz1);
			gluUnProject(x, y, 1.0f, modelView, projection, viewPort, &ox2, &oy2 ,&oz2);

			// Now we have the direction of the light-ray going into the screen (2D viewport)
			Vector3f startPoint = Vector3f((GLfloat)ox1, (GLfloat)oy1, (GLfloat)oz1);
			Vector3f direction = Vector3f((GLfloat)(ox2 - ox1), (GLfloat)(oy2 - oy1), (GLfloat)(oz2 - oz1));




			double lengthToClosest = -1;
			Vector3f foundNormal;
			GLfloat foundColor[4];

			Object3D* bestObj = 0;

			double maxT = 0;
			RayInfo ri;
			QList<Object3D*>* list = accelerator->setupRay(startPoint, direction, maxT);
			ri.startPoint = startPoint;
			ri.lineDirection = direction;			

					
			while (list != 0) { 
				// check objects
				for (int i = 0; i < list->count(); i++) {
					checks++;

					// Check if we already tested this...
					if (list->at(i)->getLastRayID() == rayID) continue;
		
					bool found = list->at(i)->intersectsRay(&ri);
					list->at(i)->setLastRayID(rayID);
					if (!found) continue;
					//return Vector3f(1,0,0);
				

					if ((ri.intersection>0) && ((ri.intersection <= lengthToClosest) || (lengthToClosest == -1))) {
						// We hit something and it was closer to us than the object before...
						foundNormal = ri.normal;
						for (int ix = 0; ix < 4; ix++) foundColor[ix] = ri.color[ix];							
						lengthToClosest = ri.intersection;
						bestObj = list->at(i);
					}
				}

				if (bestObj != 0 && lengthToClosest < maxT) break;
				list = accelerator->advance(maxT); 
				//list = 0;
			}
			
			//Object3D* hitObject = bestObj; // We remember the object we hit.

			// Now we can calculate the lightning.
			if (lengthToClosest>0) {	
				// iPoint is the intersection point in 3D.
				Vector3f iPoint = startPoint + direction*lengthToClosest;
				Vector3f lightDirection = (lightPos-iPoint);

				double light = 0;

				// This is a Phong lightning model, see e.g. (http://ai.autonomy.net.au/wiki/Graphics/Illumination)

				// -- Diffuse light 
				double diffuse = light1Diffuse*(Vector3f::dot(foundNormal, (lightDirection).normalized()));				
				if (diffuse<0) diffuse = 0;
				light += diffuse;

				// -- Specular light
				double spec = 0;
				double dot = Vector3f::dot(foundNormal, lightDirection);
				if (dot<0.1) {
				} else {
					Vector3f reflected = foundNormal*dot*2 - lightDirection;
					reflected.normalize();
					spec = -(Vector3f::dot(reflected, (direction).normalized()));		
					if (spec< 0.1) {
						spec = 0;
					} else {
						spec = light1Specular*pow(spec,50);
						if (spec<0) spec = 0;
					}
				}
				light += spec;

				// -- Ambient light
				double ambient = light1Ambient;
				light += ambient; 

				// -- calculate shadow...
				bool inShadow = false;
				bool calcShadow = true;
				if (calcShadow) {

					
					double maxT = 0;
					QList<Object3D*>* list = accelerator->setupRay(iPoint,(lightPos-iPoint), maxT);
					ri.startPoint = iPoint;
					ri.lineDirection = lightPos-iPoint;
			
					while (list != 0 && !inShadow) { 
						// check objects

						for (int i = 0; i < list->size(); i++) {
							if (list->at(i) == bestObj) continue; // self-shadow? (probably not neccesary, since the specular light will be negative)							
							inShadow = list->at(i)->intersectsRay(&ri);
							if (ri.intersection < 0 || ri.intersection > 1) inShadow = false;
							if (inShadow) break;								
						}

						list = accelerator->advance(maxT); 
					}
				}

				// Clamp light values.
				if (calcShadow && inShadow) light=ambient; // drop-shadow strength (only ambient light...)
				if (light > 1) light = 1;
				if (light < 0) light = 0;
				
				
				return Vector3f(light*foundColor[0],light*foundColor[1],light*foundColor[2]);;
			} else {
				return Vector3f(backgroundColor.x(),backgroundColor.y(),backgroundColor.z());
			}
		}



		QImage RayTracer::calculateImage(int w, int h) {

			float oh = (float)h; 
			float ow = (float)w; 
			float vh = (float)h; // windowHeight; 
			float vw = (float)w; // windowWidth; 


			// Setup progress dialog.
			QProgressDialog progress("progress", "Cancel", 0, 100);
			progress.setLabelText("Ray tracing");
			progress.setMinimumDuration(1000);

			TIME("Rendering...");
			QImage im(w,h, QImage::Format_RGB32);

			// Find a suitable light position. TODO: CHANGE!
			GLdouble sx1, sy1, sz1;				
			gluUnProject((float)0, vh/2.0, 0.0f, modelView, projection, viewPort, &sx1, &sy1 ,&sz1);				
			lightPos = Vector3f((GLfloat)sx1, (GLfloat)sy1, (GLfloat)sz1);
			light1Ambient = 0.4;
			light1Diffuse = 0.6;
			light1Specular = 0.7;

			pixels = 0;
			checks = 0;
			aaPixels = 0;
			for (int x = 0; x < w; x++) {
				float tx = (x*vw)/ow;
				if (x % 7 == 0) {
					progress.setValue((x*100)/w);
					qApp->processEvents();
					if	(progress.wasCanceled()) break;
				}

				for (int y = 0; y < h; y++) {	
					float ty = (y*vh)/oh;
					//Object3D* hitObject = 0;
					//bool inShadow = false;
					//if (pixels % 100 == 0) Debug(QString("x,y:%1,%2").arg(tx).arg(vh-ty));
					
					
					Vector3f color = rayCastPixel(tx,vh-ty);
					im.setPixel(x,y,qRgb(color.x()*255,color.y()*255,color.z()*255));	
						

				}
			}
			TIME();

			INFO(QString("Enabled objects: %1, inside viewport: %2").arg(objects.size()).arg("n.a."));
			INFO(QString("Pixels: %1, Object checks: %2, Objects checked per pixel: %3").
				arg(pixels).arg(checks).arg(checks/(float)pixels));
			INFO(QString("Pixels: %1, Needing AA: %2, Percentage: %3").
				arg(pixels).arg(aaPixels).arg(aaPixels/(float)pixels));

			progress.close();

			/// Here we show the result...
			QDialog* d = new QDialog();
			d->resize(w,h);
			QPixmap p = QPixmap::fromImage(im);
			QLabel* lb = new QLabel(d);
			lb->setPixmap(p);
			d->exec();
			delete(lb);
			delete(d);

			return im;
		}




		/*
		progress.setCaption("Ray-tracing Anti-Alias (Step 2/2)...");

		long aaPixels = 0;

		if (aaSamples>0) {
		TIME("Anti-Alias");
		float xs = (vw/ow);
		float ys = (vh/oh);
		for (unsigned int x = 1; x+1 < iw; x++) {

		float tx = (x*vw)/ow;


		progress.setProgress((x*100)/iw);
		qApp->processEvents();
		if (progress.wasCancelled()) break;

		for (unsigned int y = 1; y+1 < ih; y++) {


		float ty = (y*vh)/oh;


		if (
		(objMap[x+y*iw] != objMap[(x+1)+y*iw]) ||
		(objMap[x+y*iw] != objMap[(x-1)+y*iw]) ||
		(objMap[x+y*iw] != objMap[(x)+(y-1)*iw]) ||
		(objMap[x+y*iw] != objMap[(x)+(y+1)*iw]))
		{
		aaPixels++;

		Object3D* hitObject = 0;
		Object3D* hitObject2 = 0;
		bool inShadow = false;

		rayCastPixel(tx,
		vh-(ty), lightPos, hitObject2, &inShadow);


		Vector3 color;
		// TRICKY
		unsigned int steps = aaSamples;
		double xstepsize = xs/steps;
		double ystepsize = ys/steps;
		for (unsigned int xo = 0; xo < steps; xo++) {
		for (unsigned int yo = 0; yo < steps; yo++) {	
		Vector3 c = rayCastPixel(tx-xs/2.0 +xo*xstepsize+xstepsize/2.0,
		vh-(ty-ys/2.0 +yo*ystepsize+ystepsize/2.0), lightPos, hitObject, &inShadow);

		if (hitObject == hitObject2) {

		} else {
		color = color + c;
		}
		}
		}
		color = color / (steps*steps);
		im.setPixel(x,y,qRgb(color.x()*255,color.y()*255,color.z()*255));

		}

		}
		}
		TIME();
		*/



	}
}

