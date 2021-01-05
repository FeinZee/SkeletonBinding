#include "myopenglwidget.h"


MyOpenglWidget::MyOpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      cameraLocation(0,0,3),
      lightLocation(10,10,1),
      targetLocation(0,0,0)
{

}

void MyOpenglWidget::initializeGL(){

    /* about model */
    obj_renderer.initsize(":/obj/vanquish.obj",QImage(":/texture/vanquish.jpg"));

    /* about skeleton */
    s_modelMatrix.setToIdentity();
    s_modelMatrix.translate(0, 0, 1);
    s_renderer.initialize(0.02); // radius of the sphere
}

void MyOpenglWidget::resizeGL(int w, int h){
    /* about model */
    obj_projMatrix.setToIdentity();
    obj_projMatrix.perspective(45, float(w)/h, 0.01f, 100.0f);

    /* about skeleton */
    s_projMatrix.setToIdentity();
    s_projMatrix.perspective(45, float(w)/h, 0.01f, 100.0f);
}

void MyOpenglWidget::wheelEvent(QWheelEvent *event)
{
    if (!event->pixelDelta().isNull()) {
        cameraLocation.setZ(cameraLocation.z() + event->pixelDelta().y()); //重置视点z值
    } else if (!event->angleDelta().isNull()) {
        cameraLocation.setZ(cameraLocation.z() + (event->angleDelta() / 120).y()); //重置视点z值
    }

    event->accept();
    update();
}

void MyOpenglWidget::paintGL(){
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glClearColor(0.1f,0.0f,0.0f,1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* about obj */
    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(cameraLocation, targetLocation, QVector3D(0,1,0));

    QMatrix4x4 mMatrix;
    mMatrix.translate(0,-0.8);
    mMatrix.rotate(obj_angleY,0,1,0);
    obj_renderer.render(f, obj_projMatrix, viewMatrix, mMatrix, cameraLocation, lightLocation);

    /* about skeleton */
    s_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), s_projMatrix, s_modelMatrix, viewMatrix, lightLocation);
}

void MyOpenglWidget::slotTimeout()
{
    obj_angleX += 5;
    obj_angleY += 5;
    obj_angleZ += 5;
    update();
}

void MyOpenglWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        QPoint p = event->pos();
        qDebug() << p.x() << " " << p.y();
    }
}
