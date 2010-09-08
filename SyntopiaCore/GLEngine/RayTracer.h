#pragma once

#include "Object3D.h"
#include "EngineWidget.h"
#include <QImage>
#include "SyntopiaCore/Math/Vector3.h"
#include "SyntopiaCore/Math/Matrix4.h"
#include "SyntopiaCore/Math/Random.h"
#include "Raytracer/VoxelStepper.h"
#include "Raytracer/RenderThread.h"



namespace SyntopiaCore {
	namespace GLEngine {

		using namespace SyntopiaCore::Math;
	
		class RayTracer {
		public:
			RayTracer(EngineWidget* widget);
			QImage calculateImage(int width, int height);
			void setParameter(QString param, QString value);
			bool wasCancelled() { return userCancelled; }
		private:			
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

			long aaPixels;	
			bool userCancelled;
			
			int sizeX;
			int sizeY;
			int maxThreads;

			RenderThread rt;
		};

	}
}

