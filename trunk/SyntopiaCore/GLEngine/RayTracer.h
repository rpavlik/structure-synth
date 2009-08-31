#pragma once

#include "Object3D.h"
#include "EngineWidget.h"
#include <QImage>
#include "SyntopiaCore/Math/Vector3.h"
#include "SyntopiaCore/Math/Matrix4.h"



namespace SyntopiaCore {
	namespace GLEngine {

		using namespace SyntopiaCore::Math;


		/// See here for details about this approach:
		/// http://www.devmaster.net/articles/raytracing_series/part4.php
		/// THIS IS A DUMMY IMPLEMENTATION!
		class VoxelStepper {
		public:
			VoxelStepper(Vector3f /*minPos*/, Vector3f /*maxPos*/, unsigned int /*steps*/) {};
			void registerObject(Object3D* obj) { list.append(obj); }
			QList<Object3D*>* setupRay(Vector3f /*pos*/, Vector3f /*dir*/, double& /*maxT*/) { return &list; }
			QList<Object3D*>* advance(double& /*maxT*/) { return 0; }

		private:
			QList<Object3D*> list;
		};


		class RayTracer {
		public:
			RayTracer(EngineWidget* widget);

			QImage calculateImage(int width, int height);

		private:

			Vector3f rayCastPixel(float x, float y);

			QList<Object3D*> objects;
			int aaSamples;
			int width;
			int height;
			float xmin;
			float ymin;
			float xmax;
			float ymax;
			bool useShadows;

			int windowWidth;
			int windowHeight;

			// Matrices from the OpenGL view.
			GLdouble modelView[16];
			GLdouble projection[16];
			GLint viewPort[16];


			VoxelStepper* accelerator;

			long pixels;
			long checks;
			long aaPixels;

			Vector3f backgroundColor;
			Vector3f lightPos;
			float light1Ambient;
			float light1Diffuse;
			float light1Specular;

		};

	}
}

