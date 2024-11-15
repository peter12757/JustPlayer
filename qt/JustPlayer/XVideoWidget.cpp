#include "XVideoWidget.h"

XVideoWidget::XVideoWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    qDebug()<<"XVideoWiget create";
}

XVideoWidget::~XVideoWidget() {
    qDebug()<<"XVideoWiget destroy";
    if(fp) {
        fclose(fp);
        fp = nullptr;
    }
}

void XVideoWidget::initializeGL() {
    qDebug()<<"XVideoWiget::initializeGL";
    //初始化opengl
    initializeOpenGLFunctions();

    program = new QOpenGLShaderProgram;
    //program加载shader脚本
    qDebug()<<program->addShaderFromSourceCode(QOpenGLShader::Fragment,tString);
    qDebug()<<program->addShaderFromSourceCode(QOpenGLShader::Vertex,vString);


    //设置顶点坐标的变量
    program->bindAttributeLocation("vertexIn",A_VER);

    //设置材质坐标
    program->bindAttributeLocation("textureIn",T_VER);

    //编译shader
    program->link();
    program->bind();

    //传递顶点和材质坐标
    glVertexAttribPointer(A_VER,3,GL_FLOAT,0,0,ver);
    glEnableVertexAttribArray(A_VER);
    glVertexAttribPointer(T_VER,2,GL_FLOAT,0,0,tex);
    glEnableVertexAttribArray(T_VER);

    //从shader获取材质
    unis[0] = program->uniformLocation("tex_y");
    unis[1] = program->uniformLocation("tex_u");
    unis[2] = program->uniformLocation("tex_v");

    //创建材质
    glGenTextures(3,texs);

    //y
    glBindTexture(GL_TEXTURE_2D,texs[0]);


    //创建材质控件，在显卡控件中
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width,height,0,GL_RED,GL_UNSIGNED_BYTE,0);

    //u
    glBindTexture(GL_TEXTURE_2D,texs[1]);

    //创建材质控件，在显卡控件中
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width/2,height/2,0,GL_RED,GL_UNSIGNED_BYTE,0);


    //y
    glBindTexture(GL_TEXTURE_2D,texs[2]);


    //创建材质控件，在显卡控件中
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width/2,height/2,0,GL_RED,GL_UNSIGNED_BYTE,0);


    //分配材质的内存控件
    datas[0] = new unsigned char[width*height];     //y
    datas[1] = new unsigned char[width*height/4];   //u
    datas[2] = new unsigned char[width*height/4];   //v

    fp=fopen("out240x128.yuv","rb");
    // fp=fopen("output716_1280.yuv","rb");
    if (!fp) {
        qDebug()<<"fp open fail";
    }
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(40);
}

void XVideoWidget::paintGL() {

    if (feof(fp)) {
        // fseek(fp,0,SEEK_SET);
        qDebug()<<"XVideoWiget::paintGL complete";
        timer->stop();
    }
    bindGL(0);
    bindGL(1);
    bindGL(2);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    qDebug()<<"XVideoWiget::psintGL end";


}

void XVideoWidget::resizeGL(int width,int height) {
    qDebug()<<"XVideoWiget::resizeGL width:"<<width<<"  height:"<<height;
    if(height == 0)// 防止被零除
    {
        height = 1;// 将高设为1
    }
    //设置视口
    glViewport(0, 0, width, height);
}

void XVideoWidget::bindGL(const int index)
{

    int tempWidth = width/2;
    int tempHeight = height/2;
    if (index == 0) {
        tempWidth = width;
        tempHeight = height;
    }

    fread(datas[index],1,tempWidth*tempHeight,fp);
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,texs[index]);   //index层绑定到材质

    //修改材质内容(复制内存空间)
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,tempWidth,tempHeight,GL_RED,GL_UNSIGNED_BYTE,datas[index]);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1i(unis[index],index);

}
