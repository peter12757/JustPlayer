#ifndef XVIDEOWIGET_H
#define XVIDEOWIGET_H

#include <QObject>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDebug>

class XVideoWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    XVideoWidget(QWidget* parent);
    ~XVideoWidget();

protected:
    void psintGL(); //刷新现实
    void initializeGL();    //初始化
    void resizeGL(int width,int height);    //窗口大小变化
};

#endif // XVIDEOWIGET_H
