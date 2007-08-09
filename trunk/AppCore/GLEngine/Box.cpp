#include "Box.h"

#include "AppCore/Math/Vector3.h"

using namespace AppCore::Math;

namespace AppCore {
	namespace GLEngine {


		Box::Box(AppCore::Math::Vector3f base, 
				AppCore::Math::Vector3f dir1 , 
				 AppCore::Math::Vector3f dir2, 
				 AppCore::Math::Vector3f dir3) : base(base), v1(dir1), v2(dir2), v3(dir3) 
		{
		};

		Box::~Box() { };

		

		void Box::draw() {
			glPushMatrix();
			glTranslatef( base.x(), base.y(), base.z() );
			

			GLfloat col[4] = {0.0f, 1.0f, 1.0f, 0.1f} ;
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col );
			

			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
				

			glEnable(GL_CULL_FACE);
			/*
			glEnable (GL_BLEND);
			glBlendFnc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			glDisable (GL_DEPTH_TEST) ;
			*/

			Vector3f O(0,0,0);
			
			glEnable (GL_LIGHTING);

			glDisable(GL_CULL_FACE); // TODO: do we need this?
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			GLfloat c3[4] = {0.3f, 0.3f, 0.3f, 0.5f};
			glMaterialfv( GL_FRONT, GL_SPECULAR, c3 );
			glMateriali(GL_FRONT, GL_SHININESS, 127);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);
			
			

			/*
			GLfloat green[4] = {0.0f, 1.0f, 0.0f, 1.0f} ;
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green );
			glMaterialfv( GL_FRONT, GL_SPECULAR, green );
			glMaterialfv( GL_BACK, GL_AMBIENT_AND_DIFFUSE, green );
			glMaterialfv( GL_BACK, GL_SPECULAR, green );
			
	GLfloat red[4] = {1.0f, 0.0f, 0.0f, 1.0f} ;
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red );
				glMaterialfv( GL_FRONT, GL_SPECULAR, red );
			glMaterialfv( GL_BACK, GL_AMBIENT_AND_DIFFUSE, red );
				glMaterialfv( GL_BACK, GL_SPECULAR, red );
			*/

			glBegin( GL_QUADS );
			glColor4f(1,0,0,1);
			vertex4n(O, v2,v2+v1,v1);
			//glColor4f(1,1,0,1);
			vertex4rn(O+v3, v2+v3, v2+v1+v3, v1+v3);
			//glColor4f(1,1,1,1);
			vertex4n(O, v3, v3+v2,v2);
			vertex4rn(O+v1, v3+v1, v3+v2+v1, v2+v1);
			vertex4n(O, v1, v3+v1, v3);
			vertex4rn(O+v2, v1+v2, v3+v2+v1, v3+v2);
			glEnd();
			

		

			

			glPopMatrix();			
		};

	}
}