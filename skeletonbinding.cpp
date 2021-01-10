#include "skeletonbinding.h"

// 采用的算法即为寻找距离某点最近的骨骼，根据其投影到骨骼上的投影点的位置，决定它对于骨骼两个端点关节的权重
void SkeletonBinding::beginBinding(Skeleton &s, const QVector<float> &vertices)
{
    m_vertices = vertices;
    int pointsCount = int(vertices.count() / 3);
    for (int index = 0; index < pointsCount; ++index) {
        QVector3D point = QVector3D(vertices[index*3], vertices[index*3 + 1], vertices[index*3 + 2]);
        int id1, id2;
        float weight;
        s.findNearestBone(point, id1, id2, weight);
        if (weight != 0.0f ) weights[id1].append(QPair<int, float>(index, weight));
        if (weight != 1.0f ) weights[id2].append(QPair<int, float>(index, 1 - weight));
    }
}

void SkeletonBinding::move(int jointIdx, QVector3D offset) {
    QVector<QPair<int, float>> affectedVertexIdx = weights[jointIdx];
    for (auto pair: affectedVertexIdx) {
        int idx = pair.first;
        float weight = pair.second;
        m_vertices[idx * 3] += offset.x() * weight;
        m_vertices[idx * 3 + 1] += offset.y() * weight;
        m_vertices[idx * 3 + 2] += offset.z() * weight;
    }
}

const QHash<int, QVector<QPair<int, float>>>& SkeletonBinding::getWeigths() {
    return weights;
}

const QVector<float>& SkeletonBinding::getVertices(){
    return m_vertices;
}
