#pragma once

#include "SyntopiaCore/Math/Vector3.h"
#include "../Object3D.h"
#include "AtomicCounter.h"
#include "VoxelStepper.h"
#include "SyntopiaCore/Math/Random.h"

namespace SyntopiaCore {
	namespace GLEngine {	
		using namespace SyntopiaCore::Math;

		class RenderThread : public QThread {
		public:
			enum Task { RayTrace, AmbientOcclusion, AntiAlias } ;
			RenderThread();
			~RenderThread();
			void setTask(Task task) { this->task = task; };
			RenderThread(const RenderThread& other);
			void render();
			void rayTrace(int newUnit);
			void ambientOcclusion(int newUnit);
			void antiAlias(int newUnit);
			void setCounters(AtomicCounter* nextUnit, AtomicCounter* completedUnits, int maxUnits);
			void alloc(int w, int h);
			static void msleep(int i) { QThread::msleep(i); }
			void run();
			Vector3f rayCastPixel(float x, float y);
			Vector3f rayCastPixelWithDOF(float x, float y);
			
			Vector3f rayCast(Vector3f startPoint, Vector3f direction, Object3D* excludeThis, int level = 0);

		private:
			Task task;
			AtomicCounter* nextUnit;
			AtomicCounter* completedUnits;
			int maxUnits;
			int w;
			int h;

			// Shared between instances...
			double* depths;
			Vector3f* normals;
			Vector3f* colors;
			Vector3f* intersections;
			double* aoMap;
			Object3D** objs;

			Vector3f frontStart;
			Vector3f frontX;
			Vector3f frontY;
			Vector3f backStart;
			Vector3f backX;
			Vector3f backY;

			Vector3f lightPos;
			Vector3f backgroundColor;
			int rayID;
			int pixels;
			int checks;
			VoxelStepper* accelerator;

			int occlusionSampleStepSize;
			int ambMaxRays;
			int ambSmooth;
			int totalAOCasts;
			int aaSamples;
			int width;
			int height;
			bool useShadows;
			double dofCenter;
			double dofFalloff;

			Math::RandomNumberGenerator rg;

			Vector3f normal;
			double depth;
			Vector3f color;
			Vector3f intersection;
			Object3D* hitObject;
			bool copy;

			friend class RayTracer;
		};



	}
}

