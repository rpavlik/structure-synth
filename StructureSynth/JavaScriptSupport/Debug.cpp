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
	}
}
