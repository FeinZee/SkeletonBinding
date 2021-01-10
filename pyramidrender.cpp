#include "pyramidrender.h"



void PyramidRender::initialize(){
    m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex,":/shader/wire.vert");
    m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment,":/shader/wire.frag");
    m_program.link();

    m_vbo.create();

}

void PyramidRender::render(QOpenGLExtraFunctions *f, QVector3D startPoint, QVector3D endPoint, float radius,
                           QMatrix4x4 &projMatrix, QMatrix4x4 &viewMatrix, QVector3D &cameraLocation){

    QVector<float> points;
    QVector3D startToEnd(endPoint - startPoint);
    float a = startToEnd.x();
    float b = startToEnd.y();
    float c = startToEnd.z();
    float rcb = radius * c * b;
    float a2b2 = a*a + b*b;
    float p = rcb * rcb + 3 * a2b2 * a * a * radius * radius;
    p = sqrt(p);
    float z1 = 0.5f * radius;
    float z2 = -z1;
    float y1 = (-rcb + p)/ (2 * a2b2);
    float x1 = (- c * z1 - b * y1) / a;
    float y2 = (-rcb - p)/ (2 * a2b2);
    float x2 = (- c * z2 - b * y2) / a;

    QVector3D tmp(x1, y1, z1);
    QVector3D norm = QVector3D::crossProduct(tmp, startToEnd);
    norm.normalize();
    norm *= radius;
    points << x1 + startPoint.x() << y1 + startPoint.y() << z1 + startPoint.z() << norm.x() + startPoint.x() << norm.y() + startPoint.y() << norm.z() + startPoint.z() << endPoint.x() << endPoint.y() << endPoint.z();
    points << norm.x() + startPoint.x() << norm.y() + startPoint.y() << norm.z() + startPoint.z() << x2 + startPoint.x() << y2 + startPoint.y() << z2 + startPoint.z() << endPoint.x() << endPoint.y() << endPoint.z();

    points << x2 + startPoint.x() << y2 + startPoint.y() << z2 + startPoint.z() << startPoint.x() - norm.x() << startPoint.y()- norm.y() << startPoint.z() - norm.z() << endPoint.x() << endPoint.y() << endPoint.z();
    points << startPoint.x() - norm.x() << startPoint.y()- norm.y() << startPoint.z() - norm.z() << x1 + startPoint.x() << y1 + startPoint.y() << z1 + startPoint.z() << endPoint.x() << endPoint.y() << endPoint.z();

    m_vbo.bind();
    m_vbo.allocate(points.constData(), points.count() * sizeof(GLfloat));
    glEnable(GL_CULL_FACE);
    m_program.bind();
    m_vbo.bind();
    m_program.setUniformValue("uPMatrix", projMatrix);
    m_program.setUniformValue("uVMatrix", viewMatrix);
    m_program.setUniformValue("uCamera", cameraLocation);

    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
    f->glDrawArrays(GL_TRIANGLES, 0, points.count() / 3);

    m_program.disableAttributeArray(0);
    m_vbo.release();
    m_program.release();
    glDisable(GL_CULL_FACE);
}
