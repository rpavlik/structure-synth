#include "Line.h"

#include "AppCore/Math/Vector3.h"

using namespace AppCore::Math;

namespace AppCore {
	namespace GLEngine {


		//GLUquadric* Sphere::myQuad = 0;    

		Line::Line(AppCore::Math::Vector3f from, AppCore::Math::Vector3f to) : from(from), to(to)
		{
		};

		Line::~Line() {
		};

		void vertex(Vector3f v) {
			glVertex3f(v.x(), v.y(), v.z());
		}

		void Line::draw() {
			
			glLineWidth( 1.0 );
			glDisable (GL_LIGHTING);
			glColor3f( 1.0f, 1.0f, 1.0f );
			
			glBegin(GL_LINES);
			vertex(from);
			vertex(to);
			glEnd();

			glEnable (GL_LIGHTING);
			
		};

	}
}