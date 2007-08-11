#include "OpenGLRenderer.h"
#include "../../../AppCore/GLEngine/Sphere.h"
#include "../../../AppCore/GLEngine/Box.h"
#include "../../../AppCore/GLEngine/Grid.h"
#include "../../../AppCore/GLEngine/Dot.h"
#include "../../../AppCore/GLEngine/Line.h"
#include "../../../AppCore/Math/Vector3.h"

using namespace AppCore::GLEngine;
using namespace AppCore::Math;

#include "../../../AppCore/Logging/Logging.h"

using namespace AppCore::Logging;

namespace StructureSynth {
	namespace Model {	
		namespace Rendering {

			void OpenGLRenderer::drawBox(AppCore::Math::Vector3f base, 
				AppCore::Math::Vector3f dir1 , 
				AppCore::Math::Vector3f dir2, 
				AppCore::Math::Vector3f dir3) {
					engine->addObject(new Box( base, dir1, dir2, dir3));
			};

			void OpenGLRenderer::drawGrid(AppCore::Math::Vector3f base, 
				AppCore::Math::Vector3f dir1 , 
				AppCore::Math::Vector3f dir2, 
				AppCore::Math::Vector3f dir3) {
					engine->addObject(new Grid( base, dir1, dir2, dir3));
			};

			void OpenGLRenderer::drawLine(AppCore::Math::Vector3f from, 
				AppCore::Math::Vector3f to) {
					engine->addObject(new Line( from, to));
			};

			void OpenGLRenderer::drawDot(AppCore::Math::Vector3f v) {
					engine->addObject(new Dot(v));
			};

			void OpenGLRenderer::drawSphere(AppCore::Math::Vector3f center, float radius) {
				Object3D* o = new Sphere( center, radius);
				o->setColor(rgb, 1);
				engine->addObject(o);
			};

			void OpenGLRenderer::begin() {
				engine->clearWorld();
				rgb = Vector3f(1,0,0);
			};

			void OpenGLRenderer::end() {
				INFO(QString("Ended rendering. Wrote %1 objects.").arg(engine->objectCount()));
				engine->requireRedraw();
			};


		}
	}
}