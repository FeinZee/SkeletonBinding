#include "objloader.h"
#include <QFile>
#include <QDebug>

ObjLoader::ObjLoader()
{

}
bool ObjLoader::load(QString fileName, QVector<float> &vPoints,QVector<float> &tPoints,QVector<float> &nPoints)
{
    if(fileName.mid(fileName.lastIndexOf('.')) != ".obj"){
        qDebug() << "file is not a obj file";
        return false;
    }

    QFile objFile(fileName);
    if(!objFile.open(QIODevice::ReadOnly)){
        qDebug() << "open" << fileName << "failed";
        return false;
    }

    QVector<float> vertextPoints,texturePoints,normalPoints;
    QVector<std::tuple<int,int,int>> facesIndexs;

    while (!objFile.atEnd()) {
        QByteArray lineData = objFile.readLine();
        lineData = lineData.remove(lineData.count() - 2,2);
        if(lineData.isEmpty()){

            for(auto &verFaceInfo : facesIndexs){
                int vIndex = std::get<0>(verFaceInfo);
                int tIndex = std::get<1>(verFaceInfo);
                int nIndex = std::get<2>(verFaceInfo);

                int vPointSizes = vertextPoints.count() / 3;
                int tPointSizes = texturePoints.count() / 2;
                int nPointSizes = normalPoints.count() / 3;
                if (vIndex > 0) {
                    // 如果f的索引是正数
                    vIndex--;
                    tIndex--;
                    nIndex--;
                    vPointSizes = 0;
                    tPointSizes = 0;
                    nPointSizes = 0;
                }
                vPoints << vertextPoints.at((vPointSizes + vIndex) * 3);
                vPoints << vertextPoints.at((vPointSizes + vIndex) * 3 + 1);
                vPoints << vertextPoints.at((vPointSizes + vIndex) * 3 + 2);

                tPoints << texturePoints.at((tPointSizes + tIndex) * 2);
                tPoints << texturePoints.at((tPointSizes + tIndex) * 2 + 1);

                nPoints << normalPoints.at((nPointSizes + nIndex) * 3);
                nPoints << normalPoints.at((nPointSizes + nIndex) * 3 + 1);
                nPoints << normalPoints.at((nPointSizes + nIndex) * 3 + 2);
            }
            vertextPoints.clear();
            texturePoints.clear();
            normalPoints.clear();
            facesIndexs.clear();
            continue;
        }

        QList<QByteArray> strValues = lineData.split(' ');
        QString dataType = strValues.takeFirst();
        if(dataType == "v"){
            std::transform(strValues.begin(),strValues.end(),std::back_inserter(vertextPoints),[](QByteArray &str){
                return str.toFloat();
            });
        }else if(dataType == "vt"){
            std::transform(strValues.begin(),strValues.end(),std::back_inserter(texturePoints),[](QByteArray &str){
                return str.toFloat();
            });
        }else if(dataType == "vn"){
            std::transform(strValues.begin(),strValues.end(),std::back_inserter(normalPoints),[](QByteArray &str){
                return str.toFloat();
            });
        }else if(dataType == "f"){
            // 这里原本写的是到strValues.end(), 但有的obj文件一个面有4个点, 此处没实现
            std::transform(strValues.begin(),strValues.begin() + 3,std::back_inserter(facesIndexs),[](QByteArray &str){
                QList<QByteArray> intStr = str.split('/');

                return std::make_tuple(intStr.first().toInt(),intStr.at(1).toInt(),intStr.last().toInt());
            });
        }
    }

    objFile.close();
    return true;
}
