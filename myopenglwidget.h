#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QVector4D>
#include <GL/glu.h>
#include <GL/gl.h>
#include <QTimer>
#include "camera.h"
#include "skeleton.h"
#include "skeletonjoint.h"
#include "genericrender.h"
#include "pyramidrender.h"
#include "skeletonbinding.h"
#include "solidsphererender.h"


#define CREATE_MODE 0
#define SELECT_MODE 1
#define MOVE_MODE 2


class MyOpenglWidget: public QOpenGLWidget, protected QOpenGLFunctions{
    Q_OBJECT
public:
    MyOpenglWidget(QWidget *parent = 0);

public slots:
    void modeChanged(int);
    void CoordinateChanged(float, float, float);
    void modelWireMode(bool);
    void setRootJoint();
    void bindSkeleton();

signals:
    void LineEditDisable(bool);
    void setWorldCoord(QVector3D);
    void modeComboBoxShouldChange(int);
    void hint(QString);
    void clearLineEdit();

private:
    void drawSkeletons();
    QVector3D getScreenCoord(QVector3D pos);
    float distance(int x1, int y1, int x2, int y2);

protected:
    void initializeGL() override;
    void resizeGL(int w,int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    int width, height;
    int lastPressMouseX, lastPressMouseY;
    int lastMouseX, lastMouseY;
    bool isLeftMousePressed = false;
    int mode = CREATE_MODE;

    /* about scene */
    Camera camera = Camera(QVector3D(0, 0.8, 3));
    QVector3D lightLocation;
    QMatrix4x4 projMatrix;

    /* about model object */
    GenericRender obj_renderer;
    bool modelWire = true;

    /* about skeleton */
    bool aJointIsSelected = false;
    SkeletonJoint selectedJoint;
    int selectedJointId;
    Skeleton skeleton;
    QList<SkeletonJoint> newJointList;
    SolidSphereRender s_renderer; // renderer for joints
    SolidSphereRender s_selected_renderer; // renderer for the selected joint
    PyramidRender bone_renderer;
    int rootJointOfNewList = -1;
    int rootJointOfAll = -1;

    SkeletonBinding skeletonBind;
};

#endif // MYOPENGLWIDGET_H
