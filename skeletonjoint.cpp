#include "skeletonjoint.h"

SkeletonJoint::SkeletonJoint(QVector3D pos)
{
    worldCoordinate = pos;
}

QVector3D& SkeletonJoint::getWorldCoordinate(){
    return worldCoordinate;
}

void SkeletonJoint::setWorldCoordinate(QVector3D newPos){
    worldCoordinate = newPos;
}

QMatrix4x4 SkeletonJoint::getModelMatrix(){
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(worldCoordinate);
    return m;
}
