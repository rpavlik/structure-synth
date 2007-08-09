#pragma once

#include "AppCore/Math/Vector3.h"
#include "Object3D.h"

namespace AppCore {
	namespace GLEngine {	

		class Grid : public Object3D {
		public:
			Grid(AppCore::Math::Vector3f base, 
				AppCore::Math::Vector3f dir1 , 
				 AppCore::Math::Vector3f dir2, 
				 AppCore::Math::Vector3f dir3);

			virtual ~Grid();

			virtual void draw();

		private:
			AppCore::Math::Vector3f base;
			AppCore::Math::Vector3f v1;
			AppCore::Math::Vector3f v2;
			AppCore::Math::Vector3f v3;
			GLUquadric* myQuad;    
		};

	}
}