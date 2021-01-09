#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
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

protected:
    void initializeGL() override;
    void resizeGL(int w,int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    int width, height;
    int lastPressMouseX, lastPressMouseY;
    int lastMouseX, lastMouseY;
    bool isMousePressed = false;
    /* about scene */
    Camera camera = Camera(QVector3D(0, 0, 3));
    QVector3D lightLocation;
    QMatrix4x4 projMatrix;

    /* about model object */
    GenericRender obj_renderer;

    qreal obj_angleX, obj_angleY, obj_angleZ;

    /* about skeleton */
    std::vector<SkeletonJoint> joints;
    SolidSphereRender s_renderer; //球渲染器

    QVector3D pos;

};

#endif // MYOPENGLWIDGET_H
