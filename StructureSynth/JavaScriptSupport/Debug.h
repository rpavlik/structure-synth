#pragma once

#include <QString>
#include <QObject>
#include <QProgressDialog>

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

	}
}

