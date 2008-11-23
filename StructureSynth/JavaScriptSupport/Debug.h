#pragma once

#include <QString>
#include <QObject>
#include <QProgressDialog>
#include <QScriptable>
#include "../../SyntopiaCore/Math/Vector3.h"
#include "../../SyntopiaCore/GLEngine/EngineWidget.h"

namespace StructureSynth {
	namespace JavaScriptSupport {	

		/// Write info to console (available as a Global object field in the JavaScript environment).
		class Debug : public QObject {
			Q_OBJECT

		public:
			Debug();
			~Debug();
			
		public slots:
			void Info(QString input);
			void Message(QString input);
			void ShowProgress(QString caption);
			void SetProgress(double percentage); // between 0 and 1
			void HideProgress();
			void Sleep(int ms);

		private:
			QProgressDialog* progress;
		};

		

		
		


		// Wrapper for the 3D vector object.
		class Vector3 : public QObject, protected QScriptable {
			Q_OBJECT

			Q_PROPERTY(float x READ readX WRITE writeX)
			Q_PROPERTY(float y READ readY WRITE writeY)
			Q_PROPERTY(float z READ readZ WRITE writeZ)
     
		public:
			Vector3(){};
			Vector3(float x, float y, float z){ v = SyntopiaCore::Math::Vector3f(x,y,z); };
			Vector3(const StructureSynth::JavaScriptSupport::Vector3 & vx) : QObject() {
				v = vx.v;
			}
			
			float readX() const { return v.x(); }
			float readY() const { return v.y(); }
			float readZ() const { return v.z(); }
			void writeX(float v) { this->v.x() = v; }
			void writeY(float v) { this->v.y() = v; }
			void writeZ(float v) { this->v.z() = v; }

			SyntopiaCore::Math::Vector3f getObj() { return v; }
		
		public slots:
			QString toString() const { return QString("(%1,%2,%3)").arg(v.x()).arg(v.y()).arg(v.z()); };
			float length() const { return v.length(); };

		private:
			SyntopiaCore::Math::Vector3f v;
		};

		
		class World : public QObject {
			Q_OBJECT

		public:
			World(SyntopiaCore::GLEngine::EngineWidget* engine) : engine(engine) {};
			
		public slots:
			void addSphere(Vector3 center, float radius);
			
		private:
			QProgressDialog* progress;
			SyntopiaCore::GLEngine::EngineWidget* engine;
		};
		
	}
}






