#include "RenderThread.h"

#include "SyntopiaCore/Math/Vector3.h"

using namespace SyntopiaCore::Math;

namespace SyntopiaCore {
	namespace GLEngine {
		
			class Sampler {
			public:
				Sampler(Math::RandomNumberGenerator* rg) : rg(rg) {
				}
				
				virtual Vector3f getAODirection() {
					return rg->getUniform3D();
				}

				virtual Vector3f getLensSample() {
					return rg->getUniform2D();
				}

				virtual void prepareSamples(int nSamplesSqrt, int nAOSamplesSqrt) {};

			protected:
				Math::RandomNumberGenerator* rg;
			};

			class StratifiedSampler : public Sampler {
			public:
				StratifiedSampler(Math::RandomNumberGenerator* rg) : Sampler(rg) {
					aoSampleIndex = 0;
				}

				virtual Vector3f getAODirection() {
					//return rg->getUniform3D();
					if (aoSampleIndex>=aoSamples.count()) throw 1;
					return aoSamples[aoSampleIndex++];
				}

				virtual Vector3f getLensSample() {
					return rg->getUniform2D();
				}

				// Based on the Physical Based Rendering book
				Vector3f sampleSphere(double u1, double u2) {
					double z = 1.0 - 2.0*u1;
					double r = 0;
					if (1.0-z*z > 0) r = sqrt(1.0-z*z);
					double phi = 2.0 * 3.1415926 * u2;
					double x = r * cos(phi);
					double y = r * sin(phi);
					return Vector3f(x,y,z);
				}

				virtual void prepareSamples(int nSamplesSqrt, int nAOSamplesSqrt) {
					aoSamples = QVector<Vector3f>(nSamplesSqrt*nSamplesSqrt);
					int count = 0;
					for (int i = 0; i < nSamplesSqrt; i++) {
						for (int j = 0; j < nSamplesSqrt; j++) {
							// we need a uniform number in the interval
							// [i/nSamplesSqrt;(i+1)/nSamplesSqrt]
							double x = rg->getDouble( ((double)i)/(double)nSamplesSqrt,((double)(i+1.0))/(double)nSamplesSqrt);
							double y = rg->getDouble( ((double)j)/(double)nSamplesSqrt,((double)(j+1.0))/(double)nSamplesSqrt);
							aoSamples[count++] = sampleSphere(x,y);
							//aoSamples.append(rg->getUniform3D());
						}	
					}
					aoSampleIndex = 0;
				};
			private:
				int aoSampleIndex;
				QVector<Vector3f> aoSamples;
				QVector<Vector3f> lensSamples;
			};

		RenderThread::RenderThread() {
			backgroundColor = Vector3f(0,0,0);
			occlusionSampleStepSize = 0;
			ambMaxRays = 0;
			aaSamples = 2;
			ambSmooth = 0;
			useShadows = true;
			copy = false;
			lightPos = Vector3f(0,0,0);

			dofCenter = 0;
			dofFalloff = 0;
			sampler = new StratifiedSampler(&rg);
		}

		
		RenderThread::~RenderThread() {
			if (!copy) {
				delete[] aoMap;
				delete[] colors;
				delete[] normals;
				delete[] depths;
				delete[] objs;
				delete[] intersections;
			}
			delete accelerator;
			delete sampler;
		}


		RenderThread::RenderThread(const RenderThread& other) {
			depths = other.depths;
			normals = other.normals;
			colors = other.colors;
			intersections= other.intersections;
			aoMap = other.aoMap;
			objs = other.objs;

			rayIDs = other.rayIDs;

			frontStart = other.frontStart;
			frontX = other.frontX;
			frontY = other.frontY;
			backStart = other.backStart;
			backX = other.backX;
			backY = other.backY;

			lightPos = other.lightPos;
			backgroundColor = other.backgroundColor;
			accelerator = new VoxelStepper(*other.accelerator);
			accelerator->setCopy(true);

			occlusionSampleStepSize = other.occlusionSampleStepSize;
			ambMaxRays = other.ambMaxRays;
			ambSmooth = other.ambSmooth;
			width = other.width;
			height = other.height;
			useShadows = other.useShadows;
			w = other.w;
			h = other.h;

			task = other.task;
			nextUnit = other.nextUnit;
			completedUnits = other.completedUnits;
			maxUnits = other.maxUnits;

			aaSamples = other.aaSamples;

			dofCenter = other.dofCenter;
			dofFalloff = other.dofFalloff;

			copy = true;

			rayID = 0;
			pixels = 0;
			checks = 0;
			rg.randomizeUniformCounter(); // to avoid coherence between threads
			sampler = new Sampler(&rg);
		};

		

		double RenderThread::getAOStrength(Object3D* object, Vector3f objectNormal, Vector3f objectIntersection) {
			
			if (ambMaxRays == 0 || object==0) return 1.0;
				int tests = 0;
				int hits = 0;
				for (int ix = 0; ix < ambMaxRays; ix++) {
					tests++;
					
					Vector3f random = sampler->getAODirection();
					random.normalize();
					if (Vector3f::dot(random, objectNormal)<0) random = random*-1.0; // Only check away from surface.

					double maxT = 0;
					QList<Object3D*>* list = accelerator->setupRay(objectIntersection,random, maxT);
					RayInfo ri;
					ri.startPoint = objectIntersection;
					ri.lineDirection = random;
					bool occluded = false;
					while (list != 0 && !occluded) { 
						// check objects
						for (int i = 0; i < list->size(); i++) {
							if (list->at(i) == object) continue; // self-shadow? 							
							occluded = list->at(i)->intersectsRay(&ri);
							if (ri.intersection < 1E-5) occluded = false;
							if (occluded) break;								
						}
						if (!occluded) list = accelerator->advance(maxT); 
					}
					if (occluded) hits++;
				}
				return 1-hits/(double)tests;
		}

		
		void RenderThread::raytrace(int newUnit) {
			int x = newUnit-1;
			float fx = x/(float)(w);
			float xs = (1.0/w);
			float ys = (1.0/h);

				
			for (int y = 0; y < h; y++) {	

				sampler->prepareSamples(aaSamples,1);
			
				float fy = y/(float)(h);
				Vector3f color(0,0,0);

				unsigned int steps = aaSamples;
				double xstepsize = xs/steps;
				double ystepsize = ys/steps;

				for (unsigned int xo = 0; xo < steps; xo++) {
					for (unsigned int yo = 0; yo < steps; yo++) {
							Vector3f c =  rayCastPixel(fx-xs/2.0 +xo*xstepsize+xstepsize/2.0,
													(fy-ys/2.0 +yo*ystepsize+ystepsize/2.0));
							color = color + c * getAOStrength(hitObject, normal, intersection);
					}
				}
				
				color = color / (steps*steps);
				colors[x+y*w] = color;
			}
		};

		
		void RenderThread::run() {
			int newUnit = nextUnit->increase();
			while (newUnit <= maxUnits) {
				// do work here...
				switch (task) {
					case Raytrace: raytrace(newUnit); break;
					default: throw(1);
				}
				completedUnits->increase();
				newUnit = nextUnit->increase();
			}
		}


		void RenderThread::setCounters(AtomicCounter* nextUnit, AtomicCounter* completedUnits, int maxUnits)
		{
			this->nextUnit = nextUnit;
			this->completedUnits = completedUnits;
			this->maxUnits = maxUnits;
		}

		void RenderThread::alloc(int w, int h) {
			this->w = w;
			this->h = h;
			depths = new double[w*h];
			for (int i = 0; i<w*h; i++) depths[i] = -1;
			normals = new Vector3f[w*h];
			colors = new Vector3f[w*h];
			intersections = new Vector3f[w*h];
			aoMap = new double[w*h];
			for (int i = 0; i<w*h; i++) aoMap[i] = -1;
			objs = new Object3D*[w*h];

			rayID = 0;
			pixels = 0;
			checks = 0;
		}


	

		Vector3f RenderThread::rayCastPixel(float x, float y) {
		
			Vector3f startPoint = frontStart + frontX*x + frontY*y;
			Vector3f endPoint  =   backStart  + backX*x  + backY*y;
			
			if (dofCenter == 0) {
				Vector3f direction = endPoint - startPoint;
				return rayCast(startPoint, direction, 0);
			} else {
				Vector3f centerPoint =(endPoint-startPoint)* dofCenter+ startPoint;
				// --- Uniform Disc Sampling
				Vector3f displace = sampler->getLensSample()*dofFalloff;
				Vector3f newStartPoint = frontStart + frontX*(x+displace.x())+ frontY*(y+displace.y());	
				Vector3f direction = (centerPoint - newStartPoint)*(1/dofCenter);
				return rayCast(newStartPoint, direction, 0);
			}

		}

		Vector3f RenderThread::rayCast(Vector3f startPoint, Vector3f direction, Object3D* excludeThis, int level) {
			if (level>4) return Vector3f(backgroundColor.x(),backgroundColor.y(),backgroundColor.z());
			rayID++;
			pixels++;

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

					if (list->at(i) == excludeThis) continue;
					// Check if we already tested this...
					if (rayIDs[list->at(i)->getObjectID()] == rayID) continue;

					bool found = list->at(i)->intersectsRay(&ri);
					rayIDs[list->at(i)->getObjectID()]= rayID;
					if (!found) continue;
					if ((ri.intersection<1E-7)) continue;

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
			}
			// Now we can calculate the lightning.
			if (lengthToClosest>0) {	
				// iPoint is the intersection point in 3D.
				Vector3f iPoint = startPoint + direction*lengthToClosest;
				Vector3f lightDirection = (lightPos-iPoint);
				double light = 0;

				// This is a Phong lightning model, see e.g. (http://ai.autonomy.net.au/wiki/Graphics/Illumination)
				// -- Diffuse light 
				double diffuse = bestObj->getPrimitiveClass()->diffuse*(Vector3f::dot(foundNormal, (lightDirection).normalized()));				
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
						spec = bestObj->getPrimitiveClass()->specular*pow(spec,50);
						if (spec<0) spec = 0;
					}
				}
				light += spec;

				// -- Ambient light
				double ambient = bestObj->getPrimitiveClass()->ambient;
				light += ambient; 

				// -- calculate shadow...
				// TODO: Calculate shadow in transperant media
				bool inShadow = false;
				if (useShadows) {
					double maxT = 0;
					QList<Object3D*>* list = accelerator->setupRay(iPoint,(lightPos-iPoint), maxT);
					ri.startPoint = iPoint;
					ri.lineDirection = lightPos-iPoint;

					while (list != 0 && !inShadow) { 
						// check objects
						for (int i = 0; i < list->size(); i++) {
							if (list->at(i) == bestObj) continue; // self-shadow? (probably not neccesary, since the specular light will be negative)							
							inShadow = list->at(i)->intersectsRay(&ri);
							if (ri.intersection < 1E-5 || ri.intersection > 1) inShadow = false;
							if (ri.color[3]<1) inShadow=false;
							if (inShadow) break;								
						}

						if (!inShadow) list = accelerator->advance(maxT); 
					}
				}

				// Clamp light values.
				if (useShadows && inShadow) light=ambient; // drop-shadow strength (only ambient light...)
				if (light > 1) light = 1;
				if (light < 0) light = 0;

				if (foundColor[3] < 1) {
					Vector3f color = rayCast(iPoint, direction, bestObj, level+5); 
					foundColor[0] = light*foundColor[0]*(foundColor[3]) + color.x()*(1-foundColor[3]);
					foundColor[1] = light*foundColor[1]*(foundColor[3]) + color.y()*(1-foundColor[3]);
					foundColor[2] = light*foundColor[2]*(foundColor[3]) + color.z()*(1-foundColor[3]);
				}


				double reflection = bestObj->getPrimitiveClass()->reflection;

				if (reflection > 0) {
					Vector3f nDir = foundNormal*(-2)*Vector3f::dot(foundNormal, direction)/foundNormal.sqrLength() + direction;
					Vector3f color = rayCast(iPoint, nDir, bestObj, level+1);
					Vector3f thisColor = Vector3f(light*foundColor[0],light*foundColor[1],light*foundColor[2]) *(1-reflection)
						+ color*(reflection);
					foundColor[0] = light*foundColor[0]*(1-reflection) + color.x()*reflection;
					foundColor[1] = light*foundColor[1]*(1-reflection) + color.y()*reflection;
					foundColor[2] = light*foundColor[2]*(1-reflection) + color.z()*reflection;
					if (foundColor[0]>1) foundColor[0] = 1;
					if (foundColor[1]>1) foundColor[1] = 1;
					if (foundColor[2]>1) foundColor[2] = 1;
				}

				if (level==-1 && ambMaxRays>0) {
					int hits = 0;
					int tests = 0;
					totalAOCasts++;
					for (int ix = 0; ix < ambMaxRays; ix++) {
						tests++;
						// Use monte carlo sampling to obtain random vector.
						Vector3f random = sampler->getAODirection();
						if (Vector3f::dot(random, foundNormal)<0) random = random*-1.0; // Only check away from surface.

						double maxT = 0;
						QList<Object3D*>* list = accelerator->setupRay(iPoint,random, maxT);
						ri.startPoint = iPoint;
						ri.lineDirection = random;
						bool occluded = false;
						while (list != 0 && !occluded) { 
							// check objects
							for (int i = 0; i < list->size(); i++) {
								if (list->at(i) == bestObj) continue; // self-shadow? 							
								occluded = list->at(i)->intersectsRay(&ri);
								if (ri.intersection < 1E-5) occluded = false;
								if (occluded) break;								
							}

							if (!occluded) list = accelerator->advance(maxT); 
						}

						if (occluded) hits++;

					}
					double occ = (1-hits/(double)tests);
					/*
					if (occ < 0.4) occ = 0;
					else if (occ > 1) occ = 1;
					else occ = (occ-0.4)/(1-0.4);
					occ = occ*occ;

					*/
					light = occ*light;
				}


				normal = foundNormal;
				depth = lengthToClosest;
				color =  Vector3f(light*foundColor[0],light*foundColor[1],light*foundColor[2]);
				intersection = iPoint;
				hitObject = bestObj;
				return color;
			} else {
				depth = 10; // Real depth is [0;1]
				normal = Vector3f(0,0,0);
				color = Vector3f(backgroundColor.x(),backgroundColor.y(),backgroundColor.z());
				intersection =  Vector3f(0,0,0);
				hitObject = 0;
				return color;
			}
		}


		void RenderThread::setObjects(int count) {
			rayIDs = QVector<int>(count, -1);
		}
			

	}
}

