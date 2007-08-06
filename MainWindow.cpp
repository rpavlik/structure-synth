#include <QtGui>

#include "mainwindow.h"
#include "AppCore/Logging/ListWidgetLogger.h"
#include "AppCore/Exceptions/Exception.h"
#include "StructureSynth/Parser/EisenParser.h"
#include "StructureSynth/Model/Rendering/OpenGLRenderer.h"
#include "StructureSynth/Parser/Tokenizer.h"
#include "StructureSynth/Model/RuleSet.h"
#include "StructureSynth/Model/Builder.h"


using namespace AppCore::Logging;
using namespace StructureSynth::Model::Rendering;
using namespace AppCore::Exceptions;
using namespace StructureSynth::Parser;
using namespace StructureSynth::Model;

namespace StructureSynth {
	namespace GUI {



		class EisenScriptHighlighter : public QSyntaxHighlighter {
		public:
	
			EisenScriptHighlighter(QTextEdit* e) : QSyntaxHighlighter(e) {};

			void highlightBlock(const QString &text)
			{
				QTextCharFormat myClassFormat;
				myClassFormat.setFontWeight(QFont::Bold);
				myClassFormat.setForeground(Qt::darkMagenta);

				// Add stuff here..
				QString pattern = "(rule\\s|x\\s|y\\s|z\\s|rx\\s|ry\\s|rz\\s|box|sphere|s\\s|set)";

				QRegExp expression(pattern);
				int index = text.indexOf(expression);
				while (index >= 0) {
					int length = expression.matchedLength();
					setFormat(index, length, myClassFormat);
					index = text.indexOf(expression, index + length);
				}
			}; 

		};

		
		MainWindow::MainWindow()
		{
			init();
			setCurrentFile("");
		}

		MainWindow::MainWindow(const QString &fileName)
		{
			init();
			loadFile(fileName);
		}

		void MainWindow::closeEvent(QCloseEvent *event)
		{
			if (maybeSave()) {
				writeSettings();
				event->accept();
			} else {
				event->ignore();
			}
		}

		void MainWindow::newFile()
		{
			MainWindow *other = new MainWindow;
			other->move(x() + 40, y() + 40);
			other->show();
		}

		void MainWindow::open()
		{
			QString fileName = QFileDialog::getOpenFileName(this);
			if (!fileName.isEmpty()) {
				MainWindow *existing = findMainWindow(fileName);
				if (existing) {
					existing->show();
					existing->raise();
					existing->activateWindow();
					return;
				}

				if (isUntitled && textEdit->document()->isEmpty()
					&& !isWindowModified()) {
						loadFile(fileName);
				} else {
					MainWindow *other = new MainWindow(fileName);
					if (other->isUntitled) {
						delete other;
						return;
					}
					other->move(x() + 40, y() + 40);
					other->show();
				}
			}
		}

		bool MainWindow::save()
		{
			if (isUntitled) {
				return saveAs();
			} else {
				return saveFile(curFile);
			}
		}

		bool MainWindow::saveAs()
		{
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
				curFile);
			if (fileName.isEmpty())
				return false;

			return saveFile(fileName);
		}

		void MainWindow::about()
		{
			QMessageBox::about(this, tr("About StructureSynth"),
				tr("<b>StructureSynth</b> by hvidtfeldts.net"));
		}

		void MainWindow::documentWasModified()
		{
			setWindowModified(true);
		}

		void MainWindow::init()
		{
			setAttribute(Qt::WA_DeleteOnClose);

			isUntitled = true;

			
			QSplitter*	splitter = new QSplitter(this);
			splitter->setObjectName(QString::fromUtf8("splitter"));
			splitter->setOrientation(Qt::Horizontal);

			textEdit = new QTextEdit(splitter);
			new EisenScriptHighlighter(textEdit);
			textEdit->resize(100,100);

			textEdit->setText("\r\n\r\n rule R1 { \r\n   { x 1 y 1 rz 23 } R2 \r\n   sphere \r\n } \r\n \r\n rule R2 { \r\n   { rx 40 y 2 } R1 \r\n   box \r\n } 		");


			engine = new AppCore::GLEngine::EngineWidget(splitter);
			setCentralWidget(splitter);

			QList<int> l; l.append(100); l.append(400);
			splitter->setSizes(l);

			createActions();
			createMenus();
			createToolBars();
			createStatusBar();

			readSettings();


			// Log widget (in dockable window)
			QDockWidget* dockLog = new QDockWidget(this);
			dockLog->setWindowTitle("Log");
			dockLog->setObjectName(QString::fromUtf8("dockWidget"));
			dockLog->setAllowedAreas(Qt::BottomDockWidgetArea);
			QWidget* dockLogContents = new QWidget(dockLog);
			dockLogContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
			QVBoxLayout* vboxLayout1 = new QVBoxLayout(dockLogContents);
			vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
			vboxLayout1->setContentsMargins(0, 0, 0, 0);
			
			/*
			QListWidget* listWidget = new QListWidget(dockLog);
			new QListWidgetItem("", listWidget);
			new QListWidgetItem("(hvidtfeldts.net 2007)", listWidget);
	
			*/
			ListWidgetLogger* logger = new ListWidgetLogger(dockLog);
			vboxLayout1->addWidget(logger->getListWidget());
			dockLog->setWidget(dockLogContents);
			addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockLog);
			INFO("Welcome to StructureSynth. A Syntopia Project.");

			connect(textEdit->document(), SIGNAL(contentsChanged()),
				this, SLOT(documentWasModified()));
		}

		void MainWindow::createActions()
		{
			newAction = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
			newAction->setShortcut(tr("Ctrl+N"));
			newAction->setStatusTip(tr("Create a new file"));
			connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

			openAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
			openAction->setShortcut(tr("Ctrl+O"));
			openAction->setStatusTip(tr("Open an existing file"));
			connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

			saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
			saveAction->setShortcut(tr("Ctrl+S"));
			saveAction->setStatusTip(tr("Save the script to disk"));
			connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

			saveAsAction = new QAction(tr("Save &As..."), this);
			saveAsAction->setStatusTip(tr("Save the script under a new name"));
			connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

			closeAction = new QAction(tr("&Close"), this);
			closeAction->setShortcut(tr("Ctrl+W"));
			closeAction->setStatusTip(tr("Close this window"));
			connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

			exitAction = new QAction(tr("E&xit"), this);
			exitAction->setShortcut(tr("Ctrl+Q"));
			exitAction->setStatusTip(tr("Exit the application"));
			connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

			cutAction = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
			cutAction->setShortcut(tr("Ctrl+X"));
			cutAction->setStatusTip(tr("Cut the current selection's contents to the "
				"clipboard"));
			connect(cutAction, SIGNAL(triggered()), textEdit, SLOT(cut()));

			copyAction = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
			copyAction->setShortcut(tr("Ctrl+C"));
			copyAction->setStatusTip(tr("Copy the current selection's contents to the "
				"clipboard"));
			connect(copyAction, SIGNAL(triggered()), textEdit, SLOT(copy()));

			pasteAction = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
			pasteAction->setShortcut(tr("Ctrl+V"));
			pasteAction->setStatusTip(tr("Paste the clipboard's contents into the current "
				"selection"));
			connect(pasteAction, SIGNAL(triggered()), textEdit, SLOT(paste()));

			renderAction = new QAction(QIcon(":/images/render.png"), tr("&Render"), this);
			renderAction->setShortcut(tr("F5"));
			renderAction->setStatusTip(tr("Render the current ruleset"));
			connect(renderAction, SIGNAL(triggered()), this, SLOT(render()));


			aboutAction = new QAction(tr("&About"), this);
			aboutAction->setStatusTip(tr("Show the About box"));
			connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

			aboutQtAction = new QAction(tr("About &Qt"), this);
			aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
			connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


			cutAction->setEnabled(false);
			copyAction->setEnabled(false);
			connect(textEdit, SIGNAL(copyAvailable(bool)),
				cutAction, SLOT(setEnabled(bool)));
			connect(textEdit, SIGNAL(copyAvailable(bool)),
				copyAction, SLOT(setEnabled(bool)));
		}

		void MainWindow::createMenus()
		{
			fileMenu = menuBar()->addMenu(tr("&File"));
			fileMenu->addAction(newAction);
			fileMenu->addAction(openAction);
			fileMenu->addAction(saveAction);
			fileMenu->addAction(saveAsAction);
			fileMenu->addSeparator();
			fileMenu->addAction(closeAction);
			fileMenu->addAction(exitAction);

			editMenu = menuBar()->addMenu(tr("&Edit"));
			editMenu->addAction(cutAction);
			editMenu->addAction(copyAction);
			editMenu->addAction(pasteAction);

			renderMenu = menuBar()->addMenu(tr("&Render"));
			renderMenu->addAction(renderAction);
			
			menuBar()->addSeparator();

			helpMenu = menuBar()->addMenu(tr("&Help"));
			helpMenu->addAction(aboutAction);
			helpMenu->addAction(aboutQtAction);
		}

		void MainWindow::createToolBars()
		{
			fileToolBar = addToolBar(tr("File"));
			fileToolBar->addAction(newAction);
			fileToolBar->addAction(openAction);
			fileToolBar->addAction(saveAction);

			editToolBar = addToolBar(tr("Edit"));
			editToolBar->addAction(cutAction);
			editToolBar->addAction(copyAction);
			editToolBar->addAction(pasteAction);

			renderToolBar = addToolBar(tr("Render"));
			renderToolBar->addAction(renderAction);
			
		}

		void MainWindow::createStatusBar()
		{
			statusBar()->showMessage(tr("Ready"));
		}

		void MainWindow::readSettings()
		{
			QSettings settings("hvidtfeldts.net", "StructureSynth");
			QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
			QSize size = settings.value("size", QSize(400, 400)).toSize();
			move(pos);
			resize(size);
		}

		void MainWindow::writeSettings()
		{
			QSettings settings("hvidtfeldts.net", "StructureSynth");
			settings.setValue("pos", pos());
			settings.setValue("size", size());
		}

		bool MainWindow::maybeSave()
		{
			if (textEdit->document()->isModified()) {
				QMessageBox::StandardButton ret;
				ret = QMessageBox::warning(this, tr("StructureSynth"),
					tr("The script has been modified.\n"
					"Do you want to save your changes?"),
					QMessageBox::Save | QMessageBox::Discard
					| QMessageBox::Cancel);
				if (ret == QMessageBox::Save)
					return save();
				else if (ret == QMessageBox::Cancel)
					return false;
			}
			return true;
		}

		void MainWindow::loadFile(const QString &fileName)
		{

			QFile file(fileName);
			if (!file.open(QFile::ReadOnly | QFile::Text)) {
				QMessageBox::warning(this, tr("StructureSynth"),
					tr("Cannot read file %1:\n%2.")
					.arg(fileName)
					.arg(file.errorString()));
				return;
			}

			QTextStream in(&file);
			QApplication::setOverrideCursor(Qt::WaitCursor);
			textEdit->setPlainText(in.readAll());
			QApplication::restoreOverrideCursor();

			setCurrentFile(fileName);
			statusBar()->showMessage(tr("File loaded"), 2000);
		}

		bool MainWindow::saveFile(const QString &fileName)
		{
			QFile file(fileName);
			if (!file.open(QFile::WriteOnly | QFile::Text)) {
				QMessageBox::warning(this, tr("StructureSynth"),
					tr("Cannot write file %1:\n%2.")
					.arg(fileName)
					.arg(file.errorString()));
				return false;
			}

			QTextStream out(&file);
			QApplication::setOverrideCursor(Qt::WaitCursor);
			out << textEdit->toPlainText();
			QApplication::restoreOverrideCursor();

			setCurrentFile(fileName);
			statusBar()->showMessage(tr("File saved"), 2000);
			return true;
		}

		void MainWindow::setCurrentFile(const QString &fileName)
		{
			static int sequenceNumber = 1;

			isUntitled = fileName.isEmpty();
			if (isUntitled) {
				curFile = tr("structure%1.es").arg(sequenceNumber++);
			} else {
				curFile = QFileInfo(fileName).canonicalFilePath();
			}

			textEdit->document()->setModified(false);
			setWindowModified(false);

			setWindowTitle(tr("%1[*] - %2").arg(strippedName(curFile))
				.arg(tr("StructureSynth")));
		}

		QString MainWindow::strippedName(const QString &fullFileName)
		{
			return QFileInfo(fullFileName).fileName();
		}

		MainWindow* MainWindow::findMainWindow(const QString &fileName)
		{
			QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

			foreach (QWidget *widget, qApp->topLevelWidgets()) {
				MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
				if (mainWin && mainWin->curFile == canonicalFilePath)
					return mainWin;
			}
			return 0;
		}

		void MainWindow::render() {
			try {
				Rendering::OpenGLRenderer renderTarget(engine);
				renderTarget.begin(); // we clear before parsing...
				
				Tokenizer tokenizer(textEdit->toPlainText());
				EisenParser e(&tokenizer);
				INFO("Parsing...");
				RuleSet* rs = e.parseRuleset();

				INFO("Resolving named references...");
				rs->resolveNames();

				rs->dumpInfo();

				INFO("Building....");
				Builder b(&renderTarget, rs);
				b.build();
				renderTarget.end();

				INFO("Done...");

				delete(rs);

			} catch (Exception& er) {
				WARNING(er.getMessage());
			} 
		}
	}
}