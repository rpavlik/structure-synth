#include "Object3D.h"


using namespace AppCore::Math;

namespace AppCore {
	namespace GLEngine {


		void Object3D::vertex4n(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4) { 
			
		

			Vector3f n = (v2-v1).cross(v4-v1);
			n = n.normalize();
			normal(n);
			vertex(v1); 
			//normal(n);
			vertex(v2); 
			//normal(n);
			
			/*
			GLfloat blue[4] = {0.0f, 0.0f, 1.0f, 1.0f} ;
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue );
				glMaterialfv( GL_FRONT, GL_SPECULAR, blue );
			*/
			
			vertex(v3); 
			//normal(n);
			vertex(v4); 

			
			
		}
		
		void Object3D::vertex4rn(AppCore::Math::Vector3f v1,AppCore::Math::Vector3f v2,AppCore::Math::Vector3f v3,AppCore::Math::Vector3f v4) { 
			
			Vector3f n = (v1-v2).cross(v4-v1);
			n = n.normalize();
			normal(n);
			vertex(v4); 
			//normal(n);
			vertex(v3); 
			//normal(n);
			vertex(v2); 
			//normal(n);
			vertex(v1); 

			
		}


	}

}