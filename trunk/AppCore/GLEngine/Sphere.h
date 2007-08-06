#pragma once

#include "AppCore/Math/Vector3.h"
#include "Object3D.h"

namespace AppCore {
	namespace GLEngine {	

		class Sphere : public Object3D {
		public:
			Sphere(AppCore::Math::Vector3f center, float radius);
			virtual ~Sphere();

			virtual void draw();

		private:
			AppCore::Math::Vector3f center;
			float radius;
			GLUquadric* myQuad;    
		};

	}
}