#pragma once

#include "AppCore/Math/Vector3.h"
#include "Object3D.h"

namespace AppCore {
	namespace GLEngine {	

		class Line : public Object3D {
		public:
			Line(AppCore::Math::Vector3f from, AppCore::Math::Vector3f to);

			virtual ~Line();

			virtual void draw();

		private:
			AppCore::Math::Vector3f from;
			AppCore::Math::Vector3f to;
		};

	}
}