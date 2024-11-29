#include "OpenglRender.h"


OpenglRender::OpenglRender(QObject *parent)
    :m_pParent(parent){

}

OpenglRender::~OpenglRender()
{

}

void OpenglRender::initialize()
{
    qDebug() << "initializeGL";
    //��ʼ��opengl ��QOpenGLFunctions�̳У�����
    initializeOpenGLFunctions();

    //program����shader�������ƬԪ���ű�
    //ƬԪ�����أ�

    //vertex shader顶点着色器源码
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


    //shader element 片段着色器源码
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
    qDebug() << program.addShaderFromSourceCode(QOpenGLShader::Fragment, tString);
    //����shader
    qDebug() << program.addShaderFromSourceCode(QOpenGLShader::Vertex, vString);

    //���ö�������ı���
    program.bindAttributeLocation("vertexIn", A_VER);

    //���ò�������
    program.bindAttributeLocation("textureIn", T_VER);

    //����shader
    qDebug() << "program.link() = " << program.link();

    qDebug() << "program.bind() = " << program.bind();
    //����
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
    glEnableVertexAttribArray(A_VER);

    //����
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
    glEnableVertexAttribArray(T_VER);


    //��shader��ȡ����
    unis[0] = program.uniformLocation("tex_y");
    unis[1] = program.uniformLocation("tex_u");
    unis[2] = program.uniformLocation("tex_v");
}

void OpenglRender::paintGL()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs[0]); //0��󶨵�Y����
        //�޸Ĳ�������(�����ڴ�����)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
    //��shader uni��������
    glUniform1i(unis[0], 0);


    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texs[1]); //1��󶨵�U����
        //�޸Ĳ�������(�����ڴ�����)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
    //��shader uni��������
    glUniform1i(unis[1], 1);


    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, texs[2]); //2��󶨵�V����
        //�޸Ĳ�������(�����ڴ�����)
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
    //��shader uni��������
    glUniform1i(unis[2], 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    qDebug() << "paintGL";
}

void OpenglRender::resize(int width, int height)
{
    // glViewport(0,0,width,height);
}

void OpenglRender::Init()
{

    delete datas[0];
    delete datas[1];
    delete datas[2];
    ///��������ڴ�ռ�
    datas[0] = new unsigned char[width*height];		//Y
    datas[1] = new unsigned char[width*height / 4];	//U
    datas[2] = new unsigned char[width*height / 4];	//V
    if (texs[0])
    {
        glDeleteTextures(3, texs);
    }
    //��������
    glGenTextures(3, texs);

    //Y
    glBindTexture(GL_TEXTURE_2D, texs[0]);
    //�Ŵ���ˣ����Բ�ֵ   GL_NEAREST(Ч�ʸߣ�������������)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //���������Կ��ռ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //U
    glBindTexture(GL_TEXTURE_2D, texs[1]);
    //�Ŵ���ˣ����Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //���������Կ��ռ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //V
    glBindTexture(GL_TEXTURE_2D, texs[2]);
    //�Ŵ���ˣ����Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //���������Կ��ռ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
}



