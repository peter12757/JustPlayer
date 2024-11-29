#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qsurfaceformat.h>
#include "src/justplayer/JustDemux.h"

int main(int argc, char *argv[])
{
<<<<<<< HEAD
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, true);
    // QSurfaceFormat fmt;
    // fmt.setVersion(3, 0);
    // fmt.setProfile(QSurfaceFormat::CoreProfile);
    // QSurfaceFormat::setDefaultFormat(fmt);
=======
    string rtmp_url = "rtmp://live.hkstv.hk.lxdns.com/live/hks1";
    string file_url = "test_1.mp4";
    JustDemux demux;
    bool ret = demux.Open(file_url);
    if (ret) {
        for(;;) {
            AVPacket *pkt = demux.Read();
            if(!pkt) {
                qDebug()<<"read complete";
                break;
            }
        }

    }

    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL, true);
>>>>>>> 1ece582 (mux & codec)

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
    return a.exec();
}
