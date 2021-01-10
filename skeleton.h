#ifndef SKELETON_H
#define SKELETON_H
#include <QHash>
#include "skeletonjoint.h"
class Skeleton
{
public:
    Skeleton();
    /*所有新加入骨架的关节链，必须起头于骨架中已有的一个关节(除了第一条关节链外)*/
    void addJointList(QList<SkeletonJoint>, int);
    bool rootJointDetermined();
    const QSet<QList<int>>& getJointLists();
    const QHash<int, SkeletonJoint>& getJoints();
    bool isEmpty();
private:
    int jointCount = 0;
    int rootJointId = -1;
    QHash<int, SkeletonJoint> joints;
    QSet<QList<int>> jointLists;
};

#endif // SKELETON_H
