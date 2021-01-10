#ifndef SOLIDSPHERERENDER_H
#define SOLIDSPHERERENDER_H
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "param.h"
/*
 * 此类实现骨架关节（球体）的绘制
 */
class SolidSphereRender
{
public:
    SolidSphereRender() = default;
    void initialize(float r);
    void render(QOpenGLExtraFunctions *f, QMatrix4x4 &projMatrix, QMatrix4x4 modelMatrix, QMatrix4x4 &viewMatrix,
                QVector3D &cameraLocation, QVector3D &lightLocation, QVector4D color);

private:
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vbo;
    QVector<GLfloat> m_points;
    QVector<GLfloat> m_norm_points;
    float m_r;

};

#endif // SOLIDSPHERERENDER_H
