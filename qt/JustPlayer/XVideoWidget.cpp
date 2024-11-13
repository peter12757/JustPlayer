#include "XVideoWidget.h"

XVideoWidget::XVideoWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    qDebug()<<"XVideoWiget create";
}

XVideoWidget::~XVideoWidget() {
    qDebug()<<"XVideoWiget destroy";
}

void XVideoWidget::initializeGL() {
    qDebug()<<"XVideoWiget::initializeGL";
}

void XVideoWidget::psintGL() {
    qDebug()<<"XVideoWiget::psintGL";
}

void XVideoWidget::resizeGL(int width,int height) {
    qDebug()<<"XVideoWiget::resizeGL width:"<<width<<"  height:"<<height;
}
