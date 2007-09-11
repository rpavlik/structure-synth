#include "Object3D.h"


using namespace AppCore::Math;

namespace AppCore {
	namespace GLEngine {

		void Object3D::setColor(AppCore::Math::Vector3f rgb, float alpha) { 
			primaryColor[0] = rgb[0]; 
			primaryColor[1] = rgb[1]; 
			primaryColor[2] = rgb[2]; 
			primaryColor[3] = alpha; 
		}


		void Object3D::vertex4n(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4) { 
			Vector3f n = (v2-v1).cross(v4-v1);
			n = n.normalize();
			normal(n);
			vertex(v1); 
			vertex(v2); 
			vertex(v3); 
			vertex(v4); 
		}
		
		void Object3D::vertex4rn(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4) { 			
			Vector3f n = (v1-v2).cross(v4-v1);
			n = n.normalize();
			normal(n);
			vertex(v4); 
			vertex(v3); 
			vertex(v2); 
			vertex(v1); 
		}

		void Object3D::vertex4nc(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4,AppCore::Math::Vector3f center) { 
			normal((v1-center).normalize());
			vertex(v1); 
			normal((v2-center).normalize());
			vertex(v2); 
			normal((v3-center).normalize());
			vertex(v3); 
			normal((v4-center).normalize());
			vertex(v4); 
		}
		
		void Object3D::vertex4rnc(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4,AppCore::Math::Vector3f center) { 			
			normal((v4-center).normalize());
			vertex(v4); 
			normal((v3-center).normalize());
			vertex(v3); 
			normal((v2-center).normalize());
			vertex(v2); 
			normal((v1-center).normalize());
			vertex(v1); 
		}


		void Object3D::getBoundingBox(AppCore::Math::Vector3f& from, AppCore::Math::Vector3f& to) {
			from = this->from;
			to = this->to;
		};

		void Object3D::expandBoundingBox(AppCore::Math::Vector3f& from, AppCore::Math::Vector3f& to) {
			for (unsigned int i = 0; i < 3; i++) if (this->from[i] < from[i]) from[i] = this->from[i];
			for (unsigned int i = 0; i < 3; i++) if (this->to[i] > to[i]) to[i] = this->to[i];
		};


	}

}