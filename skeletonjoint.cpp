#include "skeletonjoint.h"

SkeletonJoint::SkeletonJoint(QVector3D pos)
{
    worldCordinate = pos;
}

QVector3D& SkeletonJoint::getWorldCordinate(){
    return worldCordinate;
}

QMatrix4x4 SkeletonJoint::getModelMatrix(){
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(worldCordinate);
    return m;
}
