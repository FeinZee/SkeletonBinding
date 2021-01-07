#ifndef SKELETONJOINT_H
#define SKELETONJOINT_H

#include <QMatrix4x4>

class SkeletonJoint
{
public:
    SkeletonJoint();

    QMatrix4x4& getModelMatrix();
private:
    // 世界坐标
    QMatrix4x4 modelMatrix;
};

#endif // SKELETONJOINT_H
