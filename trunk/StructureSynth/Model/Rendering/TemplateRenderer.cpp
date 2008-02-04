#include "TemplateRenderer.h"
#include "../../../SyntopiaCore/Math/Vector3.h"
#include "../../../SyntopiaCore/Logging/Logging.h"

#include <QDomDocument>
#include <QIODevice>
#include <QFile>
#include <QMap>

using namespace SyntopiaCore::Math;
using namespace SyntopiaCore::Logging;

namespace StructureSynth {
	namespace Model {	
		namespace Rendering {

			class Template {
			public:
				Template() {};
				Template(QString def) : def(def) {};
				Template(const Template& t) { this->def = t.def; };

				QString getText() { return def; }

				void substitute(QString before, QString after) {
					def.replace(before, after);
				};

				bool contains(QString input) {
					return def.contains(input);
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

				QMap<QString , Template> templates;
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
						INFO(QString("%1 = %2").arg(name).arg(e.text()));
						templates[name] = Template(e.text());
					}
					n = n.nextSibling();
				}

				boxTemplate = 0;
				sphereTemplate = 0;
				beginTemplate = 0;
				endTemplate = 0;

				if (!templates.contains("box")) {
					WARNING("'box' template not found.");
				} else {
					boxTemplate = new Template(templates["box"]);
				}

				if (!templates.contains("sphere")) {
					WARNING("'sphere' template not found.");
				} else {
					sphereTemplate = new Template(templates["sphere"]);
				}

				if (!templates.contains("begin")) {
					WARNING("'begin' template not found.");
				} else {
					beginTemplate = new Template(templates["begin"]);
				}

				if (!templates.contains("end")) {
					WARNING("'end' template not found.");
				} else {
					endTemplate = new Template(templates["end"]);
				}
			}


			TemplateRenderer::TemplateRenderer()  
			{

			}

			TemplateRenderer::~TemplateRenderer() {
			}

			void TemplateRenderer::drawBox(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				SyntopiaCore::Math::Vector3f dir2, 
				SyntopiaCore::Math::Vector3f dir3) 
			{
				static int counter = 0;
				Template t(*boxTemplate); 
				if (t.contains("{matrix}")) {
					QString mat = QString("%1 %2 %3 0 %4 %5 %6 0 %7 %8 %9 0 %10 %11 %12 1")
					.arg(dir1.x()).arg(dir1.y()).arg(dir1.z())
					.arg(dir2.x()).arg(dir2.y()).arg(dir2.z())
					.arg(dir3.x()).arg(dir3.y()).arg(dir3.z())
					.arg(base.x()).arg(base.y()).arg(base.z());
				
					t.substitute("{matrix}", mat);
				}
				

				if (t.contains("{uid}")) {
					t.substitute("{uid}", QString("Box%1").arg(counter++));
				}

				t.substitute("{r}", QString::number(rgb.x()));
				t.substitute("{g}", QString::number(rgb.y()));
				t.substitute("{b}", QString::number(rgb.z()));

				t.substitute("{alpha}", QString::number(alpha));
				t.substitute("{oneminusalpha}", QString::number(1-alpha));
				

				output.append(t.getText());

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
				Template t(*sphereTemplate); 
				t.substitute("{cx}", QString::number(center.x()));
				t.substitute("{cy}", QString::number(center.y()));
				t.substitute("{cz}", QString::number(center.z()));
				
				t.substitute("{rad}", QString::number(radius));

				t.substitute("{r}", QString::number(rgb.x()));
				t.substitute("{g}", QString::number(rgb.y()));
				t.substitute("{b}", QString::number(rgb.z()));

				t.substitute("{alpha}", QString::number(alpha));
				t.substitute("{oneminusalpha}", QString::number(1-alpha));
				

				output.append(t.getText());
			};

			void TemplateRenderer::begin() {
				Template t(*beginTemplate); 
				output.append(t.getText());
			};

			void TemplateRenderer::end() {
				Template t(*endTemplate); 
				output.append(t.getText());
			};

			void TemplateRenderer::setBackgroundColor(SyntopiaCore::Math::Vector3f /*rgb*/) {
				// TODO
			}

		}
	}
}

