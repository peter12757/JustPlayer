#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QDebug>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4




//vertex
static const GLfloat ver[] = {
    -1.0f,-1.0f,
    1.0f,-1.0f,
    -1.0f, 1.0f,
    1.0f,1.0f
};

//texture
static const GLfloat tex[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
};

class RenderData {
public:
    unsigned char* m_pBufYuv;
    //视频宽高的size，编码格式可能都不一样
    int m_nBufW;
    int m_nBufH;

public:
    RenderData(int width,int height){
        qDebug()<<"RenderData crate width:"<<width<<"height:"<<height;
        m_pBufYuv = new unsigned char[width*height];
        m_nBufW = width;
        m_nBufH = height;
    }

};

class OpenglRender : protected QOpenGLFunctions
{
public:
    OpenglRender(QObject *parent = nullptr);
    ~OpenglRender();

    void initialize();
    void paintGL();
    void resize(int width,int height);

    void Init();



public:
    QObject *m_pParent;
    //shader��yuv������ַ
    GLuint unis[3] = { 0 };
    //openg�� texture��ַ
    GLuint texs[3] = { 0 };

    int width = 240;
    int height = 128;

    unsigned char *datas[3] = { 0 };

    QOpenGLShaderProgram program;



};

#endif // OPENGLRENDER_H
