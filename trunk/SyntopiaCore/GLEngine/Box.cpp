#include "Box.h"

#include "SyntopiaCore/Math/Vector3.h"

using namespace SyntopiaCore::Math;

namespace SyntopiaCore {
	namespace GLEngine {


		Box::Box(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				 SyntopiaCore::Math::Vector3f dir2, 
				 SyntopiaCore::Math::Vector3f dir3) : base(base), v1(dir1), v2(dir2), v3(dir3) 
		{
			/// Bounding box
			from = base;
			to = base + dir1 + dir2 + dir3;

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
			
					
			glBegin( GL_QUADS );
			glColor4fv(primaryColor);
			vertex4n(O, v2,v2+v1,v1);
			vertex4rn(O+v3, v2+v3, v2+v1+v3, v1+v3);
			vertex4n(O, v3, v3+v2,v2);
			vertex4rn(O+v1, v3+v1, v3+v2+v1, v2+v1);
			vertex4n(O, v1, v3+v1, v3);
			vertex4rn(O+v2, v1+v2, v3+v2+v1, v3+v2);
			glEnd();
			
			glDisable(GL_COLOR_MATERIAL);
			

			glPopMatrix();			
		};

	}
}