#include "Dot.h"

#include "AppCore/Math/Vector3.h"

using namespace AppCore::Math;

namespace AppCore {
	namespace GLEngine {

		Dot::Dot(AppCore::Math::Vector3f pos) : pos(pos)
		{
			/// Bounding box
			from = pos;
			to = pos;
		};

		Dot::~Dot() { };

		void Dot::draw() {
			glDisable (GL_LIGHTING);
			glColor3f( 1.0f, 1.0f, 1.0f );
			glBegin(GL_POINTS);
			glVertex3f(pos.x(), pos.y(), pos.z());
			glEnd();			
			
			glEnable (GL_LIGHTING);

		};

	}
}