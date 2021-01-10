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
    void findNearestBone(QVector3D point, int& id1, int& id2, float& weight);
    float pointToSegDistance(QVector3D& p, const QVector3D& p1, const QVector3D& p2, float& weight);
    bool isEmpty();
private:
    int jointCount = 0;
    int rootJointId = -1;
    QSet<QList<int>> jointLists;
public:
    QHash<int, SkeletonJoint> joints;
};

#endif // SKELETON_H
