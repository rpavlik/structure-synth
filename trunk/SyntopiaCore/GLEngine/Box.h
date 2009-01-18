#pragma once

#include "SyntopiaCore/Math/Vector3.h"
#include "Object3D.h"

namespace SyntopiaCore {
	namespace GLEngine {	

		class Box : public Object3D {
		public:
			Box(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				 SyntopiaCore::Math::Vector3f dir2, 
				 SyntopiaCore::Math::Vector3f dir3);

			virtual ~Box();

			virtual void draw() const;

		private:
			SyntopiaCore::Math::Vector3f base;
			SyntopiaCore::Math::Vector3f v1;
			SyntopiaCore::Math::Vector3f v2;
			SyntopiaCore::Math::Vector3f v3;
  
		};

	}
}

