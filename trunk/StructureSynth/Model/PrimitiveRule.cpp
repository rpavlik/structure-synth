#include "PrimitiveRule.h"
#include "Builder.h"

#include "../../SyntopiaCore/Exceptions/Exception.h"
#include "../../SyntopiaCore/Logging/Logging.h"
#include "../../SyntopiaCore/Misc/ColorUtils.h"

using namespace SyntopiaCore::Exceptions;
using namespace SyntopiaCore::Logging;

#include "../../SyntopiaCore/Math/Vector3.h"

using namespace SyntopiaCore::Math;

namespace StructureSynth {
	namespace Model {	
		PrimitiveRule::PrimitiveRule(PrimitiveType type) : type(type) {

			// enum PrimitiveType { Box, Sphere, Point, Cylinder, Line } ;			
			if (type == Box) {
				name = "box";
			} else if (type == Sphere) {
				name = "sphere";
			} else if (type == Dot) {
				name = "dot";
			} else if (type == Grid) {
				name = "grid";
			} else if (type == Cylinder) {
				name = "cylinder";
			} else if (type == Line) {
				name = "line";
			} else {
				WARNING("PrimitiveRule constructor: unknown PrimitiveType");
			}

		};

		void PrimitiveRule::apply(Builder* b) const {
			b->increaseObjectCount();
			b->getRenderer()->setColor(
				SyntopiaCore::Misc::ColorUtils::HSVtoRGB( b->getState().hsv)
			);

			b->getRenderer()->setAlpha( b->getState().alpha );
			
			if (type == Sphere) {
				Vector3f v(0,0,0);
				Vector3f v1 = b->getState().matrix * v;
				Vector3f c =  b->getState().matrix * Vector3f(0.5,0.5,0.5);
				Vector3f c2 = (b->getState().matrix * Vector3f(0.5,0.5,0.0));
				double r =  (c-  c2).length();

				b->getRenderer()->drawSphere(c,r,classID);
			} else if (type == Box) {
				Vector3f v(0,0,0);

				Vector3f v1 = b->getState().matrix * v;
				Vector3f v2 = b->getState().matrix * Vector3f(1,0,0);
				Vector3f v3 = b->getState().matrix * Vector3f(0,1,0);
				Vector3f v4 = b->getState().matrix * Vector3f(0,0,1);

				b->getRenderer()->drawBox(v1,v2-v1,v3-v1,v4-v1,classID);
				
			} else if (type == Grid) {
				Vector3f v(0,0,0);

				Vector3f v1 = b->getState().matrix * v;
				Vector3f v2 = b->getState().matrix * Vector3f(1,0,0);
				Vector3f v3 = b->getState().matrix * Vector3f(0,1,0);
				Vector3f v4 = b->getState().matrix * Vector3f(0,0,1);

				b->getRenderer()->drawGrid(v1,v2-v1,v3-v1,v4-v1,classID);
			} else if (type == Dot) {
				Vector3f v = b->getState().matrix * Vector3f(0.5,0.5,0.5);
				
				b->getRenderer()->drawDot(v,classID);
			} else if (type == Line) {
				Vector3f v = b->getState().matrix * Vector3f(0,0.5,0.5);
				Vector3f v2 = b->getState().matrix * Vector3f(1,0.5,0.5);
				
				b->getRenderer()->drawLine(v,v2,classID);
			}

		};
	
				
	}
}

