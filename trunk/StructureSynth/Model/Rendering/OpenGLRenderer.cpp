#include "OpenGLRenderer.h"
#include "../../../AppCore/GLEngine/Sphere.h"
#include "../../../AppCore/GLEngine/Box.h"
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

			void OpenGLRenderer::drawSphere(AppCore::Math::Vector3f center, float radius) {
				engine->addObject(new Sphere( center, radius));
			};

			void OpenGLRenderer::begin() {
				engine->clearWorld();
			};

			void OpenGLRenderer::end() {
				INFO(QString("Ended rendering. Wrote %1 objects.").arg(engine->objectCount()));
				engine->requireRedraw();
			};


		}
	}
}