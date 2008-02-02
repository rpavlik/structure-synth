#include "TemplateRenderer.h"
#include "../../../SyntopiaCore/Math/Vector3.h"
#include "../../../SyntopiaCore/Logging/Logging.h"

#include <QDomDocument>
#include <QIODevice>
#include <QFile>

using namespace SyntopiaCore::Math;
using namespace SyntopiaCore::Logging;

namespace StructureSynth {
	namespace Model {	
		namespace Rendering {

			class Template {
			public:
				Template(QString def) : def(def) {};
				Template(const Template& t) { this->def = t.def; };
				
				void substitute(QString before, QString after) {
					def.replace(before, after);
				};

			private:
				QString def;
			};

			TemplateRenderer::TemplateRenderer(QString xmlDefinitionFile) {
				QDomDocument doc;
				QFile file(xmlDefinitionFile);
				if (!file.open(QIODevice::ReadOnly)) {
					WARNING("Unable to open file: " + xmlDefinitionFile);
					return;
				}
				if (!doc.setContent(&file)) {
					WARNING("Unable to parsefile: " + xmlDefinitionFile);
					file.close();
					return;
				}
				file.close();

				QDomElement docElem = doc.documentElement();

				QDomNode n = docElem.firstChild();
				while(!n.isNull()) {
					QDomElement e = n.toElement(); // try to convert the node to an element.
					if(!e.isNull()) {
						if (e.tagName() != "substitution") {
							WARNING("Expected 'substitution' element, found: " + e.tagName());
							continue;
						}
						if (!e.hasAttribute("name")) {
							WARNING("Substitution without name attribute found!");
							continue;
						}

						QString name = e.attribute("name");
						QString s = e.text();
						INFO(QString("%1 = %2").arg(name).arg(e.text()));
					}
					n = n.nextSibling();
				}
			}


			TemplateRenderer::TemplateRenderer() {
				boxTemplate = new Template("Box %1 %2 %3");
			}

			TemplateRenderer::~TemplateRenderer() {
			}

			void TemplateRenderer::drawBox(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				SyntopiaCore::Math::Vector3f dir2, 
				SyntopiaCore::Math::Vector3f dir3) 
			{
				Template t(*boxTemplate); 
				t.substitute("%1", "");

			};

			void TemplateRenderer::drawGrid(SyntopiaCore::Math::Vector3f /*base*/, 
				SyntopiaCore::Math::Vector3f /*dir1*/ , 
				SyntopiaCore::Math::Vector3f /*dir2*/, 
				SyntopiaCore::Math::Vector3f /*dir3*/) {
					// TODO
			};

			void TemplateRenderer::drawLine(SyntopiaCore::Math::Vector3f /*from*/, SyntopiaCore::Math::Vector3f /* to*/) {
				// TODO
			};

			void TemplateRenderer::drawDot(SyntopiaCore::Math::Vector3f /*v*/) {
				// TODO	
			};

			void TemplateRenderer::drawSphere(SyntopiaCore::Math::Vector3f center, float radius) {
			
			};

			void TemplateRenderer::begin() {
				
			};

			void TemplateRenderer::end() {
				// TODO
			};

			void TemplateRenderer::setBackgroundColor(SyntopiaCore::Math::Vector3f /*rgb*/) {
				// TODO
			}

		}
	}
}

