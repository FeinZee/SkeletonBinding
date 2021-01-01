#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QTimer>
#include "genericrender.h"

class MyOpenglWidget: public QOpenGLWidget{
    Q_OBJECT
public:
    MyOpenglWidget(QWidget *parent = 0);

protected:
    void initializeGL() override;
    void resizeGL(int w,int h) override;
    void paintGL() override;

private:
    GenericRender m_render;
    QVector3D m_cameraLocation,m_lightLocation;
    QMatrix4x4 m_pMatrix;
    qreal m_angleX, m_angleY, m_anglZ;
    QTimer m_tm;
private slots:
    void slotTimeout();

};

#endif // MYOPENGLWIDGET_H
