#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <QString>

/*
 * 参考来源：
 * https://gitlab.com/gitHubwhl562916378/objload
 */
class ObjLoader
{
public:
    ObjLoader();
    bool load(QString fileName, QVector<float> &vPoints,QVector<float> &tPoints,QVector<float> &nPoints);
};

#endif // OBJLOADER_H
