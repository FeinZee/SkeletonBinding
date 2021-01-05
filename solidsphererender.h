#ifndef SOLIDSPHERERENDER_H
#define SOLIDSPHERERENDER_H
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <math.h>
#define PI 3.14159265f

class SolidSphereRender
{
public:
    SolidSphereRender() = default;
    void initialize(float r);
    void render(QOpenGLExtraFunctions *f,QMatrix4x4 &posMatrix, QMatrix4x4 &model, QMatrix4x4 &viewMatrix, QVector3D &lightLocation);

private:
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vbo;
    QVector<GLfloat> m_points;
    float m_r;

};

#endif // SOLIDSPHERERENDER_H
