#include <QtGui>
#include <QDir>

#include "MainWindow.h"
#include "SyntopiaCore/Logging/ListWidgetLogger.h"
#include "SyntopiaCore/Exceptions/Exception.h"
#include "StructureSynth/Parser/EisenParser.h"
#include "StructureSynth/Model/Rendering/OpenGLRenderer.h"
#include "StructureSynth/Parser/Tokenizer.h"
#include "StructureSynth/Parser/Preprocessor.h"
#include "StructureSynth/Model/RuleSet.h"
#include "StructureSynth/Model/Builder.h"


using namespace SyntopiaCore::Logging;
using namespace StructureSynth::Model::Rendering;
using namespace SyntopiaCore::Exceptions;
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
				static QString pattern = "(\\smd\\s|\\sw\\s|\\sweigth\\s|\\smaxdepth\\s|rule\\s|\\sx\\s|\\sy\\s|\\sz\\s|\\srx\\s|\\sry\\s|\\srz\\s|\\sbox\\s|\\ssphere\\s|\\ss\\s|set)";

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
			if (maybeSave()) {
				textEdit->clear();
				setCurrentFile("");
				// TODO: Clear 3D GUI...
			} else {
				WARNING("Unable to save file...");
			}
		}

		void MainWindow::open()
		{
			if (maybeSave()) {
				QString fileName = QFileDialog::getOpenFileName(this);
				if (!fileName.isEmpty()) {
					loadFile(fileName);
					// TODO: Clear 3D GUI...
				}
				
			} else {
				WARNING("Unable to save file...");
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
			
			QFile file(getMiscDir() + QDir::separator() + "about.html");
			if (!file.open(QFile::ReadOnly | QFile::Text)) {
				WARNING("Could not open about.html...");
				return;
			}

			QTextStream in(&file);
			QString text = in.readAll();

			text.replace("$VERSION$", version.toLongString());
			
			QMessageBox mb(this);
			mb.setText(text);
			mb.setWindowTitle("About Structure Synth");
			mb.setIconPixmap(getMiscDir() + QDir::separator() + "icon.jpg");
			mb.exec();

		}

		void MainWindow::documentWasModified()
		{
			setWindowModified(true);
		}

		void MainWindow::init()
		{
			version = SyntopiaCore::Misc::Version(0, 5, 0, -1, " Alpha (\"Graf Zeppelin\")");
			setAttribute(Qt::WA_DeleteOnClose);

			isUntitled = true;

			
			QSplitter*	splitter = new QSplitter(this);
			splitter->setObjectName(QString::fromUtf8("splitter"));
			splitter->setOrientation(Qt::Horizontal);

			textEdit = new QTextEdit(splitter);
			new EisenScriptHighlighter(textEdit);
			textEdit->resize(100,100);

			
			QString s = 
			QString("// Double spirals\r\n")+
			"set maxdepth 400\r\n"+
			"R1\r\n"+
			"R2\r\n"+
			"\r\n"+
			"rule R1 { \r\n"+
			"{ x 1 rz 6 ry 6 s 0.99 } R1\r\n"+
			"{ s 2 } sphere\r\n"+
			"} \r\n"+
			"\r\n"+
			"rule R2 { \r\n"+
			"{ x -1  rz 6 ry 6 s 0.99 } R2\r\n"+
			"{ s 2 } sphere \r\n"+
			"} \r\n";
			textEdit->setText(s);

			
			engine = new SyntopiaCore::GLEngine::EngineWidget(splitter);
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
			INFO("Welcome to Structure Synth. A Syntopia Project.");
			INFO("Hold 'CTRL' for speed draw'.");
			INFO("Press 'Panic' if the view disappears...");

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

			panicAction = new QAction("Panic!", this);
			panicAction->setStatusTip(tr("Resets the viewport"));
			connect(panicAction, SIGNAL(triggered()), this, SLOT(resetView()));


			aboutAction = new QAction(tr("&About"), this);
			aboutAction->setStatusTip(tr("Show the About box"));
			connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

			

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

			// Examples...
			QMenu* examplesMenu = menuBar()->addMenu(tr("&Examples"));

			// Scan examples dir...
			QDir d(getExamplesDir());
			QStringList filters;
			filters << "*.es";
			d.setNameFilters(filters);
			if (!d.exists()) {
				QAction* a = new QAction("Unable to locate: "+d.absolutePath(), this);
				a->setEnabled(false);
				examplesMenu->addAction(a);
			} else {
				QStringList sl = d.entryList();
				for (int i = 0; i < sl.size(); i++) {
					QAction* a = new QAction(sl[i], this);
					a->setData(sl[i]);
					connect(a, SIGNAL(triggered()), this, SLOT(openFile()));
					examplesMenu->addAction(a);
				}
			}
			
			helpMenu = menuBar()->addMenu(tr("&Help"));
			helpMenu->addAction(aboutAction);
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
			renderToolBar->addAction(panicAction);
			
		}

		void MainWindow::createStatusBar()
		{
			statusBar()->showMessage(tr("Ready"));
		}

		void MainWindow::readSettings()
		{
			QSettings settings("hvidtfeldts.net", "Structure Synth");
			QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
			QSize size = settings.value("size", QSize(400, 400)).toSize();
			move(pos);
			resize(size);
		}

		void MainWindow::writeSettings()
		{
			QSettings settings("hvidtfeldts.net", "Structure Synth");
			settings.setValue("pos", pos());
			settings.setValue("size", size());
		}

		bool MainWindow::maybeSave()
		{
			if (textEdit->document()->isModified()) {
				QMessageBox::StandardButton ret;
				ret = QMessageBox::warning(this, tr("Structure Synth"),
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

		void MainWindow::openFile()
		{
			QAction *action = qobject_cast<QAction *>(sender());
			if (action) {
				if (maybeSave()) {
					QDir d(getExamplesDir());
					loadFile(d.absoluteFilePath(action->data().toString()));
				} else {
					WARNING("Unable to save...");
				}
			} else {
				WARNING("No data!");
			}
		}

		void MainWindow::loadFile(const QString &fileName)
		{
			INFO("LoadFile");
			QFile file(fileName);
			if (!file.open(QFile::ReadOnly | QFile::Text)) {
				QMessageBox::warning(this, tr("Structure Synth"),
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
				QMessageBox::warning(this, tr("Structure Synth"),
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
				.arg(tr("Structure Synth")));
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
				
				Tokenizer tokenizer(Preprocessor::Process(textEdit->toPlainText()));
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

				//delete(rs);

			} catch (Exception& er) {
				WARNING(er.getMessage());
			} 
		}

		QString MainWindow::getExamplesDir() {
			// TODO: Implement
			return "Examples";
		}

		QString MainWindow::getMiscDir() {
			// TODO: Implement
			return "Misc";
		}

		void MainWindow::resetView() {
			engine->reset();
		}
			
	}
}