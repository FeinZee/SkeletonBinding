#include "skeletonjoint.h"

SkeletonJoint::SkeletonJoint()
{
    modelMatrix.setToIdentity();
    modelMatrix.translate(0, 0, 1);
}

QMatrix4x4& SkeletonJoint::getModelMatrix(){
    return modelMatrix;
}
