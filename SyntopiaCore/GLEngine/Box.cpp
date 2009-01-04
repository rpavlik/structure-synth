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

		

		void Box::draw() const {
			glPushMatrix();
			glTranslatef( base.x(), base.y(), base.z() );
			
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, primaryColor );
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
			
			glEnable (GL_LIGHTING);
			glEnable(GL_CULL_FACE); // TODO: do we need this?
			
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glMateriali( GL_FRONT, GL_SPECULAR, 30 );
			glMateriali(GL_FRONT, GL_SHININESS, 127);
			
			glBegin( GL_QUADS );
			Vector3f O(0,0,0);
			vertex4n(O, v2,v2+v1,v1);
			vertex4rn(O+v3, v2+v3, v2+v1+v3, v1+v3);
			vertex4n(O, v3, v3+v2,v2);
			vertex4rn(O+v1, v3+v1, v3+v2+v1, v2+v1);
			vertex4n(O, v1, v3+v1, v3);
			vertex4rn(O+v2, v1+v2, v3+v2+v1, v3+v2);
			glEnd();
			
			glPopMatrix();			
		};

	}
}

