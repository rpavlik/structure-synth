#pragma once

#include "SyntopiaCore/Math/Vector3.h"
#include "Object3D.h"

namespace SyntopiaCore {
	namespace GLEngine {	

		class Sphere : public Object3D {
		public:
			Sphere(SyntopiaCore::Math::Vector3f center, float radius);
			virtual ~Sphere();

			virtual void draw() const;

		private:
			SyntopiaCore::Math::Vector3f center;
			float radius;
			GLUquadric* myQuad;    
		};

	}
}

