#pragma once

#include "Object3D.h"
#include "EngineWidget.h"
#include <QImage>
#include "SyntopiaCore/Math/Vector3.h"
#include "SyntopiaCore/Math/Matrix4.h"



namespace SyntopiaCore {
	namespace GLEngine {

		using namespace SyntopiaCore::Math;
	
		class VoxelStepper; // Forward decl.

		class RayTracer {
		public:
			RayTracer(EngineWidget* widget);

			QImage calculateImage(int width, int height);

		private:

			Vector3f rayCastPixel(float x, float y);
			Vector3f rayCast(Vector3f startPoint, Vector3f direction);

			Vector3f frontStart;
			Vector3f frontX;
			Vector3f frontY;
			Vector3f backStart;
			Vector3f backX;
			Vector3f backY;

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

