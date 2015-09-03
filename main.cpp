#include <QtGui>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    app.setApplicationName("Music Player");
    app.setQuitOnLastWindowClosed(false);

    MainWindow win;
    win.show();

    return app.exec();
}

