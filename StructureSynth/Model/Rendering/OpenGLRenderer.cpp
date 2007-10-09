#include "OpenGLRenderer.h"
#include "../../../SyntopiaCore/GLEngine/Sphere.h"
#include "../../../SyntopiaCore/GLEngine/Box.h"
#include "../../../SyntopiaCore/GLEngine/Grid.h"
#include "../../../SyntopiaCore/GLEngine/Dot.h"
#include "../../../SyntopiaCore/GLEngine/Line.h"
#include "../../../SyntopiaCore/Math/Vector3.h"

using namespace SyntopiaCore::GLEngine;
using namespace SyntopiaCore::Math;

#include "../../../SyntopiaCore/Logging/Logging.h"

using namespace SyntopiaCore::Logging;

namespace StructureSynth {
	namespace Model {	
		namespace Rendering {

			void OpenGLRenderer::drawBox(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				SyntopiaCore::Math::Vector3f dir2, 
				SyntopiaCore::Math::Vector3f dir3) {
					Object3D* o =new Box( base, dir1, dir2, dir3);
					o->setColor(rgb, alpha);
				
					engine->addObject(o);
			};

			void OpenGLRenderer::drawGrid(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				SyntopiaCore::Math::Vector3f dir2, 
				SyntopiaCore::Math::Vector3f dir3) {
					Object3D* o = new Grid( base, dir1, dir2, dir3);
					o->setColor(rgb, alpha);
					engine->addObject(o);
			};

			void OpenGLRenderer::drawLine(SyntopiaCore::Math::Vector3f from, SyntopiaCore::Math::Vector3f to) {
					Object3D* o = new Line( from, to);
					o->setColor(rgb, alpha);
					engine->addObject(o);
			};

			void OpenGLRenderer::drawDot(SyntopiaCore::Math::Vector3f v) {
					Object3D* o = new Dot(v);
					o->setColor(rgb, alpha);
					engine->addObject(o);	
			};

			void OpenGLRenderer::drawSphere(SyntopiaCore::Math::Vector3f center, float radius) {
				Object3D* o = new Sphere( center, radius);
				o->setColor(rgb, alpha);
				engine->addObject(o);
			};

			void OpenGLRenderer::begin() {
				engine->clearWorld();
				engine->setBackgroundColor(0,0,0);
				rgb = Vector3f(1,0,0);
				alpha = 1;
			};

			void OpenGLRenderer::end() {
				INFO(QString("Rendering done. Wrote %1 objects.").arg(engine->objectCount()));
				engine->requireRedraw();
			};

			void OpenGLRenderer::setBackgroundColor(SyntopiaCore::Math::Vector3f rgb) {
				engine->setBackgroundColor(rgb.x(),rgb.y(),rgb.z());
			}

		}
	}
}