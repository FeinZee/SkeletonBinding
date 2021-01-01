#include "myopenglwidget.h"


MyOpenglWidget::MyOpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    connect(&m_tm,SIGNAL(timeout()),this,SLOT(slotTimeout()));
        m_tm.start(30);
}

void MyOpenglWidget::initializeGL(){
    m_render.initsize(":/obj/vanquish.obj",QImage(":/texture/vanquish.jpg"));
    m_cameraLocation.setX(0);
    m_cameraLocation.setY(0);
    m_cameraLocation.setZ(3);
    m_lightLocation.setX(10);
    m_lightLocation.setY(10);
    m_lightLocation.setZ(1);

//    initializeOpenGLFunctions();
//    glClearColor(1,0,0,0);
}

void MyOpenglWidget::resizeGL(int w, int h){
    m_pMatrix.setToIdentity();
    m_pMatrix.perspective(45, float(w)/h, 0.01f, 100.0f);
}

void MyOpenglWidget::paintGL(){
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glClearColor(0.1f,0.0f,0.0f,1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix;
    vMatrix.lookAt(m_cameraLocation,QVector3D(0,0,0),QVector3D(0,1,0));

    QMatrix4x4 mMatrix;
    mMatrix.translate(0,-0.8);
//    mMatrix.rotate(angleX_,1,0,0);
    mMatrix.rotate(m_angleY,0,1,0);
//    mMatrix.rotate(anglZ_,0,0,1);
    m_render.render(f,m_pMatrix,vMatrix,mMatrix,m_cameraLocation,m_lightLocation);

}

void MyOpenglWidget::slotTimeout()
{
    m_angleX += 5;
    m_angleY += 5;
    m_anglZ += 5;
    update();
}
