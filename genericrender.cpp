#include "genericrender.h"
#include "objloader.h"
#include <QCoreApplication>
#include <string>
#include <stdio.h>
void GenericRender::initsize(QString filename, const QImage &textureImg){

    m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vsrc.vert");
    m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fsrc.frag");
    m_program.link();

    ObjLoader objModelLoader;
    objModelLoader.load(filename, m_vertPoints, m_texturePoints, m_normalPoints);
    QVector<float> points;
    points << m_vertPoints << m_texturePoints << m_normalPoints;
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(points.data(), points.count() * sizeof (float));

    m_texture = new QOpenGLTexture(textureImg);
    m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    m_texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
}
void GenericRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix,
                           QMatrix4x4 &mMatrix, QVector3D &cameraLocation, QVector3D &lightLocation){
    f->glEnable(GL_DEPTH_TEST);
    m_program.bind();
    m_vbo.bind();
    f->glActiveTexture(GL_TEXTURE0 + 0);
    m_program.setUniformValue("uPMatrix",pMatrix);
    m_program.setUniformValue("uVMatrix",vMatrix);
    m_program.setUniformValue("uMMatrix",mMatrix);
    m_program.setUniformValue("uLightLocation",lightLocation);
    m_program.setUniformValue("uCamera",cameraLocation);
    m_program.setUniformValue("sTexture",0);

    m_program.enableAttributeArray(0);
    m_program.enableAttributeArray(1);
    m_program.enableAttributeArray(2);
    m_program.setAttributeBuffer(0,GL_FLOAT,0,3,3*sizeof(GLfloat));
    m_program.setAttributeBuffer(1,GL_FLOAT,m_vertPoints.count() * sizeof(GLfloat),2,2*sizeof(GLfloat));
    m_program.setAttributeBuffer(2,GL_FLOAT,(m_vertPoints.count() + m_texturePoints.count()) * sizeof(GLfloat),3,3*sizeof(GLfloat));
    m_texture->bind(0);
    f->glDrawArrays(GL_TRIANGLES,0,m_vertPoints.count()/3);

    m_program.disableAttributeArray(0);
    m_program.disableAttributeArray(1);
    m_program.disableAttributeArray(2);
    m_texture->release();
    m_vbo.release();
    m_program.release();
    f->glDisable(GL_DEPTH_TEST);
}
