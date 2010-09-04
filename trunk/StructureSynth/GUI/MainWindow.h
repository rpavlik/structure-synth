#pragma once

#include <QMainWindow>
#include <QTabBar>
#include <QStackedWidget>


#include "../../SyntopiaCore/GLEngine/EngineWidget.h"
#include "../../SyntopiaCore/Misc/Version.h"
#include "VariableEditor.h"
#include "TemplateExportDialog.h"
#include "../Model/Rendering/TemplateRenderer.h"
#include "../Model/Builder.h"



class QAction;
class QMenu;
class QTextEdit;

namespace StructureSynth {
	namespace GUI {
 
		struct TabInfo {
			TabInfo() {}; 
			TabInfo(QString filename, QTextEdit* textEdit) : filename(filename), unsaved(false), textEdit(textEdit), hasBeenSavedOnce(false) {};
			TabInfo(QString filename, QTextEdit* textEdit, bool unsaved, bool hasBeenSavedOnce=false) : filename(filename), unsaved(unsaved), textEdit(textEdit), hasBeenSavedOnce(hasBeenSavedOnce) {};
			QString filename;
			bool unsaved;
			QTextEdit* textEdit;
			bool hasBeenSavedOnce;			
        };

		// A modified QTextEdit with an extended context menu
		class TextEdit : public QTextEdit {
			Q_OBJECT
			public:
				TextEdit() : QTextEdit() {};
				TextEdit(QWidget* parent) : QTextEdit(parent) {};

				void contextMenuEvent(QContextMenuEvent *event);
			public slots:
				void insertText();
		};
		


		/// The main window of the application.
		class MainWindow : public QMainWindow
		{
			Q_OBJECT

		public:
			MainWindow();
			MainWindow(const QString &fileName);
			void setSeed(int randomSeed);
			int getSeed();
			void templateRender(const QString& fileName, Model::Rendering::Template* myTemplate, QString inputText, int width = 0, int height = 0, bool postModify = false);
		

			SyntopiaCore::GLEngine::EngineWidget* getEngine() { return engine; };
			static QString getExamplesDir();
			static QString getMiscDir();
			static QString getTemplateDir();
			void saveImage(QImage im);
			
			QString getCameraSettings();
			QString getScriptWithSettings(QString filename);
			
		protected:
			void dragEnterEvent(QDragEnterEvent *ev);
			void dropEvent(QDropEvent *ev);
			void closeEvent(QCloseEvent* ev);
			void keyReleaseEvent(QKeyEvent* ev);

		private slots:
			void templateExport();
			
			void closeTab(int id);
			void cut();
			void copy();
			void paste();
			void insertCameraSettings();
			void cursorPositionChanged();
			void seedChanged();
			void tabChanged(int index);
			void makeScreenshot();
			void closeTab();
			void launchSfHome();
			void fastRotateChanged();
			void launchGallery();
			void launchReferenceHome();
			void templateRender();
			void templateRenderToFile();
			void templateRender(const QString& fileName);
			
			void openFile();
			void raytrace();
			void newFile();
			void open();
			bool save();
			bool saveAs();
			void about();
			void documentWasModified();
			void render();
			void resetView();
			void toggleFullScreen();
			

		private:
			void setRecentFile(const QString &fileName);
			void parseEaster(QString text);
			void parseJavaScript(QString scripture);
			void insertTabPage(QString filename);
			QTextEdit* getTextEdit();
			void init();
			void createActions();
			void createMenus();
			void createToolBars();
			void createStatusBar();
			void readSettings();
			void writeSettings();
			void updateRandom();
			void loadFile(const QString &fileName);
			bool saveFile(const QString &fileName);
			QString strippedName(const QString &fullFileName);
			void createOpenGLContextMenu();

			bool hasBeenResized;
			QSpinBox* seedSpinBox;
			QCheckBox* autoIncrementCheckbox;

			QDockWidget* dockLog;
			QAction* fullScreenAction;
			QAction* insertCameraSettingsAction;
			QAction* screenshotAction;
			QAction* sfHomeAction;
			QAction* referenceAction;
			QAction* galleryAction;
			QMenu *fileMenu;
			QMenu *editMenu;
			QMenu *renderMenu;
			QMenu *helpMenu;
			QToolBar *fileToolBar;
			QToolBar *renderToolBar;
			QToolBar *editToolBar;
			QToolBar *randomToolBar;
			QAction *newAction;
			QAction *rayTraceAction;
			QAction *openAction;
			QAction *saveAction;
			QAction *saveAsAction;
			QAction *closeAction;
			QAction *exitAction;
			QAction *cutAction;
			QAction *copyAction;
			QAction *pasteAction;
			QAction *aboutAction;

			QAction *renderAction;
			QAction *exportAction;
			QAction *panicAction;
			SyntopiaCore::GLEngine::EngineWidget* engine;
			QTabBar* tabBar;

			SyntopiaCore::Misc::Version version;

			QMenu* openGLContextMenu;
			bool fullScreenEnabled;
			QStackedWidget *stackedTextEdits;

			QVector<TabInfo> tabInfo;

			int oldDirtyPosition;

			QVBoxLayout* frameMainWindow;
			QCheckBox* fastRotateCheckbox;
			VariableEditor* variableEditor;
			QDockWidget* editorDockWidget;

			QVector<QAction*> recentFileActions;
			QAction* recentFileSeparator;
			
		};

		class PreviewWindow : public QDialog {
			Q_OBJECT
		public:
			PreviewWindow(MainWindow* parent, QImage im) ;
		public slots:
			void saveImage();
		private: 
			MainWindow* mainWindow;
			QImage image;
		};

	}
}

