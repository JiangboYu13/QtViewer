#include <QApplication>
#include <QCommandLineParser>
#include <QtCore>
#include <QString>
#include <QWidget>
#include "QtViewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(QString("Image Viewer"));
    QtViewer imageViewer;
    imageViewer.show();
    return app.exec();
}