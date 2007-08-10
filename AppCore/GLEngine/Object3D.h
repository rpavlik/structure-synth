#pragma once

#include "AppCore/Math/Vector3.h"
#include <QGLWidget>

namespace AppCore {
	namespace GLEngine {	

		class Object3D {
		public:
			Object3D() {};
			virtual ~Object3D() {};

			virtual void draw() = 0;
		protected:

			void vertex(AppCore::Math::Vector3f v) { glVertex3f(v.x(), v.y(), v.z()); }
			void normal(AppCore::Math::Vector3f v) { glNormal3f(v.x(), v.y(), v.z()); }
			void vertex4(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4) { vertex(v1); vertex(v2); vertex(v3); vertex(v4); }
			void vertex4r(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4) { vertex(v4); vertex(v3); vertex(v2); vertex(v1); }
			void vertex4n(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4);
			void vertex4rn(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4);
			void vertex4nc(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4,AppCore::Math::Vector3f center);
			void vertex4rnc(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4,AppCore::Math::Vector3f center);
		private:
		};

	}
}