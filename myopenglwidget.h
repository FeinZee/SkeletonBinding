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
#include "skeletonjoint.h"
#include "genericrender.h"
#include "solidsphererender.h"


class MyOpenglWidget: public QOpenGLWidget, protected QOpenGLFunctions{
    Q_OBJECT
public:
    MyOpenglWidget(QWidget *parent = 0);

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
    bool isMousePressed = false;
    int mode;

    /* about scene */
    Camera camera = Camera(QVector3D(0, 0, 3));
    QVector3D lightLocation;
    QMatrix4x4 projMatrix;

    /* about model object */
    GenericRender obj_renderer;

    /* about skeleton */
    bool aJointIsSelected = false;
    SkeletonJoint selectedJoint;
    std::vector<SkeletonJoint> joints;
    SolidSphereRender s_renderer; // renderer for joints
    SolidSphereRender s_selected_renderer; // renderer for the selected joint

    QVector3D pos;

};

#endif // MYOPENGLWIDGET_H
