#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qsurfaceformat.h>
#include "XVideoWidget.h"
#include "test/TestCase.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, true);


    TestThread tt;
    tt.Init();



    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "JustPlayer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();

    XVideoWidget *widget = w.findChild<XVideoWidget *>("openGLWidget");
    widget->Init(tt.demux.width, tt.demux.height);
    tt.video = widget;
    tt.start();

    return a.exec();
}
