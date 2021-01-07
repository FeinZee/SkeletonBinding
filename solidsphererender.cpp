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

            m_points << x1 << y1 << z1 << x3 << y3 << z3
                   << x0 << y0 << z0 << x1 << y1 << z1
                   << x2 << y2 << z2 << x3 << y3 << z3;
        }
    }

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_points.constData(),m_points.count() * sizeof(GLfloat));
}

void SolidSphereRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &projMatrix, QMatrix4x4 &modelMatrix,  QMatrix4x4 &viewMatrix, QVector3D &lightLocation)
{

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);
    m_program.bind();
    m_vbo.bind();
    m_program.setUniformValue("uPMatrix",projMatrix);
    m_program.setUniformValue("uVMatrix",viewMatrix);
    m_program.setUniformValue("uMMatrix",modelMatrix);
    m_program.setUniformValue("uR",m_r);
    m_program.enableAttributeArray(0);

    m_program.setAttributeBuffer(0,GL_FLOAT,0,3,0);
    f->glDrawArrays(GL_TRIANGLES,0,m_points.count() / 3);

    m_program.disableAttributeArray(0);
    m_vbo.release();
    m_program.release();
    f->glDisable(GL_DEPTH_TEST);
    f->glDisable(GL_CULL_FACE);
}
