#include "XVideoWidget.h"

XVideoWidget::XVideoWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    qDebug()<<"XVideoWiget create";
    render = new OpenglRender(this);
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
}

XVideoWidget::~XVideoWidget() {
    qDebug()<<"XVideoWiget destroy";
    // if(fp) {
    //     fclose(fp);
    //     fp = nullptr;
    // }
    if(render) {
        delete render;
        render = nullptr;
    }
}

void XVideoWidget::initializeGL() {
    qDebug() << "initializeGL";
    mux.lock();
    render->initialize();
    mux.unlock();

}

void XVideoWidget::paintGL() {
    mux.lock();
    render->paintGL();
    mux.unlock();


}

void XVideoWidget::resizeGL(int width,int height) {
    qDebug()<<"XVideoWiget::resizeGL width:"<<width<<"  height:"<<height;
    mux.lock();
    if(height == 0)// 防止被零除
    {
        height = 1;// 将高设为1
    }
    //设置视口
    render->resize(width,height);
    mux.unlock();
}

void XVideoWidget::Repaint(AVFrame *frame)
{
    if (!frame)return;
    mux.lock();
    //�ݴ�����֤�ߴ���ȷ
    if (! render->datas[0] || render->width*render->height == 0 || frame->width != render->width || frame->height != render->height)
    {
        av_frame_free(&frame);
        mux.unlock();
        return;
    }
    memcpy( render->datas[0], frame->data[0], render->width* render->height);
    memcpy( render->datas[1], frame->data[1], render->width* render->height/4);
    memcpy( render->datas[2], frame->data[2], render->width* render->height/4);
    //�ж�������
    mux.unlock();

    //ˢ����ʾ
    update();
}

void XVideoWidget::Init(int width, int height)
{
    mux.lock();
    render->width = width;
    render->height = height;

    render->Init();
    mux.unlock();
}
