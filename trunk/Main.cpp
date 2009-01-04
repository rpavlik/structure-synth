#include <QApplication>

#include "StructureSynth/GUI/MainWindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(StructureSynth);
    QApplication app(argc, argv);

	QPixmap pixmap(QDir(StructureSynth::GUI::MainWindow::getMiscDir()).absoluteFilePath("splash.png"));
	QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
	splash.show();
	qApp->processEvents();

	StructureSynth::GUI::MainWindow *mainWin = new StructureSynth::GUI::MainWindow();
    mainWin->show();
	splash.show();
	return app.exec();
}

