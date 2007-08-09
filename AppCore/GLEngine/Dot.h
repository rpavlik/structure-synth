#pragma once

#include "AppCore/Math/Vector3.h"
#include "Object3D.h"

namespace AppCore {
	namespace GLEngine {	

		class Dot : public Object3D {
		public:
			Dot(AppCore::Math::Vector3f pos);

			virtual ~Dot();

			virtual void draw();

		private:
			AppCore::Math::Vector3f pos;
		};

	}
}