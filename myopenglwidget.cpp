#include "myopenglwidget.h"


MyOpenglWidget::MyOpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      lightLocation(10,10,1)
{
}

void MyOpenglWidget::initializeGL(){

    /* about model */
    obj_renderer.initsize(":/obj/vanquish.obj",QImage(":/texture/vanquish.jpg"));

    /* about skeleton */
    s_renderer.initialize(0.02); // radius of the sphere
}

void MyOpenglWidget::resizeGL(int w, int h){
    width = w;
    height = h;

    projMatrix.setToIdentity();
    projMatrix.perspective(camera.Zoom, float(w)/h, 0.01f, 100.0f);
}

void MyOpenglWidget::wheelEvent(QWheelEvent *event)
{
    if (!event->pixelDelta().isNull()) {
        camera.ProcessMouseScroll(event->pixelDelta().y());
        // cameraLocation.setZ(cameraLocation.z() + event->pixelDelta().y()); //重置视点z值
    } else if (!event->angleDelta().isNull()) {
        camera.ProcessMouseScroll((event->angleDelta()/120).y());
        // cameraLocation.setZ(cameraLocation.z() + (event->angleDelta() / 120).y()); //重置视点z值
    }
    projMatrix.setToIdentity();
    projMatrix.perspective(camera.Zoom, float(width)/height, 0.01f, 100.0f);
    event->accept();
    update();
}

void MyOpenglWidget::paintGL(){
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glClearColor(0.1f,0.0f,0.0f,1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* about obj */
    QMatrix4x4 viewMatrix = camera.GetViewMatrix();
    // viewMatrix.lookAt(cameraLocation, targetLocation, QVector3D(0,1,0));

    QMatrix4x4 mMatrix;
    mMatrix.translate(0,-0.8);
    mMatrix.rotate(obj_angleY,0,1,0);
    obj_renderer.render(f, projMatrix, viewMatrix, mMatrix, camera.Position, lightLocation);
    /* about skeleton */
    QMatrix4x4 test;
    test.setToIdentity();
    test.translate(pos);
    s_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, test, viewMatrix, lightLocation);
    for (auto joint: joints) {
        s_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, joint.getModelMatrix(), viewMatrix, lightLocation);
    }

}

void MyOpenglWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {        
        QMatrix4x4 modelMatrix;
        modelMatrix.setToIdentity();
        QMatrix4x4 viewMatrix = camera.GetViewMatrix();
        viewMatrix /= -(camera.Position.z() / 2);
        QRect viewPort = QRect(0.0, 0.0, width, height);
        pos = QVector3D(event->pos().rx(),height - event->pos().ry(), 1);
        qDebug() << pos;
        QMatrix4x4 inverse = QMatrix4x4(viewMatrix * projMatrix).inverted();
        QVector4D tmp(pos, 1.0f);
        tmp.setX((tmp.x() - float(viewPort.x())) / float(viewPort.width()));
        tmp.setY((tmp.y() - float(viewPort.y())) / float(viewPort.height()));
        tmp = tmp * 2.0f - QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
        QVector4D obj = inverse * tmp;
        pos = obj.toVector3D();
        if (camera.Position.z() > 0) {
            pos.setX(-pos.x());
            pos.setY(-pos.y());
        }
        qDebug() << pos;
        update();
    }else if (event->button() == Qt::LeftButton){
        lastMouseX = event->pos().rx();
        lastMouseY = event->pos().ry();
        isMousePressed = true;
    }
}


void MyOpenglWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isMousePressed) {
        int offsetX = event->pos().rx() - lastMouseX;
        int offsetY = event->pos().ry() - lastMouseY;
        camera.ProcessMouseMovement(offsetX, offsetY, true);
        lastMouseX = event->pos().rx();
        lastMouseY = event->pos().ry();
        update();
    }
}



void MyOpenglWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMousePressed = false;
    }
}

