#include <QtGui>
#include <QDir>
#include <QClipboard>
#include <QDesktopServices>
#include <QImageWriter>
#include <QTextBlockUserData>
#include <QStack>

#include "MainWindow.h"
#include "../../SyntopiaCore/Logging/ListWidgetLogger.h"
#include "../../SyntopiaCore/Exceptions/Exception.h"
#include "../../StructureSynth/Parser/EisenParser.h"
#include "../../StructureSynth/Model/Rendering/OpenGLRenderer.h"
#include "../../StructureSynth/Model/Rendering/POVRenderer.h"
#include "../../StructureSynth/Model/Rendering/TemplateRenderer.h"
#include "../../StructureSynth/Parser/Tokenizer.h"
#include "../../StructureSynth/Parser/Preprocessor.h"
#include "../../StructureSynth/Model/RuleSet.h"
#include "../../StructureSynth/Model/Builder.h"
#include "../../StructureSynth/JavaScriptSupport/JavaScriptParser.h"

#include "../../SyntopiaCore/Math/Vector3.h"
#include "../../SyntopiaCore/Math/Matrix4.h"

using namespace SyntopiaCore::Math;
using namespace SyntopiaCore::Logging;
using namespace StructureSynth::Model::Rendering;
using namespace SyntopiaCore::Exceptions;
using namespace StructureSynth::Parser;
using namespace StructureSynth::Model;
using namespace StructureSynth::JavaScriptSupport;

namespace StructureSynth {
	namespace GUI {

		class EisenScriptHighlighter : public QSyntaxHighlighter {
		public:

			EisenScriptHighlighter(QTextEdit* e) : QSyntaxHighlighter(e) {
				keywordFormat.setFontWeight(QFont::Bold);
				keywordFormat.setForeground(Qt::darkMagenta);
				bracketFormat.setFontWeight(QFont::Bold);
				bracketFormat.setForeground(Qt::blue);
				primitiveFormat.setFontWeight(QFont::Bold);
				primitiveFormat.setForeground(Qt::darkYellow);
				commentFormat.setForeground(Qt::darkGreen);
				warningFormat.setBackground(QBrush(Qt::yellow));
				preprocessorFormat.setForeground(QBrush(Qt::blue));
				preprocessorFormat.setFontWeight(QFont::Bold);

			};

			void highlightBlock(const QString &text)
			{
				static QRegExp expression("(set|rule|a|alpha|matrix|h|hue|sat|b|brightness|v|x|y|z|rx|ry|rz|s|fx|fy|fz|maxdepth|weight|md|w)");
				static QRegExp primitives("(sphere(::\\w+)?|box(::\\w+)?|dot(::\\w+)?|line(::\\w+)?|grid(::\\w+)?)");

				if (currentBlockState() == 2) {
					setFormat(0, text.length(), warningFormat);
					setCurrentBlockState(-1);
					return;
				}


				if (previousBlockState() != 1 && currentBlockState() == 1) {
					// This line was previously a multi-line start 
					if (!text.contains("*/")) setCurrentBlockState(0);
				}

				if (previousBlockState() == 1) {
					// Part of multi-line comment. Skip the rest...
					if (!text.contains("*/")) {
						setFormat(0, text.length(), commentFormat);
						setCurrentBlockState(1);
						return;
					}
				}

				// Line parsing
				QString current;
				int startMatch = 0;
				for (int i = 0; i < text.length(); i++) {
					if ((i > 0) && text.at(i) == '*' && text.at(i-1) == '/') {
						// Multi-line comment begins
						setFormat(i-1, text.length()-i+1, commentFormat);
						setCurrentBlockState(1);
						return;
					}

					if ((i > 0) && text.at(i) == '/' && text.at(i-1) == '*') {
						// Multi-line comment ends
						setFormat(0, i, commentFormat);
						if (currentBlockState() != 0) {
							setCurrentBlockState(0);
						}
						continue;
					}

					if (text.at(0) == '#') {
						// Preprocessor format
						setFormat(0, text.length(), preprocessorFormat);
						continue;
					}

					if ((i > 0) && (i < text.length()-2) && text.at(i) == '/' && text.at(i-1) == '/') {
						// Single-line comments
						setFormat(i-1, text.length()-i+1, commentFormat);
						break;
					}

					bool delimiter = (text.at(i) == '{' || text.at(i) == '}' || text.at(i) == ' '  || (text.at(i) == '\r') || (text.at(i) == '\n'));
					bool lastChar = (i==text.length()-1);
					if (delimiter || lastChar) {
						if (lastChar && !delimiter) current += text.at(i);
						int adder = (i==text.length()-1 ? 1 : 0);
						if (expression.exactMatch(current)) setFormat(startMatch, i-startMatch+adder, keywordFormat);
						if (primitives.exactMatch(current)) setFormat(startMatch, i-startMatch+adder, primitiveFormat);
						if (text.at(i) == '{' || text.at(i) == '}') setFormat(i, 1, bracketFormat);
						startMatch = i;
						current = "";
					} else {
						current += text.at(i);
					}
				}

			}; 
		private:
			QTextCharFormat keywordFormat;
			QTextCharFormat bracketFormat;
			QTextCharFormat primitiveFormat;
			QTextCharFormat commentFormat;
			QTextCharFormat warningFormat;
			QTextCharFormat preprocessorFormat;


		};


		MainWindow::MainWindow()
		{
			init();
		}

		MainWindow::MainWindow(const QString &fileName)
		{
			init();
			loadFile(fileName);
		}

		void MainWindow::closeEvent(QCloseEvent *ev)
		{

			bool modification = false;
			for (int i = 0; i < tabInfo.size(); i++) {
				if (tabInfo[i].unsaved) modification = true;
			}
			if (modification) {
				int i = QMessageBox::warning(this, "Unsaved changed", "There are tabs with unsaved changes.\r\nContinue and loose changes?", QMessageBox::Ok, QMessageBox::Cancel);
				if (i == QMessageBox::Ok) {
					// OK
					ev->accept();
					return;
				} else {
					// Cancel
					ev->ignore();
					return;
				}
			}
			ev->accept();

			writeSettings();
		}

		void MainWindow::newFile()
		{
			insertTabPage("");
		}

		void MainWindow::open()
		{
			QString filter = "EisenScript (*.es);;All Files (*.*)";
			QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), filter);
			if (!fileName.isEmpty()) {
				loadFile(fileName);
			}
		}

		void MainWindow::keyReleaseEvent(QKeyEvent* ev) {
			if (ev->key() == Qt::Key_Escape) {
				toggleFullScreen();
			} else {
				ev->ignore();
			}
		};



		bool MainWindow::save()
		{
			int index = tabBar->currentIndex();
			if (index == -1) { WARNING("No open tab"); return false; } 
			TabInfo t = tabInfo[index];

			if (t.hasBeenSavedOnce) {
				return saveFile(t.filename);
			} else {
				return saveAs();
			}
		}

		bool MainWindow::saveAs()
		{
			int index = tabBar->currentIndex();
			if (index == -1) { WARNING("No open tab"); return false; } 
			
			TabInfo t = tabInfo[index];

			QString filter = "EisenScript (*.es); All Files (*.*)";
			
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), t.filename, filter);
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
			mb.setMinimumWidth(800);
			mb.exec();

		}

		void MainWindow::documentWasModified()
		{
			tabInfo[tabBar->currentIndex()].unsaved = true;
			tabChanged(tabBar->currentIndex());
		}

		void MainWindow::init()
		{
			
			oldDirtyPosition = -1;
			setFocusPolicy(Qt::StrongFocus);

			version = SyntopiaCore::Misc::Version(0, 8, 5, -1, " (\"Exonautica\")");
			setAttribute(Qt::WA_DeleteOnClose);


			QSplitter*	splitter = new QSplitter(this);
			splitter->setObjectName(QString::fromUtf8("splitter"));
			splitter->setOrientation(Qt::Horizontal);


			stackedTextEdits = new QStackedWidget(splitter);




			engine = new SyntopiaCore::GLEngine::EngineWidget(splitter);

			tabBar = new QTabBar(this);

			QFrame* f = new QFrame(this);
			frameMainWindow = new QVBoxLayout();
			frameMainWindow->setSpacing(0);
			frameMainWindow->setMargin(4);
			f->setLayout(frameMainWindow);
			f->layout()->addWidget(tabBar);
			f->layout()->addWidget(splitter);
			setCentralWidget(f);

			QList<int> l; l.append(100); l.append(400);
			splitter->setSizes(l);


			createActions();
			createMenus();
			createToolBars();
			createStatusBar();

			QDir d(getExamplesDir());
			loadFile(d.absoluteFilePath("Default.es"));
			tabChanged(0); // to update title.

			
			// Log widget (in dockable window)
			dockLog = new QDockWidget(this);
			dockLog->setWindowTitle("Log");
			dockLog->setObjectName(QString::fromUtf8("dockWidget"));
			dockLog->setAllowedAreas(Qt::BottomDockWidgetArea);
			QWidget* dockLogContents = new QWidget(dockLog);
			dockLogContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
			QVBoxLayout* vboxLayout1 = new QVBoxLayout(dockLogContents);
			vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
			vboxLayout1->setContentsMargins(0, 0, 0, 0);

			ListWidgetLogger* logger = new ListWidgetLogger(dockLog);
			vboxLayout1->addWidget(logger->getListWidget());
			dockLog->setWidget(dockLogContents);
			addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockLog);
			INFO("Welcome to Structure Synth. A Syntopia Project.");
			INFO("Hold 'CTRL' for speed draw'.");
			INFO("Press 'Reset View' if the view disappears...");
			INFO("Zoom by pressing both mouse buttons, holding SHIFT+left mouse button, or using scroll wheel.");
			INFO("Translate using right mouse button.");


			fullScreenEnabled = false;
			createOpenGLContextMenu();

			connect(this->tabBar, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));



			readSettings();

		}

		void MainWindow::createOpenGLContextMenu() {
			openGLContextMenu = new QMenu();			
			openGLContextMenu->addAction(insertCameraSettingsAction);
			openGLContextMenu->addAction(fullScreenAction);
			openGLContextMenu->addAction(screenshotAction);
			openGLContextMenu->addAction(panicAction);
			engine->setContextMenu(openGLContextMenu);
		}


		void MainWindow::toggleFullScreen() {
			if (fullScreenEnabled) {
				frameMainWindow->setMargin(4);
				showNormal();
				fullScreenEnabled = false;
				fullScreenAction->setChecked(false);
				stackedTextEdits->show();
				dockLog->show();
				menuBar()->show();
				statusBar()->show();
				fileToolBar->show();
				editToolBar->show();
				renderToolBar->show();
				tabBar->show();
				randomToolBar->show();
				
			} else {
				frameMainWindow->setMargin(0);
				fullScreenAction->setChecked(true);
				fullScreenEnabled = true;

				tabBar->hide();
				stackedTextEdits->hide();
				dockLog->hide();
				menuBar()->hide();
				statusBar()->hide();
				fileToolBar->hide();
				editToolBar->hide();
				renderToolBar->hide();
				randomToolBar->hide();
				showFullScreen();
				
			}
		}



		void MainWindow::createActions()
		{
			fullScreenAction = new QAction(tr("F&ullscreen"), this);
			fullScreenAction->setShortcut(tr("Ctrl+F"));
			fullScreenAction->setCheckable(true);
			connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

			insertCameraSettingsAction  = new QAction(tr("&Copy Camera settings to EisenScript Window"), this);
			connect(insertCameraSettingsAction, SIGNAL(triggered()), this, SLOT(insertCameraSettings()));


			screenshotAction = new QAction(tr("&Save as bitmap..."), this);
			connect(screenshotAction, SIGNAL(triggered()), this, SLOT(makeScreenshot()));

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

			saveAsAction = new QAction(QIcon(":/images/filesaveas.png"), tr("Save &As..."), this);
			saveAsAction->setStatusTip(tr("Save the script under a new name"));
			connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

			closeAction = new QAction(QIcon(":/images/fileclose.png"), tr("&Close Tab"), this);
			closeAction->setShortcut(tr("Ctrl+W"));
			closeAction->setStatusTip(tr("Close this tab"));
			connect(closeAction, SIGNAL(triggered()), this, SLOT(closeTab()));

			exitAction = new QAction(QIcon(":/images/exit.png"), tr("E&xit Application"), this);
			exitAction->setShortcut(tr("Ctrl+Q"));
			exitAction->setStatusTip(tr("Exit the application"));
			connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

			cutAction = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
			cutAction->setShortcut(tr("Ctrl+X"));
			cutAction->setStatusTip(tr("Cut the current selection's contents to the "
				"clipboard"));
			//connect(cutAction, SIGNAL(triggered()), textEdit, SLOT(cut()));

			copyAction = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
			copyAction->setShortcut(tr("Ctrl+C"));
			copyAction->setStatusTip(tr("Copy the current selection's contents to the "
				"clipboard"));
			//connect(copyAction, SIGNAL(triggered()), textEdit, SLOT(copy()));

			pasteAction = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
			pasteAction->setShortcut(tr("Ctrl+V"));
			pasteAction->setStatusTip(tr("Paste the clipboard's contents into the current "
				"selection"));
			//connect(pasteAction, SIGNAL(triggered()), textEdit, SLOT(paste()));

			renderAction = new QAction(QIcon(":/images/render.png"), tr("&Render"), this);
			renderAction->setShortcut(tr("F5"));
			renderAction->setStatusTip(tr("Render the current ruleset"));
			connect(renderAction, SIGNAL(triggered()), this, SLOT(render()));

			/*
			povRenderAction = new QAction(QIcon(":/images/render.png"), tr("&Export as POV-Ray script"), this);
			povRenderAction->setShortcut(tr("F6"));
			povRenderAction->setStatusTip(tr("Export as POV-Ray script"));
			connect(povRenderAction, SIGNAL(triggered()), this, SLOT(povRender()));
			*/

			panicAction = new QAction("Reset View", this);
			panicAction->setStatusTip(tr("Resets the viewport"));
			connect(panicAction, SIGNAL(triggered()), this, SLOT(resetView()));


			aboutAction = new QAction(QIcon(":/images/documentinfo.png"), tr("&About"), this);
			aboutAction->setStatusTip(tr("Show the About box"));
			connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

			sfHomeAction = new QAction(QIcon(":/images/agt_internet.png"), tr("&Project Homepage (web link)"), this);
			sfHomeAction->setStatusTip(tr("Open the SourceForge project page in a browser."));
			connect(sfHomeAction, SIGNAL(triggered()), this, SLOT(launchSfHome()));

			referenceAction = new QAction(QIcon(":/images/agt_internet.png"), tr("&Structure Synth Reference (web link)"), this);
			referenceAction->setStatusTip(tr("Open a Structure Synth reference web page in a browser."));
			connect(referenceAction, SIGNAL(triggered()), this, SLOT(launchReferenceHome()));

			galleryAction = new QAction(QIcon(":/images/agt_internet.png"), tr("&Flickr Structure Synth Group (web link)"), this);
			galleryAction->setStatusTip(tr("Opens the main Flickr group for Structure Synth creations."));
			connect(galleryAction, SIGNAL(triggered()), this, SLOT(launchGallery()));

			cutAction->setEnabled(false);
			copyAction->setEnabled(false);
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
			//renderMenu->addAction(povRenderAction);

			// Scan render templates...
			QDir miscDir(getTemplateDir());
			QStringList filters;
			filters << "*.rendertemplate";
			miscDir.setNameFilters(filters);
			if (!miscDir.exists()) {
				QAction* a = new QAction("Unable to locate: "+miscDir.absolutePath(), this);
				a->setEnabled(false);
				renderMenu->addAction(a);
			} else {
				QStringList sl = miscDir.entryList();
				QMenu* templateMenu = renderMenu->addMenu(QIcon(":/images/render.png"), "Template Render to Clipboard");
				for (int i = 0; i < sl.size(); i++) {
					QAction* a = new QAction(sl[i], this);
					a->setData(sl[i]);
					connect(a, SIGNAL(triggered()), this, SLOT(templateRender()));
					templateMenu->addAction(a);
				}

				templateMenu = renderMenu->addMenu(QIcon(":/images/render.png"), "Template Render to File...");
				for (int i = 0; i < sl.size(); i++) {
					QAction* a = new QAction(sl[i], this);
					a->setData(sl[i]);
					connect(a, SIGNAL(triggered()), this, SLOT(templateRenderToFile()));
					templateMenu->addAction(a);
				}
			}

			renderMenu->addSeparator();
			renderMenu->addAction(fullScreenAction);
			renderMenu->addAction(screenshotAction);

			menuBar()->addSeparator();

			// Examples...
			QMenu* examplesMenu = menuBar()->addMenu(tr("&Examples"));

			// Scan examples dir...
			QDir d(getExamplesDir());
			filters.clear();
			filters << "*.es";
			d.setNameFilters(filters);
			if (!d.exists()) {
				QAction* a = new QAction("Unable to locate: "+d.absolutePath(), this);
				a->setEnabled(false);
				examplesMenu->addAction(a);
			} else {
				// we will recurse the dirs...
				QStack<QString> pathStack;
				pathStack.append(QDir(getExamplesDir()).absolutePath());

				QMap< QString , QMenu* > menuMap;
				while (!pathStack.isEmpty()) {
					
					QMenu* currentMenu = examplesMenu;
					QString path = pathStack.pop();
					if (menuMap.contains(path)) currentMenu = menuMap[path];
					QDir dir(path);

					QStringList sl = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
					for (int i = 0; i < sl.size(); i++) {
						QMenu* menu = new QMenu(sl[i]);
						QString absPath = QDir(path + QDir::separator() +  sl[i]).absolutePath();
						menuMap[absPath] = menu;
						currentMenu->addMenu(menu);
						menu->setIcon(QIcon(":/images/folder.png"));
						pathStack.push(absPath);
					}

					dir.setNameFilters(filters);

					sl = dir.entryList();
					for (int i = 0; i < sl.size(); i++) {
						INFO(QString("Found item:")+ sl[i]);
						QAction* a = new QAction(sl[i], this);
						a->setIcon(QIcon(":/images/mail_new.png"));
						

						QString absPath = QDir(path ).absoluteFilePath(sl[i]);
						
						a->setData(absPath);
						connect(a, SIGNAL(triggered()), this, SLOT(openFile()));
						currentMenu->addAction(a);
					}
				}

				
			}

			helpMenu = menuBar()->addMenu(tr("&Help"));
			helpMenu->addAction(aboutAction);
			helpMenu->addSeparator();
			helpMenu->addAction(sfHomeAction);
			helpMenu->addAction(referenceAction);
			helpMenu->addAction(galleryAction);
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

			randomToolBar = addToolBar(tr("Random"));

			QLabel* randomSeed = new QLabel("Seed:"); 
			randomToolBar->addWidget(randomSeed);
			seedSpinBox = new QSpinBox();
			seedSpinBox->setRange(1,32768);
			seedSpinBox->setValue(1);
			randomToolBar->addWidget(seedSpinBox);
			autoIncrementCheckbox = new QCheckBox("Auto Increment", randomToolBar);
			randomToolBar->addWidget(autoIncrementCheckbox);
			autoIncrementCheckbox->setChecked(true);

			

			renderToolBar = addToolBar(tr("Render"));
			renderToolBar->addAction(renderAction);
			renderToolBar->addAction(panicAction);



			connect(seedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(seedChanged()));
		}


		void MainWindow::seedChanged() {
			autoIncrementCheckbox->setChecked(false);
		}

		void MainWindow::createStatusBar()
		{
			statusBar()->showMessage(tr("Ready"));
		}

		void MainWindow::readSettings()
		{
			QSettings settings("Syntopia Software", "Structure Synth");
			QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
			QSize size = settings.value("size", QSize(1024, 800)).toSize();
			move(pos);
			resize(size);
		}

		void MainWindow::writeSettings()
		{
			QSettings settings("Syntopia Software", "Structure Synth");
			settings.setValue("pos", pos());
			settings.setValue("size", size());
		}


		void MainWindow::openFile()
		{
			QAction *action = qobject_cast<QAction *>(sender());
			if (action) {
				loadFile(action->data().toString());
			} else {
				WARNING("No data!");
			}
		}

		void MainWindow::loadFile(const QString &fileName)
		{
			insertTabPage(fileName);
		}

		bool MainWindow::saveFile(const QString &fileName)
		{
			if (tabBar->currentIndex() == -1) { WARNING("No open tab"); return false; } 
			

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
			out << getTextEdit()->toPlainText();
			QApplication::restoreOverrideCursor();

			tabInfo[tabBar->currentIndex()].hasBeenSavedOnce = true;
			tabInfo[tabBar->currentIndex()].unsaved = false;
			tabInfo[tabBar->currentIndex()].filename = fileName;
			tabChanged(tabBar->currentIndex()); // to update displayed name;

			statusBar()->showMessage(tr("File saved"), 2000);
			return true;
		}



		QString MainWindow::strippedName(const QString &fullFileName)
		{
			return QFileInfo(fullFileName).fileName();
		}


		void MainWindow::updateRandom() {

			if (tabBar->currentIndex() == -1) { WARNING("No open tab"); return; } 
			
			seedSpinBox->blockSignals(true);
			setSeed((getSeed()+1) % 32768);
			seedSpinBox->blockSignals(false);
			INFO(QString("Auto-incremented random seed: %1").arg(getSeed()));

			// Should we try something like below?
			if (tabInfo[tabBar->currentIndex()].unsaved) {
				// Current tab is unsaved, we will not change the random seed.
			} else {
				// We will auto-increment random seed.
			}

			srand(getSeed());
		}

		void MainWindow::render() {
			INFO(getTextEdit()->toPlainText());
			if (getTextEdit()->toPlainText().startsWith("#javascript", Qt::CaseInsensitive)) {
			    // This is javascript...
				QString text = getTextEdit()->toPlainText();
				text = text.remove("#javascript", Qt::CaseInsensitive);
				parseJavaScript(text);
				return;
			}

			if (autoIncrementCheckbox->isChecked()) updateRandom();
			srand(getSeed());
			INFO(QString("Random seed: %1").arg(getSeed()));

			try {
				
				Rendering::OpenGLRenderer renderTarget(engine);
				renderTarget.begin(); // we clear before parsing...

				Tokenizer tokenizer(Preprocessor::Process(getTextEdit()->toPlainText()));
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

				if (b.seedChanged()) {
					setSeed(b.getNewSeed());
					INFO(QString("Builder changed seed to: %1").arg(b.getNewSeed()));
				} 

				if (oldDirtyPosition > 0) {
					getTextEdit()->document()->markContentsDirty(oldDirtyPosition,1);
					oldDirtyPosition = -1;
				}

				//if (getTextEdit()->isWindowModified()) getTextEdit()->document()->markContentsDirty(0,getTextEdit()->toPlainText().count());


			} catch (ParseError& pe) {

				WARNING(pe.getMessage());
				int pos = pe.getPosition();
				INFO(QString("Found at character %1.").arg(pos));	
				getTextEdit()->document()->findBlock(pos).setUserState(2);
				getTextEdit()->document()->markContentsDirty(pos,1);
				oldDirtyPosition = pos;
				engine->clearWorld();
				engine->requireRedraw();
			} catch (Exception& er) {
				WARNING(er.getMessage());
				engine->clearWorld();
				engine->requireRedraw();
			} 

		}

		namespace {
			// Returns the first valid directory.
			QString findDirectory(QStringList guesses) {
				for (int i = 0; i < guesses.size(); i++) {
					if (QFile::exists(guesses[i])) return guesses[i];
				}

				// not found.
				WARNING("Could not locate directory in: " + guesses.join(",") + ".");
				return "[not found]";
			}
		}
		// Mac needs to step two directies up, when debugging in XCode...
		QString MainWindow::getExamplesDir() {
			QStringList examplesDir;
			examplesDir << "Examples" << "../../Examples";
			return findDirectory(examplesDir);
		}

		QString MainWindow::getMiscDir() {
			QStringList miscDir;
			miscDir << "Misc" << "../../Misc";
			return findDirectory(miscDir);
		}

		QString MainWindow::getTemplateDir() {
			return getMiscDir();
		}

		void MainWindow::resetView() {
			engine->reset();
		}

		QTextEdit* MainWindow::getTextEdit() {
			return (stackedTextEdits->currentWidget() ? (QTextEdit*)stackedTextEdits->currentWidget() : 0);
		}

		void MainWindow::cursorPositionChanged() {
			if (!this->getTextEdit()) return;
			int pos = this->getTextEdit()->textCursor().position();
			int blockNumber = this->getTextEdit()->textCursor().blockNumber();
			statusBar()->showMessage(QString("Position: %1, Line: %2").arg(pos).arg(blockNumber+1), 5000);
					
		}

		void MainWindow::insertTabPage(QString filename) {

			QTextEdit* textEdit = new QTextEdit();
			connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));

			textEdit->setLineWrapMode(QTextEdit::NoWrap);
			textEdit->setTabStopWidth(20);
			new EisenScriptHighlighter(textEdit);

			QString s = QString("// Write EisenScript code here...\r\n");
			textEdit->setText(s);

			bool loadingSucceded = false;
			if (!filename.isEmpty()) {
				INFO(QString("Loading file: %1").arg(filename));
				QFile file(filename);
				if (!file.open(QFile::ReadOnly | QFile::Text)) {
					textEdit->setPlainText(QString("Cannot read file %1:\n%2.").arg(filename).arg(file.errorString()));
				} else {
					QTextStream in(&file);
					QApplication::setOverrideCursor(Qt::WaitCursor);
					textEdit->setPlainText(in.readAll());
					QApplication::restoreOverrideCursor();
					statusBar()->showMessage(QString("Loaded file: %1").arg(filename), 2000);
					loadingSucceded = true;
				}
			}


			QString displayName = filename;
			if (displayName.isEmpty()) {
				// Find a new name
				displayName = "Unnamed";
				QString suggestedName = displayName;

				bool unique = false;
				int counter = 1;
				while (!unique) {
					unique = true;
					for (int i = 0; i < tabInfo.size(); i++) {
						if (tabInfo[i].filename == suggestedName) {
							INFO("equal");
							unique = false;
							break;
						}	
					}
					if (!unique) suggestedName = displayName + " " + QString::number(counter++);
				}
				displayName = suggestedName;
			}

			stackedTextEdits->addWidget(textEdit);

			if (loadingSucceded) {
				tabInfo.append(TabInfo(displayName, textEdit, false, true));
			} else {
				tabInfo.append(TabInfo(displayName, textEdit, true));
			}

			QString tabTitle = QString("%1%3").arg(strippedName(displayName)).arg(!loadingSucceded? "*" : "");
			tabBar->setCurrentIndex(tabBar->addTab(strippedName(tabTitle)));

			connect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

		}

		void MainWindow::tabChanged(int index) {
			if (index > tabInfo.size()) return;
			if (index < 0) return;

			TabInfo t = tabInfo[index];
			QString tabTitle = QString("%1%3").arg(strippedName(t.filename)).arg(t.unsaved ? "*" : "");
			setWindowTitle(QString("%1 - %2").arg(tabTitle).arg("Structure Synth"));
			stackedTextEdits->setCurrentWidget(t.textEdit);
			tabBar->setTabText(tabBar->currentIndex(), tabTitle);
		}

		void MainWindow::closeTab() {
			int index = tabBar->currentIndex();
			if (tabBar->currentIndex() == -1) { WARNING("No open tab"); return; } 
			
			TabInfo t = tabInfo[tabBar->currentIndex()];
			if (t.unsaved) {
				int answer = QMessageBox::warning(this, QString("Unsaved changes"), "Close this tab without saving changes?", "OK", "Cancel");
				if (answer == 1) return;
			}
			
			tabInfo.remove(index);
			tabBar->removeTab(index);
			
			stackedTextEdits->removeWidget(t.textEdit);
			delete(t.textEdit); // ?
			
		}

		void MainWindow::launchSfHome() {
			INFO("Launching web browser...");
			bool s = QDesktopServices::openUrl(QUrl("http://structuresynth.sourceforge.net/"));
			if (!s) WARNING("Failed to open browser...");
		}

		void MainWindow::launchReferenceHome() {
			INFO("Launching web browser...");
			bool s = QDesktopServices::openUrl(QUrl("http://structuresynth.sourceforge.net/reference.php"));
			if (!s) WARNING("Failed to open browser...");
		}

		void MainWindow::launchGallery() {
			INFO("Launching web browser...");
			bool s = QDesktopServices::openUrl(QUrl("http://flickr.com/groups/structuresynth/"));
			if (!s) WARNING("Failed to open browser...");
		}

		void MainWindow::makeScreenshot() {
			
			QList<QByteArray> a = QImageWriter::supportedImageFormats();
			QStringList allowedTypesFilter;
			QStringList allowedTypes;
			for (int i = 0; i < a.count(); i++) {
				allowedTypesFilter.append("*."+a[i]);
				allowedTypes.append(a[i]);
			}
			QString filter = "Image Files (" + allowedTypesFilter.join(" ")+")";

			QImage image = engine->grabFrameBuffer();

			QString filename = QFileDialog::getSaveFileName(this, "Save Screenshot As...", QString(), filter);
			if (filename.isEmpty()) {
				INFO("User cancelled save...");
				return;
			}


			QString ext = filename.section(".", -1).toLower();
			if (!allowedTypes.contains(ext)) {
				WARNING("Invalid image extension.");
				WARNING("File must be one of the following types: " + allowedTypes.join(","));
				return;
			}

			bool succes = image.save(filename);
			if (succes) {
				INFO("Saved screenshot as: " + filename);
			} else {
				WARNING("Save failed! Filename: " + filename);
			}


		}

		void MainWindow::setSeed(int randomSeed) {
			seedSpinBox->setValue(randomSeed);
		}

		int MainWindow::getSeed() {
			return seedSpinBox->value();
		};

		void MainWindow::insertCameraSettings() {
			///xxx
			Vector3f translation = engine->getTranslation();
			Matrix4f rotation = engine->getRotation();
			Vector3f pivot = engine->getPivot();
			double scale = engine->getScale();

			QStringList sl;
			sl << "// Camera settings. Place these before first rule call." 
			   << QString("set translation %1").arg(translation.toString())
			   << QString("set rotation %1").arg(rotation.toStringAs3x3())
			   << QString("set pivot %1").arg(pivot.toString())
			   << QString("set scale %1").arg(scale);


			getTextEdit()->insertPlainText(sl.join("\r\n"));
			INFO("Camera settings are now pasted into the script window.");
			INFO("Remember to clear previous 'set' commands if neccesary.");
		}

		void MainWindow::templateRenderToFile()
		{
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "output.txt");
			if (fileName.isEmpty()) {
				INFO("User cancelled.");
				return;
			}

			
			templateRender(fileName);
			
			
		}

		void MainWindow::templateRender()
		{
			templateRender(""); // Renders to clip board when file name is empty.
		}

		void MainWindow::templateRender(const QString& fileName)
		{
			QAction *action = qobject_cast<QAction *>(sender());
			if (action) {
				QDir d(getTemplateDir());
				QString templateFileName = d.absoluteFilePath(action->data().toString());
				INFO("Starting Template Renderer: " + fileName);
				
				srand(getSeed());
				INFO(QString("Random seed: %1").arg(getSeed()));
				try {
					QString text = "// Structure Synth Export. \r\n\r\n";
					TemplateRenderer rendering(templateFileName);
					rendering.begin(); // we clear before parsing...

					Tokenizer tokenizer(Preprocessor::Process(getTextEdit()->toPlainText()));
					EisenParser e(&tokenizer);
					INFO("Parsing...");
					RuleSet* rs = e.parseRuleset();

					INFO("Resolving named references...");
					rs->resolveNames();

					rs->dumpInfo();

					INFO("Building....");
					Builder b(&rendering, rs);
					b.build();
					rendering.end();

					if (b.seedChanged()) {
						setSeed(b.getNewSeed());
						INFO(QString("Builder changed seed to: %1").arg(b.getNewSeed()));
					} 

					if (fileName.isEmpty()){
						QClipboard *clipboard = QApplication::clipboard();
						clipboard->setText(rendering.getOutput()); 
					} else {
						QFile file(fileName);
						INFO("Writing to file: " + QFileInfo(file).absoluteFilePath());
						if (!file.open(QFile::WriteOnly | QFile::Text)) {
							QMessageBox::warning(this, tr("Structure Synth"),
								tr("Cannot write file %1:\n%2.")
								.arg(fileName)
								.arg(file.errorString()));
							return;
						}

						QTextStream out(&file);
						QApplication::setOverrideCursor(Qt::WaitCursor);
						out << rendering.getOutput();
						QApplication::restoreOverrideCursor();
						INFO("File saved.");
					}

					INFO("Done...");
					INFO("Script is now copied to the clipboard");


				} catch (Exception& er) {
					WARNING(er.getMessage());
				}

			} else {
				WARNING("No data!");
			}
		}


		void MainWindow::parseJavaScript(QString scripture) {
			JavaScriptParser jsp(engine);
			jsp.parse(scripture);

		}
			
	}
}

