#ifndef SKELETONJOINT_H
#define SKELETONJOINT_H

#include <QMatrix4x4>

class SkeletonJoint
{
public:
    SkeletonJoint(QVector3D);

    QVector3D& getWorldCordinate();
    QMatrix4x4 getModelMatrix();
private:
    // 世界坐标
    QVector3D worldCordinate;
};

#endif // SKELETONJOINT_H
