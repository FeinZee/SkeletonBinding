#ifndef SKELETONJOINT_H
#define SKELETONJOINT_H

#include <QMatrix4x4>

/*
 * 此类实现骨架的关节的逻辑
 */
class SkeletonJoint
{
public:
    SkeletonJoint(){}
    SkeletonJoint(QVector3D);

    QVector3D& getWorldCoordinate();
    void setWorldCoordinate(QVector3D);
    QMatrix4x4 getModelMatrix();
private:
    // 世界坐标
    QVector3D worldCoordinate;
};

#endif // SKELETONJOINT_H
