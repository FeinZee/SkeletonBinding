#include "myopenglwidget.h"


MyOpenglWidget::MyOpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      lightLocation(10,10,1)
{

}

void MyOpenglWidget::initializeGL(){

    initializeOpenGLFunctions();
    /* about model */
    obj_renderer.initsize(":/obj/vanquish.obj",QImage(":/texture/vanquish.jpg"));

    /* about skeleton */
    s_renderer.initialize(0.02); // radius of the sphere
}

void MyOpenglWidget::resizeGL(int w, int h){
    width = w;
    height = h;

    glViewport(0, 0, w, h);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

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
    glClearColor(0.1f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* about obj */
    QMatrix4x4 viewMatrix = camera.GetViewMatrix();
    // viewMatrix.lookAt(cameraLocation, targetLocation, QVector3D(0,1,0));

    QMatrix4x4 mMatrix;
    mMatrix.translate(0,-0.8);
    mMatrix.rotate(obj_angleY,0,1,0);
    obj_renderer.render(f, projMatrix, viewMatrix, mMatrix, camera.Position, lightLocation);
    /* about skeleton */
    for (auto joint: joints) {
        s_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, joint.getModelMatrix(), viewMatrix, lightLocation);
    }
}

void MyOpenglWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        /* here remains a question: how to get the z?
         * For now, I get z by testing the screen position of a sphere at world cordinate - (1, 0, 0).
         */
        float z = 0.995;
        QVector3D worldPosition = QVector3D(event->x(), height - event->y(), z).unproject(camera.GetViewMatrix(), projMatrix, QRect(0, 0, width, height));
        joints.push_back(SkeletonJoint(worldPosition));
        update();
    }else if (event->button() == Qt::LeftButton){
        lastMouseX = event->x();
        lastMouseY = event->y();
        lastPressMouseX = event->x();
        lastPressMouseY = event->y();
        isMousePressed = true;
    }
}


void MyOpenglWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isMousePressed) {
        int offsetX = event->x() - lastMouseX;
        int offsetY = event->y() - lastMouseY;
        camera.ProcessMouseMovement(offsetX, offsetY, true);
        lastMouseX = event->x();
        lastMouseY = event->y();
        update();
    }
}



void MyOpenglWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMousePressed = false;
        if (lastPressMouseX == event->x() && lastPressMouseY == event->y()) {
            qDebug() << "same position";
        }else {
            qDebug() << "different position";
        }
    }
}

