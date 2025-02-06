#ifndef XVIDEOWIGET_H
#define XVIDEOWIGET_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <mutex>
#include "opengl/OpenglRender.h"
#include <QtOpenGLWidgets/QOpenGLWidget>
#include "justplayer/ffinc.h"
#include <justplayer/video/IJVideocall.h>



struct AVFrame;

class XVideoWidget : public QOpenGLWidget,public IJVideoCall
{
    Q_OBJECT

public:
    XVideoWidget(QWidget* parent);
    ~XVideoWidget();
    virtual void Init(int width, int height);
    virtual void Repaint(AVFrame *frame);

protected:
    void paintGL(); //刷新现实
    void initializeGL();    //初始化
    void resizeGL(int width,int height);    //窗口大小变化




private:
    void bindGL(int index);

    //data

private:

    OpenglRender *render;

    FILE *fp;

    QTimer *timer;
    std::mutex mux;






};

#endif // XVIDEOWIGET_H
