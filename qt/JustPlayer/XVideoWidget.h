#ifndef XVIDEOWIGET_H
#define XVIDEOWIGET_H

#include <QObject>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QFile>


#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4




class XVideoWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    XVideoWidget(QWidget* parent);
    ~XVideoWidget();

protected:
    void paintGL(); //刷新现实
    void initializeGL();    //初始化
    void resizeGL(int width,int height);    //窗口大小变化

private:
    void bindGL(int index);

    //data

private:
    QOpenGLShaderProgram *program;
    //shader中yuv变量地址
    GLuint unis[3] = {0};
    //shader中tex变量地址
    GLuint texs[3] = {0};

    unsigned char* datas[3];

    FILE *fp;

    QTimer *timer;

    int width =240;
    int height = 128;

    // int width =716;
    // int height = 1280;


    //vertex shader
    const char *vString = GET_STR(
        attribute vec4 vertexIn;
        attribute vec2 textureIn;
        varying vec2 textureOut;
        void main(void)
        {
            gl_Position = vertexIn;
            textureOut = textureIn;
        }
        );


    //shader element
    const char *tString = GET_STR(
        varying vec2 textureOut;
        uniform sampler2D tex_y;
        uniform sampler2D tex_u;
        uniform sampler2D tex_v;
        void main(void)
        {
            vec3 yuv;
            vec3 rgb;
            yuv.x = texture2D(tex_y, textureOut).r;
            yuv.y = texture2D(tex_u, textureOut).r - 0.5;
            yuv.z = texture2D(tex_v, textureOut).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            gl_FragColor = vec4(rgb, 1.0);
        }
        );
    // const char *tString = GET_STR(
    //     varying vec2 textureOut;
    //     uniform sampler2D tex_y;
    //     uniform sampler2D tex_u;
    //     uniform sampler2D tex_v;
    //     void main(void)
    //     {
    //         vec3 yuv;
    //         vec3 rgb;
    //         vec4 c = vec4((texture(tex_y, textureOut).r - 16./255.) * 1.164);
    //         vec4 U = vec4(texture(tex_u, textureOut).r - 128./255.);
    //         vec4 V = vec4(texture(tex_v, textureOut).r - 128./255.);
    //         c += V * vec4(1.596, -0.813, 0, 0);
    //         c += U * vec4(0, -0.392, 2.017, 0);
    //         c.a = 1.0;
    //         gl_FragColor = c;
    //     }
    //     );

    //vertex
    const GLfloat ver[12] = {
        -1.0f,-1.0f,0.0f,
        1.0f,-1.0f,0.0f,
        -1.0f,1.0f,0.0f,
        1.0f,1.0f,0.0f,
    };

    //texture
    const GLfloat tex[8] = {
        0.0f,1.0f,
        1.0f,1.0f,
        0.0f,0.0f,
        1.0f,0.0f,
    };





};

#endif // XVIDEOWIGET_H
