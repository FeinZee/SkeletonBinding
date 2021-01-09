#include "myopenglwidget.h"


MyOpenglWidget::MyOpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      lightLocation(10,10,1)
{
    setFocusPolicy(Qt::ClickFocus);
}

void MyOpenglWidget::initializeGL(){

    initializeOpenGLFunctions();

    /* about model */
    obj_renderer.initsize(":/obj/vanquish.obj",QImage(":/texture/vanquish.jpg"));

    /* about skeleton */
    s_renderer.initialize(0.02f); // 0.02 is the radius of the sphere joint
    s_selected_renderer.initialize(0.025f);
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
    glClearColor(0.1f,0.1f,0.1f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* about obj */
    QMatrix4x4 viewMatrix = camera.GetViewMatrix();
    // viewMatrix.lookAt(cameraLocation, targetLocation, QVector3D(0,1,0));

    QMatrix4x4 mMatrix;
    mMatrix.translate(0, -0.8f);
    // mMatrix.rotate(obj_angleY,0,1,0);
    obj_renderer.render(f, projMatrix, viewMatrix, mMatrix, camera.Position, lightLocation, false);
    /* about skeleton */
    for (auto joint: joints) {
        s_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, joint.getModelMatrix(), viewMatrix,
                          camera.Position, lightLocation, QVector4D(0.6f, 0, 0, 0));
    }
    if (aJointIsSelected) {
        s_selected_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, selectedJoint.getModelMatrix(), viewMatrix,
                                   camera.Position, lightLocation, QVector4D(0.6f, 0.6f, 0, 0));
        qDebug() << "拾取到的点" << selectedJoint.getWorldCoordinate();
    }


}

void MyOpenglWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        /* here remains a question: how to get the z?
         * For now, I get z by testing the screen position of a sphere at world cordinate - (1, 0, 0).
         */
        float z = 0.996;
        QVector3D worldPosition = QVector3D(event->x(), height - event->y(), z).unproject(camera.GetViewMatrix(), projMatrix, QRect(0, 0, width, height));
        joints.push_back(SkeletonJoint(worldPosition));
        qDebug() << worldPosition;


        update();
    }else if (event->button() == Qt::LeftButton){
        lastMouseX = event->x();
        lastMouseY = event->y();
        lastPressMouseX = event->x();
        lastPressMouseY = event->y();
        isMousePressed = true;
    }
}

QVector3D MyOpenglWidget::getScreenCoord(QVector3D pos) {
    QVector3D screenPos = pos.project(camera.GetViewMatrix(), projMatrix, QRect(0, 0, width, height));
    screenPos[1] = height - screenPos[1];
    return screenPos;
}

float MyOpenglWidget::distance(int x1, int y1, int x2, int y2) {
    return sqrt(float(x2-x1) * (x2-x1) + (y2-y1) * (y2-y1));
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
            // test if pick a joint
            bool PickSuccess = false;
            for (auto j: joints) {
                QVector3D screenCoord = getScreenCoord(j.getWorldCoordinate());
                if (distance(int(screenCoord.x()), int(screenCoord.y()), event->x(), event->y()) < 10) {
                    qDebug() << "拾取到一个点" << j.getWorldCoordinate();
                    aJointIsSelected = true;
                    PickSuccess = true;
                    selectedJoint = j;
                    break;
                }
            }
            if (!PickSuccess) {
                aJointIsSelected = false;
            }
            update();

        }else {
        }
    }
}

void MyOpenglWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) {
        camera.ProcessKeyboard('w', 0.1f);
    }
    if (event->key() == Qt::Key_A) {
        camera.ProcessKeyboard('a', 0.1f);
    }
    if (event->key() == Qt::Key_S) {
        camera.ProcessKeyboard('s', 0.1f);
    }
    if (event->key() == Qt::Key_D) {
        camera.ProcessKeyboard('d', 0.1f);
    }
    if (event->key() == Qt::Key_F) {
        // look from front
        camera.lookFrom('f');
    }
    if (event->key() == Qt::Key_T) {
        // look from top
        camera.lookFrom('t');
    }
    if (event->key() == Qt::Key_R) {
        // look from top
        camera.lookFrom('r');
    }
    if (event->key() == Qt::Key_L) {
        // look from top
        camera.lookFrom('l');
    }
    update();
}

