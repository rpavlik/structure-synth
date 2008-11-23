#include "Debug.h"

#include <QScriptEngine>
#include <QMessageBox>
#include <QApplication>
#include <QThread>
#include "SyntopiaCore/Logging/Logging.h"
#include "../../SyntopiaCore/GLEngine/Sphere.h"


using namespace SyntopiaCore::Logging;



namespace StructureSynth {
	namespace JavaScriptSupport {	

		namespace {
			// Dont know why Qt has chosen to make 'sleep' protected.
			class MyThread : public QThread {
			public:
				static void sleep(unsigned long msecs) { msleep(msecs); }
			};

			
		};

		Debug::Debug(){
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

		void World::addSphere(Vector3 center, float radius) {
			SyntopiaCore::GLEngine::Object3D* o = new SyntopiaCore::GLEngine::Sphere( center.getObj(), radius);
			//o->setColor(rgb, alpha);
			engine->addObject(o);
		}
	}
}
