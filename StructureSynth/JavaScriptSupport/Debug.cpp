#if defined(_MSC_VER) 
// disable warning "'QtConcurrent::BlockSizeManager' : assignment operator could not be generated"
#pragma warning( disable : 4512 )
#endif

#include "Debug.h"

#include <QScriptEngine>
#include <QMessageBox>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include "SyntopiaCore/Logging/Logging.h"
#include "../../SyntopiaCore/GLEngine/Sphere.h"
#include "../../SyntopiaCore/GLEngine/RayTracer.h"
#include "../../StructureSynth/Model/Rendering/OpenGLRenderer.h"
#include "../../StructureSynth/Parser/Tokenizer.h"
#include "../../StructureSynth/Parser/Preprocessor.h"
#include "../../StructureSynth/Model/RuleSet.h"
#include "../../StructureSynth/Model/Builder.h"
#include "../../StructureSynth/Parser/EisenParser.h"
#include "../../SyntopiaCore/Exceptions/Exception.h"

using namespace SyntopiaCore::Logging;
using namespace SyntopiaCore::GLEngine;
using namespace StructureSynth::Model;
using namespace StructureSynth::Parser;
using namespace SyntopiaCore::Exceptions;
using namespace StructureSynth::Model::Rendering;


namespace StructureSynth {
	namespace JavaScriptSupport {	

		namespace {
			// Dont know why Qt has chosen to make 'sleep' protected.
			class MyThread : public QThread {
			public:
				static void sleep(unsigned long msecs) { msleep(msecs); }
			};


		};

		Debug::Debug(QStatusBar* statusBar) : statusBar(statusBar) {
			progress = 0;
		}

		Debug::~Debug() {
		}

		void Debug::Info(QString input) {
			INFO(input);
		}

		void Debug::Message(QString input) {
			QMessageBox::information(0, "JavaScript Message", input);
		}

		void Debug::ShowProgress(QString caption) {
			delete(progress); 
			progress = new QProgressDialog(caption, "Cancel", 0, 1000, 0);
			progress->setWindowModality(Qt::WindowModal);
			progress->show();

		}

		void Debug::SetProgress(double percentage) {
			if (progress) progress->setValue(percentage*1000.0);
			qApp->processEvents();
		}

		void Debug::HideProgress() {
			delete(progress);
			progress = 0;
		}

		void Debug::Sleep(int ms) {
			MyThread::sleep(ms);
		}

		void World::addSphere2(Vector3 center, float radius) {
			SyntopiaCore::GLEngine::Object3D* o = new SyntopiaCore::GLEngine::Sphere( center.getObj(), radius);
			o->setColor(this->rgb, this->alpha);
			engine->addObject(o);
		}

		void World::setColor2(Vector3 rgb, float alpha) {
			this->rgb = rgb.getObj();
			this->alpha = alpha;
		}

		Vector3::Vector3(){ };

		Vector3::Vector3(float x, float y, float z){ 
			//INFO(QString("Vector3(%1,%2,%3)").arg(x).arg(y).arg(z));
			v = SyntopiaCore::Math::Vector3f(x,y,z); 
		};

		Vector3::Vector3(const StructureSynth::JavaScriptSupport::Vector3 & vx) : QObject(), QScriptable() {
			v = vx.v;
			//INFO(QString("Vector3 CopyConstructor(%1,%2,%3)").arg(v.x()).arg(v.y()).arg(v.z()));

		}

		void Builder::render() {

			engine3D->setDisabled(true);
		
			try {
				Rendering::OpenGLRenderer renderTarget(engine3D);
				renderTarget.begin(); // we clear before parsing...

				Preprocessor pp;
				QString out = pp.Process(loadedSystem);
				Tokenizer tokenizer(out);
				EisenParser e(&tokenizer);
				RuleSet* rs = e.parseRuleset();
				rs->resolveNames();
				Model::Builder b(&renderTarget, rs, false);
				b.build();
				renderTarget.end();
				engine3D->setRaytracerCommands(b.getRaytracerCommands());
				//INFO(QString("Setting %1 raytracer commands.").arg(raytracerCommands.count()));
				delete(rs);
				rs = 0;
			} catch (ParseError& pe) {
				WARNING(pe.getMessage());
			} catch (Exception& er) {
				WARNING(er.getMessage());
			} 
			engine3D->setDisabled(false);
			engine3D->requireRedraw();

		}

		void Builder::load(QString fileName) {	
			fileName = QDir("").absoluteFilePath(fileName);
			QFile file(fileName);
			if (!file.open(QFile::ReadOnly | QFile::Text)) {
				WARNING(QString("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
			} else {
				QTextStream in(&file);
				loadedSystem = in.readAll();
				originalSystem = loadedSystem;
			}
		};
		void Builder::define(QString input, QString value) {
			QStringList s = loadedSystem.split("\n");
			for (int i = 0; i < s.count(); i++) {

				if (!s[i].contains("#define", Qt::CaseInsensitive)) {
					//INFO("N: " + s[i]);
					s[i] = s[i].replace(input, value);
				} else {
					//INFO("X: " + s[i]);
				}
			}
			loadedSystem = s.join("\n");
			
		};
		
		void Builder::renderToFile(QString fileName, bool overwrite) {
			render();
			engine3D->requireRedraw();
			engine3D->update();
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
			engine3D->update();
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
			//engine3D->updateGL();

			QImage image = engine3D->grabFrameBuffer();

			QFileInfo fi(fileName);
			if (fi.exists()) {
				if (!overwrite) {
					if (QMessageBox::question(0, "File Error", "Overwrite file: " + fi.absoluteFilePath() + "?", 
						QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Cancel) {
							WARNING("Cancelled save.");
							return;
							
					}
				}
				INFO("Overwriting: " + fi.absoluteFilePath());		
			}
			bool succes = image.save(fileName);
			if (succes) {
				INFO("Saved: " + fi.absoluteFilePath());		
			} else {
				WARNING("Failed to save: " + fi.absoluteFilePath());
			}
		};
		
		void Builder::reset() { loadedSystem = originalSystem; };
		
		void Builder::raytraceToFile(QString fileName, bool overwrite) {
			raytraceToFile(fileName, 0,0, overwrite);
		}

		void Builder::raytraceToFile(QString fileName, int w, int h, bool overwrite) {
			RayTracer rt(engine3D);
			if (w == 0) w = engine3D->width();
			if (h == 0) h = engine3D->height();
			INFO(QString("Raytracing %1x%2 image...").arg(w).arg(h));
			QImage im = rt.calculateImage(w,h);
			QFileInfo fi(fileName);
			if (fi.exists()) {
				if (!overwrite) {
					if (QMessageBox::question(0, "File Error", "Overwrite file: " + fi.absoluteFilePath() + "?", 
						QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Cancel) {
						
						WARNING("Cancelled save.");
							return;
					}
				}
				INFO("Overwriting: " + fi.absoluteFilePath());		
			}
			bool succes = im.save(fileName);
			if (succes) {
				INFO("Saved: " + fi.absoluteFilePath());		
			} else {
				WARNING("Failed to save: " + fi.absoluteFilePath());
			}

		}
			

		void Debug::waitForMouseButton() {
			while (true) {

				statusBar->showMessage("Left Mousebutton to continue, right to quit.", 4000);
				if (QApplication::mouseButtons() == Qt::LeftButton) {
					//statusBar->showMessage("");
				
					break;
				} else if (QApplication::mouseButtons() == Qt::RightButton) {
					//statusBar->showMessage("");
					throw Exception("");
					break;
				}
				qApp->processEvents();
				//QThread::msleep(100);

			}
		}
	}
}
