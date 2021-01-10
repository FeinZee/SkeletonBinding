#ifndef SKELETONBINDING_H
#define SKELETONBINDING_H
#include <QHash>
#include "skeleton.h"

/*
 * 此类是骨架绑定的算法实现类
 * 构造时需要：一个骨架 & 顶点
 */
class SkeletonBinding
{
public:
    SkeletonBinding() = default;
    void beginBinding(Skeleton &s, const QVector<float> &points);
    void move(int jointIdx, QVector3D offset);
    const QHash<int, QVector<QPair<int, float>>>& getWeigths();
    const QVector<float>& getVertices();
private:
    /* QHash<JointIdx, QVector<Qpair<VerticesIdx, weight>>> */
    QHash<int, QVector<QPair<int, float>>> weights;
    QVector<float> m_vertices;
};

#endif // SKELETONBINDING_H
