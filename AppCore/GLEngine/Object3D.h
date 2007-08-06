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
		private:
		};

	}
}