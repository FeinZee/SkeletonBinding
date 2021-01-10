#ifndef PYRAMIDRENDER_H
#define PYRAMIDRENDER_H
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "param.h"

/*
 * 此类绘制了骨头（立体锥）
 */
class PyramidRender
{
public:
    PyramidRender() = default;
    void initialize();
    void render(QOpenGLExtraFunctions *f, QVector3D startPoint, QVector3D endPoint, float radius,
                QMatrix4x4 &projMatrix, QMatrix4x4 &viewMatrix, QVector3D &cameraLocation);
private:
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vbo;
};

#endif // PYRAMIDRENDER_H
