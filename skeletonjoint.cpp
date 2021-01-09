#include "skeletonjoint.h"

SkeletonJoint::SkeletonJoint(QVector3D pos)
{
    worldCoordinate = pos;
}

QVector3D& SkeletonJoint::getWorldCoordinate(){
    return worldCoordinate;
}

QMatrix4x4 SkeletonJoint::getModelMatrix(){
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(worldCoordinate);
    return m;
}
