#include "PrimitiveRule.h"
#include "Builder.h"

#include "../../AppCore/Exceptions/Exception.h"
#include "../../AppCore/Logging/Logging.h"

using namespace AppCore::Exceptions;
using namespace AppCore::Logging;

#include "../../AppCore/Math/Vector3.h"

using namespace AppCore::Math;

namespace StructureSynth {
	namespace Model {	
		PrimitiveRule::PrimitiveRule(PrimitiveType type) : type(type) {

			// enum PrimitiveType { Box, Sphere, Point, Cylinder, Line } ;			
			if (type == Box) {
				name = "box";
			} else if (type == Sphere) {
				name = "sphere";
			} else if (type == Point) {
				name = "point";
			} else if (type == Cylinder) {
				name = "cylinder";
			} else if (type == Line) {
				name = "line";
			} else {
				WARNING("PrimitiveRule constructor: unknown PrimitiveType");
			}

		};

		void PrimitiveRule::apply(Builder* b) {

			if (type == Sphere) {
				Vector3f v(0,0,0);
				Vector3f v1 = b->getState().matrix * v;
				Vector3f c =  b->getState().matrix * Vector3f(0.5,0.5,0.5);
				double r =  (c-b->getState().matrix * Vector3f(0.5,0.5,0.0)).length();

				b->getRenderer()->drawSphere(c,r);
			} else if (type == Box) {
				Vector3f v(0,0,0);

				Vector3f v1 = b->getState().matrix * v;
				Vector3f v2 = b->getState().matrix * Vector3f(1,0,0);
				Vector3f v3 = b->getState().matrix * Vector3f(0,1,0);
				Vector3f v4 = b->getState().matrix * Vector3f(0,0,1);

				b->getRenderer()->drawBox(v1,v2-v1,v3-v1,v4-v1);
				
			}

		};
	
				
	}
}