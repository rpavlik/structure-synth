#pragma once

#include "SyntopiaCore/Math/Vector3.h"
#include "Object3D.h"

namespace SyntopiaCore {
	namespace GLEngine {	

		class Mesh : public Object3D {
		public:
			Mesh(SyntopiaCore::Math::Vector3f startBase, 
				SyntopiaCore::Math::Vector3f startDir1 , 
				 SyntopiaCore::Math::Vector3f startDir2, 
				 SyntopiaCore::Math::Vector3f endBase, 
				SyntopiaCore::Math::Vector3f endDir1 , 
				 SyntopiaCore::Math::Vector3f endDir2 
				);

			virtual ~Mesh();

			virtual QString name() { return "Mesh"; }
			
			virtual void draw() const;

		private:
			SyntopiaCore::Math::Vector3f startBase;
			SyntopiaCore::Math::Vector3f startDir1;
			SyntopiaCore::Math::Vector3f startDir2;
			SyntopiaCore::Math::Vector3f endBase;
			SyntopiaCore::Math::Vector3f endDir1;
			SyntopiaCore::Math::Vector3f endDir2;   
		};

	}
}

