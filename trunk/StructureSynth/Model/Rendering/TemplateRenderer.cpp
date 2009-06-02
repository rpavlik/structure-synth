#include "TemplateRenderer.h"
#include "../../../SyntopiaCore/Math/Vector3.h"
#include "../../../SyntopiaCore/Logging/Logging.h"
#include "../../../SyntopiaCore/Exceptions/Exception.h"

#include <QDomDocument>
#include <QIODevice>
#include <QFile>
#include <QFileInfo>
#include <QMap>

using namespace SyntopiaCore::Math;
using namespace SyntopiaCore::Logging;

namespace StructureSynth {
	namespace Model {	
		namespace Rendering {


			void Template::read(QString xml) {
				QDomDocument doc;
				
				if (!doc.setContent(xml)) {
					WARNING("Unable to parse xml.");
					return;
				}

				fullText = doc.toString();

				parse(doc);
			}

			void Template::read(QFile& file) {
				QDomDocument doc;
				if (!file.open(QIODevice::ReadOnly)) {
					WARNING("Unable to open file: " + QFileInfo(file).absoluteFilePath());
					return;
				}
				if (!doc.setContent(&file)) {
					WARNING("Unable to parse file: " + QFileInfo(file).absoluteFilePath());
					file.close();
					return;
				}
				file.close();

				fullText = doc.toString();
				
				parse(doc);
			}

			void Template::parse(QDomDocument& doc) {
				QDomElement docElem = doc.documentElement();

				QDomNode n = docElem.firstChild();
				while(!n.isNull()) {
					QDomElement e = n.toElement(); // try to convert the node to an element.
					if(!e.isNull()) {
						if (e.tagName() == "substitution") {

							if (!e.hasAttribute("name")) {
								WARNING("Substitution without name attribute found!");
								continue;
							}


							QString type = "";
							if (e.hasAttribute("type")) {
								type = "::" + e.attribute("type");
							}


							QString name = e.attribute("name") + type;
							//INFO(QString("%1 = %2").arg(name).arg(e.text()));
							primitives[name] = TemplatePrimitive(e.text());
						} else if (e.tagName() == "description") {
							description = e.text();
						} else {

							WARNING("Expected 'substitution' or 'description' element, found: " + e.tagName());
							continue;

						}
					}
					n = n.nextSibling();
				}


			}

			TemplateRenderer::TemplateRenderer(QString xmlDefinitionFile) {
				counter = 0;
				QFile file(xmlDefinitionFile);

				workingTemplate.read(file);
			}


			TemplateRenderer::TemplateRenderer()  
			{
				counter = 0;
			}

			TemplateRenderer::~TemplateRenderer() {
			}

			bool TemplateRenderer::assertPrimitiveExists(QString templateName) {
				if (!workingTemplate.getPrimitives().contains(templateName)) {
					QString error = 
						QString("Template error: the primitive '%1' is not defined.").arg(templateName);

					if (!missingTypes.contains(error)) {
						// Only show each error once.
						WARNING(error);
						INFO("(A template may not support all drawing primitives. Either update the template or choose another primitive)");
						missingTypes.insert(error);
					} 
					return false;
				}
				return true;

			} 

			void TemplateRenderer::doStandardSubstitutions(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				SyntopiaCore::Math::Vector3f dir2, 
				SyntopiaCore::Math::Vector3f dir3, TemplatePrimitive& t) {
					if (t.contains("{matrix}")) {
						QString mat = QString("%1 %2 %3 0 %4 %5 %6 0 %7 %8 %9 0 %10 %11 %12 1")
							.arg(dir1.x()).arg(dir1.y()).arg(dir1.z())
							.arg(dir2.x()).arg(dir2.y()).arg(dir2.z())
							.arg(dir3.x()).arg(dir3.y()).arg(dir3.z())
							.arg(base.x()).arg(base.y()).arg(base.z());

						t.substitute("{matrix}", mat);


					}

					if (t.contains("{povmatrix}")) {
						QString mat = QString("%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12")
							.arg(dir1.x()).arg(dir1.y()).arg(dir1.z())
							.arg(dir2.x()).arg(dir2.y()).arg(dir2.z())
							.arg(dir3.x()).arg(dir3.y()).arg(dir3.z())
							.arg(base.x()).arg(base.y()).arg(base.z());

						t.substitute("{povmatrix}", mat);
					}

					t.substitute("{r}", QString::number(rgb.x()));
					t.substitute("{g}", QString::number(rgb.y()));
					t.substitute("{b}", QString::number(rgb.z()));
					t.substitute("{alpha}", QString::number(alpha));
					t.substitute("{oneminusalpha}", QString::number(1-alpha));

			}

			void TemplateRenderer::drawBox(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1 , 
				SyntopiaCore::Math::Vector3f dir2, 
				SyntopiaCore::Math::Vector3f dir3,
				const QString& classID) 
			{
				QString alternateID = (classID.isEmpty() ? "" : "::" + classID);
				if (!assertPrimitiveExists("box"+alternateID)) return;
				TemplatePrimitive t(workingTemplate.get("box"+alternateID)); 

				doStandardSubstitutions(base, dir1, dir2, dir3, t);

				if (t.contains("{uid}")) {
					t.substitute("{uid}", QString("Box%1").arg(counter++));
				}

				output.append(t.getText());
			};

			void TemplateRenderer::drawTriangle(SyntopiaCore::Math::Vector3f p1,
				SyntopiaCore::Math::Vector3f p2,
				SyntopiaCore::Math::Vector3f p3,
				const QString& classID) {

					QString alternateID = (classID.isEmpty() ? "" : "::" + classID);
					if (!assertPrimitiveExists("triangle"+alternateID)) return;
					TemplatePrimitive t(workingTemplate.get("triangle"+alternateID)); 

					if (t.contains("{uid}")) {
						t.substitute("{uid}", QString("Triangle%1").arg(counter++));
					}

					t.substitute("{p1x}", QString::number(p1.x()));
					t.substitute("{p1y}", QString::number(p1.y()));
					t.substitute("{p1z}", QString::number(p1.z()));
					t.substitute("{p2x}", QString::number(p2.x()));
					t.substitute("{p2y}", QString::number(p2.y()));
					t.substitute("{p2z}", QString::number(p2.z()));
					t.substitute("{p3x}", QString::number(p3.x()));
					t.substitute("{p3y}", QString::number(p3.y()));
					t.substitute("{p3z}", QString::number(p3.z()));

					t.substitute("{alpha}", QString::number(alpha));
					t.substitute("{oneminusalpha}", QString::number(1-alpha));


					output.append(t.getText());

			}


			void TemplateRenderer::drawGrid(SyntopiaCore::Math::Vector3f base, 
				SyntopiaCore::Math::Vector3f dir1, 
				SyntopiaCore::Math::Vector3f dir2, 
				SyntopiaCore::Math::Vector3f dir3,
				const QString& classID) {

					QString alternateID = (classID.isEmpty() ? "" : "::" + classID);
					if (!assertPrimitiveExists("grid"+alternateID)) return;
					TemplatePrimitive t(workingTemplate.get("grid"+alternateID)); 


					doStandardSubstitutions(base, dir1, dir2, dir3, t);

					if (t.contains("{uid}")) {
						t.substitute("{uid}", QString("Grid%1").arg(counter++));
					}



					output.append(t.getText());
			};

			void TemplateRenderer::drawLine(SyntopiaCore::Math::Vector3f from, SyntopiaCore::Math::Vector3f to,const QString& classID) {
				QString alternateID = (classID.isEmpty() ? "" : "::" + classID);
				if (!assertPrimitiveExists("line"+alternateID)) return;
				TemplatePrimitive t(workingTemplate.get("line"+alternateID)); 
				t.substitute("{x1}", QString::number(from.x()));
				t.substitute("{y1}", QString::number(from.y()));
				t.substitute("{z1}", QString::number(from.z()));

				t.substitute("{x2}", QString::number(to.x()));
				t.substitute("{y2}", QString::number(to.y()));
				t.substitute("{z2}", QString::number(to.z()));

				t.substitute("{alpha}", QString::number(alpha));
				t.substitute("{oneminusalpha}", QString::number(1-alpha));

				if (t.contains("{uid}")) {
					t.substitute("{uid}", QString("Line%1").arg(counter++));
				}

				output.append(t.getText());
			};

			void TemplateRenderer::drawDot(SyntopiaCore::Math::Vector3f v,const QString& classID) {
				QString alternateID = (classID.isEmpty() ? "" : "::" + classID);
				if (!assertPrimitiveExists("dot"+alternateID)) return;
				TemplatePrimitive t(workingTemplate.get("dot"+alternateID)); 
				t.substitute("{x}", QString::number(v.x()));
				t.substitute("{y}", QString::number(v.y()));
				t.substitute("{z}", QString::number(v.z()));

				t.substitute("{r}", QString::number(rgb.x()));
				t.substitute("{g}", QString::number(rgb.y()));
				t.substitute("{b}", QString::number(rgb.z()));

				t.substitute("{alpha}", QString::number(alpha));
				t.substitute("{oneminusalpha}", QString::number(1-alpha));

				if (t.contains("{uid}")) {
					t.substitute("{uid}", QString("Dot%1").arg(counter++));
				}

				output.append(t.getText());
			};

			void TemplateRenderer::drawSphere(SyntopiaCore::Math::Vector3f center, float radius,const QString& classID) {
				QString alternateID = (classID.isEmpty() ? "" : "::" + classID);
				if (!assertPrimitiveExists("sphere"+alternateID)) return;
				TemplatePrimitive t(workingTemplate.get("sphere"+alternateID)); 
				t.substitute("{cx}", QString::number(center.x()));
				t.substitute("{cy}", QString::number(center.y()));
				t.substitute("{cz}", QString::number(center.z()));

				t.substitute("{rad}", QString::number(radius));

				t.substitute("{r}", QString::number(rgb.x()));
				t.substitute("{g}", QString::number(rgb.y()));
				t.substitute("{b}", QString::number(rgb.z()));

				t.substitute("{alpha}", QString::number(alpha));
				t.substitute("{oneminusalpha}", QString::number(1-alpha));

				if (t.contains("{uid}")) {
					t.substitute("{uid}", QString("Sphere%1").arg(counter++));
				}

				output.append(t.getText());
			};

			void TemplateRenderer::begin() {
				if (!assertPrimitiveExists("begin")) return;
				TemplatePrimitive t(workingTemplate.get("begin")); 

				t.substitute("{CamPosX}", QString::number(cameraPosition.x()));
				t.substitute("{CamPosY}", QString::number(cameraPosition.y()));
				t.substitute("{CamPosZ}", QString::number(cameraPosition.z()));
				t.substitute("{CamUpX}", QString::number(cameraUp.x()));
				t.substitute("{CamUpY}", QString::number(cameraUp.y()));
				t.substitute("{CamUpZ}", QString::number(cameraUp.z()));
				Vector3f cameraDir = cameraTarget-cameraPosition;
				cameraDir = cameraDir.normalize();
				t.substitute("{CamDirX}", QString::number(cameraDir.x()));
				t.substitute("{CamDirY}", QString::number(cameraDir.y()));
				t.substitute("{CamDirZ}", QString::number(cameraDir.z()));
				t.substitute("{CamRightX}", QString::number(cameraRight.x()));
				t.substitute("{CamRightY}", QString::number(cameraRight.y()));
				t.substitute("{CamRightZ}", QString::number(cameraRight.z()));
				t.substitute("{CamTargetX}", QString::number(cameraTarget.x()));
				t.substitute("{CamTargetY}", QString::number(cameraTarget.y()));
				t.substitute("{CamTargetZ}", QString::number(cameraTarget.z()));

				t.substitute("{aspect}", QString::number(aspect));	
				t.substitute("{width}", QString::number(width));
				t.substitute("{height}", QString::number(height));
				t.substitute("{fov}", QString::number(fov));

				t.substitute("{BR}", QString::number(backRgb.x()));
				t.substitute("{BG}", QString::number(backRgb.y()));
				t.substitute("{BB}", QString::number(backRgb.z()));

				t.substitute("{BR256}", QString::number(backRgb.x()*255));
				t.substitute("{BG256}", QString::number(backRgb.y()*255));
				t.substitute("{BB256}", QString::number(backRgb.z()*255));



				output.append(t.getText());
			};

			void TemplateRenderer::end() {
				if (!assertPrimitiveExists("end")) return;
				TemplatePrimitive t(workingTemplate.get("end")); 
				output.append(t.getText());
			};

			void TemplateRenderer::setBackgroundColor(SyntopiaCore::Math::Vector3f rgb) {
				backRgb = rgb;
			}

			void TemplateRenderer::drawMesh(  SyntopiaCore::Math::Vector3f /*startBase*/, 
				SyntopiaCore::Math::Vector3f /*startDir1*/, 
				SyntopiaCore::Math::Vector3f /*startDir2*/, 
				SyntopiaCore::Math::Vector3f /*endBase*/, 
				SyntopiaCore::Math::Vector3f /*endDir1*/, 
				SyntopiaCore::Math::Vector3f /*endDir2*/, 
				const QString& /*classID*/) {
			};

			void TemplateRenderer::callCommand(const QString& renderClass, const QString& /*command*/) {
				if (renderClass != this->renderClass()) return;

			}

			QString TemplateRenderer::getOutput() { 
				QString out = output.join(""); 

				// Normalize output (seems the '\n' converts to CR+LF on windows while saving
				// whereas '\r\n' converts to CR+CR+LF? so we remove the \r's).
				out = out.replace("\r","");
				return out;
			}


		}
	}
}

