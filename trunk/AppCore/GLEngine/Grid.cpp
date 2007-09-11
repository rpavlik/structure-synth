#include "Grid.h"

#include "AppCore/Math/Vector3.h"

using namespace AppCore::Math;

namespace AppCore {
	namespace GLEngine {

		Grid::Grid(AppCore::Math::Vector3f base, 
				AppCore::Math::Vector3f dir1 , 
				 AppCore::Math::Vector3f dir2, 
				 AppCore::Math::Vector3f dir3) : base(base), v1(dir1), v2(dir2), v3(dir3) 
		{
			/// Bounding box
			from = base;
			to = base + dir1 + dir2 + dir3;
		};


		Grid::~Grid() { };

		void Grid::draw() {
			glPushMatrix();
			glTranslatef( base.x(), base.y(), base.z() );
			glLineWidth( 1.0 );

			glDisable (GL_LIGHTING);
			glColor3f( 1.0f, 1.0f, 1.0f );
			

			glBegin( GL_LINE_LOOP  );
			Vector3f O(0,0,0);
			vertex(O);
			vertex(v2);
			vertex(v2+v1);
			vertex(v1);
			glEnd();

			glBegin( GL_LINE_LOOP  );
			vertex(v3);
			vertex(v2+v3);
			vertex(v2+v1+v3);
			vertex(v1+v3);
			glEnd();
			
			glBegin( GL_LINES  );
			vertex( v3 );   vertex( O );
			vertex( v2 );   vertex( v2+v3 );
			vertex( v1+v2 );   vertex( v1+v2+v3 );
			vertex( v1 );   vertex( v1+v3 );
			glEnd();



			glEnable (GL_LIGHTING);
			

			glPopMatrix();			
		};

	}
}