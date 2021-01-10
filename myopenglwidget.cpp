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
    } else if (!event->angleDelta().isNull()) {
        camera.ProcessMouseScroll((event->angleDelta()/120).y());
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
    obj_renderer.render(f, projMatrix, viewMatrix, mMatrix, camera.Position, lightLocation, modelWire);

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
    // draw the root joint
    if (rootJointOfAll != -1) {
        s_selected_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, joints[rootJointOfAll].getModelMatrix(), viewMatrix,
                                   camera.Position, lightLocation, QVector4D(0.0f, 0.6f, 0.0f, 0));
    }
    // draw the selected joint
    if (aJointIsSelected) {
        s_selected_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), projMatrix, selectedJoint.getModelMatrix(), viewMatrix,
                                   camera.Position, lightLocation, QVector4D(0.6f, 0.6f, 0, 0));
    }

    // draw the current bone
    if (mode == CREATE_MODE && aJointIsSelected) {
        QVector3D worldPosition = QVector3D(lastMouseX, height - lastMouseY, 0.9968).unproject(camera.GetViewMatrix(), projMatrix, QRect(0, 0, width, height));
        bone_renderer.render(QOpenGLContext::currentContext()->extraFunctions(), selectedJoint.getWorldCoordinate(), worldPosition, JOINT_RADIUS, projMatrix, viewMatrix, camera.Position);
    }
}


void MyOpenglWidget::mousePressEvent(QMouseEvent *event) {
    lastMouseX = event->x();
    lastMouseY = event->y();
    if (event->button() == Qt::RightButton) {
        /* 这里z值的选择曾尝试用glu获取失败，只能尝试出比较合适的z值，使得生成的点在世界坐标 z=0 附近
         */
        if (mode == CREATE_MODE) {
            if (aJointIsSelected || skeleton.isEmpty()) {
                float z = 0.9968;
                QVector3D worldPosition = QVector3D(event->x(), height - event->y(), z).unproject(camera.GetViewMatrix(), projMatrix, QRect(0, 0, width, height));
                SkeletonJoint newJoint = SkeletonJoint(worldPosition);
                newJointList.push_back(newJoint);
                aJointIsSelected = true;
                selectedJoint = newJoint;
                emit setWorldCoord(selectedJoint.getWorldCoordinate());
                setMouseTracking(true);

                update();
            }else {
                emit hint("先选中一个已存在关节，再继续创建。");
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
            // 检测是否左键选中了某个关节
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
                    if (mode == CREATE_MODE) {
                        setMouseTracking(true);
                    }
                    selectedJoint = sj;
                    selectedJointId = id;
                    emit setWorldCoord(selectedJoint.getWorldCoordinate());
                    break;
                }
            }
            // 如果是左键点在其他位置，就停止当前关节链的创建，并加入骨架
            if (!PickSuccess && aJointIsSelected) {
                aJointIsSelected = false;
                selectedJointId = -1;
                emit clearLineEdit();
                if (newJointList.count() > 0) {
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
        emit clearLineEdit();
        setMouseTracking(false);
        skeleton.addJointList(newJointList, rootJointOfNewList);
        newJointList.clear();
        rootJointOfNewList = -1;
    }
    update();
}

/* slots */
void MyOpenglWidget::modeChanged(int modeIdx)
{
    mode = modeIdx;
    if (mode == CREATE_MODE) {
        emit LineEditDisable(true);
    }else if (mode == SELECT_MODE) {
        emit LineEditDisable(false);
    }else if (mode == MOVE_MODE) {
        emit LineEditDisable(false);
    }
}

void MyOpenglWidget::CoordinateChanged(float x, float y, float z)
{
    if (aJointIsSelected) {
        if (mode == SELECT_MODE) {
            if (selectedJointId == rootJointOfAll) {
                // all joints move
                QVector3D oldPos = skeleton.joints[rootJointOfAll].getWorldCoordinate();
                float offsetX = x - oldPos.x();
                float offsetY = y - oldPos.y();
                float offsetZ = z - oldPos.z();
                for (int i = 0; i < skeleton.joints.count(); ++i) {
                    skeleton.joints[i].setWorldCoordinate(skeleton.joints[i].getWorldCoordinate() + QVector3D(offsetX, offsetY, offsetZ));
                }
                selectedJoint.setWorldCoordinate(QVector3D(x, y, z));
            }else{
                skeleton.joints[selectedJointId].setWorldCoordinate(QVector3D(x, y, z));
                selectedJoint.setWorldCoordinate(QVector3D(x, y, z));
            }
        }else if (mode == MOVE_MODE) {
            QVector3D oldPos = skeleton.joints[selectedJointId].getWorldCoordinate();
            float offsetX = x - oldPos.x();
            float offsetY = y - oldPos.y();
            float offsetZ = z - oldPos.z();
            skeleton.joints[selectedJointId].setWorldCoordinate(QVector3D(x, y, z));
            selectedJoint.setWorldCoordinate(QVector3D(x, y, z));
            skeletonBind.move(selectedJointId, QVector3D(offsetX, offsetY, offsetZ));
            obj_renderer.resetVertices(skeletonBind.getVertices());
        }
        update();
    }else{
        emit hint("请先选中一个关节");
    }
}

void MyOpenglWidget::modelWireMode(bool result)
{
    modelWire = result;
    update();
}

void MyOpenglWidget::setRootJoint() {
    if (mode != SELECT_MODE) {
        mode = SELECT_MODE;
        emit LineEditDisable(false);
        emit modeComboBoxShouldChange(SELECT_MODE);
    }
    if (aJointIsSelected){
        rootJointOfAll = selectedJointId;
        aJointIsSelected = false;
        selectedJointId = -1;
        emit clearLineEdit();
        update();
    }else{
        emit hint("请先选中一个关节");
    }
}

void MyOpenglWidget::bindSkeleton() {
    skeletonBind.beginBinding(skeleton, obj_renderer.getVertices());
    emit hint("绑定完成");
    emit modeComboBoxShouldChange(MOVE_MODE);
}

