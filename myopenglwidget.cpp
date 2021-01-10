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
    s_renderer.initialize(JOINT_RADIUS);
    s_selected_renderer.initialize(0.025f);
    bone_renderer.initialize();
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
    QMatrix4x4 mMatrix;
    mMatrix.translate(0, -0.8f);
    obj_renderer.render(f, projMatrix, viewMatrix, mMatrix, camera.Position, lightLocation, false);

    /* about skeleton */
    // draw the skeleton
    const QSet<QList<int>>& jointLists = skeleton.getJointLists();
    QHash<int, SkeletonJoint> joints = skeleton.getJoints();
    SkeletonJoint lastJoint;
    bool isFirst = true;
    for (auto jointList: jointLists) {
        isFirst = true;
        for(int id: jointList) {
            if (!isFirst) {
                bone_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), lastJoint.getWorldCoordinate(), joints[id].getWorldCoordinate(), JOINT_RADIUS, projMatrix, viewMatrix, camera.Position);
            }
            s_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, joints[id].getModelMatrix(), viewMatrix,
                              camera.Position, lightLocation, QVector4D(0.6f, 0, 0, 0));
            lastJoint = joints[id];
            isFirst = false;
        }

    }
    // draw the new joint list

    isFirst = true;
    if (rootJointOfNewList != -1) lastJoint = joints[rootJointOfNewList];
    for (auto joint: newJointList) {
        if ((isFirst && rootJointOfNewList != -1) || !isFirst) {
            bone_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), lastJoint.getWorldCoordinate(), joint.getWorldCoordinate(), JOINT_RADIUS, projMatrix, viewMatrix, camera.Position);
        }
        s_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, joint.getModelMatrix(), viewMatrix,
                          camera.Position, lightLocation, QVector4D(0.6f, 0, 0, 0));
        lastJoint = joint;
        isFirst = false;
    }
    // draw the selected joint
    if (aJointIsSelected) {
        s_selected_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, selectedJoint.getModelMatrix(), viewMatrix,
                                   camera.Position, lightLocation, QVector4D(0.6f, 0.6f, 0, 0));
    }
    // draw the current bone
    if (mode == CREATE_MODE && aJointIsSelected) {
        QVector3D worldPosition = QVector3D(lastMouseX, height - lastMouseY, 0.996).unproject(camera.GetViewMatrix(), projMatrix, QRect(0, 0, width, height));
        bone_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), selectedJoint.getWorldCoordinate(), worldPosition, JOINT_RADIUS, projMatrix, viewMatrix, camera.Position);
    }
}


void MyOpenglWidget::mousePressEvent(QMouseEvent *event) {
    lastMouseX = event->x();
    lastMouseY = event->y();
    if (event->button() == Qt::RightButton) {
        /* here remains a question: how to get the z?
         * For now, I get z by testing the screen position of a sphere at world cordinate - (1, 0, 0).
         */
        if (mode == CREATE_MODE) {
            if (aJointIsSelected || skeleton.isEmpty()) {
                float z = 0.996;
                QVector3D worldPosition = QVector3D(event->x(), height - event->y(), z).unproject(camera.GetViewMatrix(), projMatrix, QRect(0, 0, width, height));
                SkeletonJoint newJoint = SkeletonJoint(worldPosition);
                newJointList.push_back(newJoint);
                aJointIsSelected = true;
                selectedJoint = newJoint;
                setMouseTracking(true);

                update();
            }else {
                qDebug() << "先选中一个已存在关节，再继续创建。";
            }
        }
    }else if (event->button() == Qt::LeftButton){
        lastPressMouseX = event->x();
        lastPressMouseY = event->y();
        isLeftMousePressed = true;
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
    if (isLeftMousePressed) {
        // camera movement
        int offsetX = event->x() - lastMouseX;
        int offsetY = event->y() - lastMouseY;
        camera.ProcessMouseMovement(offsetX, offsetY, true);
        lastMouseX = event->x();
        lastMouseY = event->y();
        update();
    }
    if (aJointIsSelected && mode == CREATE_MODE) {
        // bone drawing

        lastMouseX = event->x();
        lastMouseY = event->y();

        update();
    }
}



void MyOpenglWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isLeftMousePressed = false;
        if (lastPressMouseX == event->x() && lastPressMouseY == event->y()) {
            // test if pick a joint from the skeleton
            bool PickSuccess = false;

            QHash<int, SkeletonJoint> joints = skeleton.getJoints();
            QHashIterator<int, SkeletonJoint> i(joints);
            while (i.hasNext()) {
                i.next();
                int id = i.key();
                SkeletonJoint sj = i.value();
                QVector3D screenCoord = getScreenCoord(sj.getWorldCoordinate());
                if (distance(int(screenCoord.x()), int(screenCoord.y()), event->x(), event->y()) < 10) {
                    aJointIsSelected = true;
                    PickSuccess = true;
                    rootJointOfNewList = id;
                    setMouseTracking(true);
                    selectedJoint = sj;
                    break;
                }
            }

            if (!PickSuccess && aJointIsSelected) {
                aJointIsSelected = false;
                if (newJointList.count() > 0) {
                    qDebug() << "add new list with root = " << rootJointOfNewList << "count = " << newJointList.count();
                    skeleton.addJointList(newJointList, rootJointOfNewList);
                    newJointList.clear();
                    rootJointOfNewList = -1;
                    setMouseTracking(false);
                }
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
    if (event->key() == Qt::Key_Q) {
        aJointIsSelected = false;
        setMouseTracking(false);
        qDebug() << "add new list with root = " << rootJointOfNewList << "count = " << newJointList.count();
        skeleton.addJointList(newJointList, rootJointOfNewList);
        newJointList.clear();
        rootJointOfNewList = -1;
    }
    update();
}

