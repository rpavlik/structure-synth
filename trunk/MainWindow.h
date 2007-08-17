#pragma once

#include <QMainWindow>

#include "AppCore/GLEngine/EngineWidget.h"


class QAction;
class QMenu;
class QTextEdit;

namespace StructureSynth {
	namespace GUI {

		/// The main window of the application.
		/// As of now a SDI interface, but at some point tabs will be added.
		class MainWindow : public QMainWindow
		{
			Q_OBJECT

		public:
			MainWindow();
			MainWindow(const QString &fileName);

		protected:
			void closeEvent(QCloseEvent *event);

		private slots:
			void openFile();
			void newFile();
			void open();
			bool save();
			bool saveAs();
			void about();
			void documentWasModified();
			void render();
			

		private:
			QString getExamplesDir();
			void init();
			void createActions();
			void createMenus();
			void createToolBars();
			void createStatusBar();
			void readSettings();
			void writeSettings();
			bool maybeSave();
			void loadFile(const QString &fileName);
			bool saveFile(const QString &fileName);
			void setCurrentFile(const QString &fileName);
			QString strippedName(const QString &fullFileName);
			MainWindow *findMainWindow(const QString &fileName);

			QTextEdit *textEdit;
			QString curFile;
			bool isUntitled;

			QMenu *fileMenu;
			QMenu *editMenu;
			QMenu *renderMenu;
			QMenu *helpMenu;
			QToolBar *fileToolBar;
			QToolBar *renderToolBar;
			QToolBar *editToolBar;
			QAction *newAction;
			QAction *openAction;
			QAction *saveAction;
			QAction *saveAsAction;
			QAction *closeAction;
			QAction *exitAction;
			QAction *cutAction;
			QAction *copyAction;
			QAction *pasteAction;
			QAction *aboutAction;
			QAction *aboutQtAction;

			QAction *renderAction;
			AppCore::GLEngine::EngineWidget* engine;
		};

	}
}