#include "skeleton.h"

Skeleton::Skeleton()
{

}

bool Skeleton::rootJointDetermined(){
    if (rootJointId == -1) {
        return false;
    }else{
        return true;
    }
}

bool Skeleton::isEmpty() {
    return (jointCount == 0);
}

void Skeleton::addJointList(QList<SkeletonJoint> newJointList, int rootJointIdOfNewList){
    QList<int> listID;
    if (rootJointIdOfNewList != -1) {
        // this is not the first joint list of the skeleton
        listID.push_back(rootJointIdOfNewList);
    }

    for(auto j: newJointList) {
        int id = jointCount;
        joints.insert(id, j);
        listID.push_back(id);
        jointCount++;
    }
    jointLists.insert(listID);
    qDebug() << listID;
}

const QHash<int, SkeletonJoint>& Skeleton::getJoints(){
    return joints;
}

const QSet<QList<int>>& Skeleton::getJointLists(){
    return jointLists;
}
