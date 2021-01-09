#include "solidsphererender.h"

void SolidSphereRender::initialize(float r)
{
    m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex,":/shader/sphere.vsh");
    m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment,":/shader/sphere.fsh");
    m_program.link();

    m_r = r;
    int angleSpan = 10; //弧度 = 角度 * PI / 180
    float toRadius = PI / 180;
    for(int vAngle = -90; vAngle < 90; vAngle = vAngle + angleSpan){ //生成球面顶点
        for(int hAngle = 0; hAngle <= 360; hAngle = hAngle + angleSpan){
            float x0 = r * cos(vAngle * toRadius) * cos(hAngle * toRadius);
            float y0 = r * cos(vAngle * toRadius) * sin(hAngle * toRadius);
            float z0 = r * sin(vAngle * toRadius);

            float x1 = r * cos(vAngle * toRadius) * cos((hAngle + angleSpan) * toRadius);
            float y1 = r * cos(vAngle * toRadius) * sin((hAngle + angleSpan) * toRadius);
            float z1 = r * sin(vAngle * toRadius);

            float x2 = r * cos((vAngle + angleSpan) * toRadius) * cos((hAngle + angleSpan) * toRadius);
            float y2 = r * cos((vAngle + angleSpan) * toRadius) * sin((hAngle + angleSpan) * toRadius);
            float z2 = r * sin((vAngle + angleSpan) * toRadius);

            float x3 = r * ::cos((vAngle + angleSpan) * toRadius) * ::cos(hAngle * toRadius);
            float y3 = r * ::cos((vAngle + angleSpan) * toRadius) * ::sin(hAngle * toRadius);
            float z3 = r * ::sin((vAngle + angleSpan) * toRadius);

            QVector3D v3v1(x1-x3, y1-y3, z1-z3);
            QVector3D v3v0(x0-x3, y0-y3, z0-z3);
            QVector3D norm1 = QVector3D::crossProduct(v3v1, v3v0);

            QVector3D v2v1(x1-x2, y1-y2, z1-z2);
            QVector3D v2v3(x3-x2, y3-y2, z3-z2);
            QVector3D norm2 = QVector3D::crossProduct(v2v1, v2v3);

            m_points << x1 << y1 << z1 << x3 << y3 << z3 << x0 << y0 << z0
                     << x1 << y1 << z1 << x2 << y2 << z2 << x3 << y3 << z3;

            m_norm_points << norm1.x() << norm1.y() << norm1.z() << norm1.x() << norm1.y() << norm1.z() << norm1.x() << norm1.y() << norm1.z()
                          << norm2.x() << norm2.y() << norm2.z() << norm2.x() << norm2.y() << norm2.z() << norm2.x() << norm2.y() << norm2.z();
        }
    }
    QVector<float> points;
    points << m_points << m_norm_points;
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(points.constData(), points.count() * sizeof(GLfloat));
}

void SolidSphereRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &projMatrix, QMatrix4x4 modelMatrix,  QMatrix4x4 &viewMatrix,
                                QVector3D &cameraLocation, QVector3D &lightLocation, QVector4D color)
{
    glEnable(GL_CULL_FACE);
    m_program.bind();
    m_vbo.bind();
    m_program.setUniformValue("uPMatrix", projMatrix);
    m_program.setUniformValue("uVMatrix", viewMatrix);
    m_program.setUniformValue("uMMatrix", modelMatrix);
    m_program.setUniformValue("uLightLocation", lightLocation);
    m_program.setUniformValue("uCamera", cameraLocation);
    m_program.setUniformValue("uColor", color);

    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_program.enableAttributeArray(1);
    m_program.setAttributeBuffer(1, GL_FLOAT, m_points.count() * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    glPolygonMode(GL_FRONT_AND_BACK ,GL_FILL);
    f->glDrawArrays(GL_TRIANGLES, 0, m_points.count() / 3);

    m_program.disableAttributeArray(0);
    m_vbo.release();
    m_program.release();
    glDisable(GL_CULL_FACE);
}
