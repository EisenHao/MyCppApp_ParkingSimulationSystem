#ifndef SENSORCLASS_H
#define SENSORCLASS_H
#include "qglobal.h"
#include "qgraphicsscene.h"
#include "qgraphicsitem.h"
#include "qpen.h"

/**
 * 传感器类
*/

class sensorClass{
private:
    QGraphicsEllipseItem* sensorItem;//传感器绘图 图形项指针
    QGraphicsScene *scene;//场景指针
    QColor colorStatus;
    qint32 radius;//绘图传感器圆 -- 半径
    qint32 X0;//X0坐标
    qint32 Y0;//Y0坐标
    bool isArrived;//是否有汽车抵达传感器位置
    bool isPassedBy;//汽车是否走过
public:
    //构造函数1
    sensorClass();
    //构造函数2
    sensorClass(qint32 X0, qint32 Y0, qint32 carWidth, QGraphicsScene *scene);
    //设置有汽车来
    void somethingArrived();
    //设置已经通过
    void somethingPassedBy();
    //获取颜色状态
    QColor getColorStatus();
    //绘制传感器图形
    void drawSensorItem();
    bool getIsPassedBy();//获取是否走过
};


#endif // SENSORCLASS_H
