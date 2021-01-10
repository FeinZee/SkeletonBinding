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
}

void Skeleton::findNearestBone(QVector3D point, int& id1, int& id2, float& weight){
    float minDist = 100;
    float w;
    for (auto jointList : jointLists) {
        int lastPointId = -1;
        for (auto jIdx: jointList) {
            if (lastPointId != -1) {
                QVector3D p1 = joints[lastPointId].getWorldCoordinate();
                QVector3D p2 = joints[jIdx].getWorldCoordinate();
                float dist = pointToSegDistance(point, p1, p2, w);
                if (dist < minDist) {
                    minDist = dist;
                    id1 = lastPointId;
                    id2 = jIdx;
                    weight = w;
                }
            }
            lastPointId = jIdx;
        }
    }
}

float Skeleton::pointToSegDistance(QVector3D& p, const QVector3D& p1, const QVector3D& p2, float& weight) {
    QVector3D p2p1 = p1 - p2;
    QVector3D pp1 = p1 - p;
    float cross = QVector3D::dotProduct(p2p1, pp1);
    if (cross <= 0) {
        weight = 1.0f;
        return p.distanceToPoint(p1);
    }
    float d2 = QVector3D::dotProduct(p2p1, p2p1);

    if (cross >= d2) {
        weight = 0;
        return p.distanceToPoint(p2);
    }


    weight = cross / d2;
    QVector3D projP = p1 + (p2-p1) * weight;
    weight = 1 - weight;
    return p.distanceToPoint(projP);
}

const QHash<int, SkeletonJoint>& Skeleton::getJoints(){
    return joints;
}

const QSet<QList<int>>& Skeleton::getJointLists(){
    return jointLists;
}
