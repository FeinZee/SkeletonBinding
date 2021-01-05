#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "genericrender.h"
#include "solidsphererender.h"

class MyOpenglWidget: public QOpenGLWidget{
    Q_OBJECT
public:
    MyOpenglWidget(QWidget *parent = 0);

protected:
    void initializeGL() override;
    void resizeGL(int w,int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    /* about scene */
    QVector3D cameraLocation, lightLocation, targetLocation;

    /* about model object */
    GenericRender obj_renderer;
    QMatrix4x4 obj_projMatrix;
    qreal obj_angleX, obj_angleY, obj_angleZ;

    /* about skeleton */
    SolidSphereRender s_renderer; //球渲染器
    QMatrix4x4 s_projMatrix,s_modelMatrix; //投影矩阵、基本转换矩阵
private slots:
    void slotTimeout();

};

#endif // MYOPENGLWIDGET_H
