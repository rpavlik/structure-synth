#pragma once

#include "Object3D.h"
#include "EngineWidget.h"
#include <QImage>
#include "SyntopiaCore/Math/Vector3.h"
#include "SyntopiaCore/Math/Matrix4.h"
#include "SyntopiaCore/Math/Random.h"



namespace SyntopiaCore {
	namespace GLEngine {

		using namespace SyntopiaCore::Math;
	
		class VoxelStepper; // Forward decl.

		class RayTracer {
		public:
			RayTracer(EngineWidget* widget);

			QImage calculateImage(int width, int height);
			void setParameter(QString param, QString value);
			void setBackgroundColor(Vector3f b) { backgroundColor = b; }

		private:

			Vector3f rayCastPixel(float x, float y);
			Vector3f rayCast(Vector3f startPoint, Vector3f direction, Object3D* excludeThis, int level = 0);

			Vector3f frontStart;
			Vector3f frontX;
			Vector3f frontY;
			Vector3f backStart;
			Vector3f backX;
			Vector3f backY;

			QList<Object3D*> objects;
			float xmin;
			float ymin;
			float xmax;
			float ymax;
			
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

			Math::RandomNumberGenerator rg;

			int ambMinRays;
			int ambMaxRays;
			float ambPrecision;
			int aaSamples;
			int width;
			int height;
			bool useShadows;
			double globalAmbient;
			double globalDiffuse;
			double globalSpecular;
			double reflection;
			double precision;

		};

	}
}

