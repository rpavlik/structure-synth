#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(StructureSynth);
    QApplication app(argc, argv);
	StructureSynth::GUI::MainWindow *mainWin = new StructureSynth::GUI::MainWindow;
    mainWin->show();
    return app.exec();
}
