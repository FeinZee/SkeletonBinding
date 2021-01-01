#ifndef GENERICRENDER_H
#define GENERICRENDER_H
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QString>
#include <QImage>

class GenericRender
{
public:
    GenericRender() = default;
    void initsize(QString filename, const QImage &textureImg);
    void render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix,
                QMatrix4x4 &mMatrix, QVector3D &cameraLocation, QVector3D &lightLocation);
private:
    QOpenGLTexture *m_texture{nullptr};
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram m_program;
    QVector<float> m_vertPoints, m_texturePoints, m_normalPoints;

};

#endif // GENERICRENDER_H
