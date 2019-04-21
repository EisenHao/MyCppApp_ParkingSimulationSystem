#include "sensorclass.h"

//构造函数1
sensorClass::sensorClass()
{
    this->colorStatus = QColor(255, 0, 0);//red
    this->X0 = 0;
    this->Y0 = 0;
    this->radius = 0;
    this->isArrived = false;
    this->isPassedBy = false;
}

//构造函数2
sensorClass::sensorClass(qint32 X0, qint32 Y0, qint32 carWidth, QGraphicsScene *scene)
{
    this->colorStatus = QColor(255, 0, 0);//red
    this->X0 = X0;
    this->Y0 = Y0;
    this->radius = carWidth/5;
    this->isArrived = false;
    this->isPassedBy = false;
    this->scene = scene;
}

//设置有汽车来
void sensorClass::somethingArrived(){
    isArrived = true;
    isPassedBy = false;
}

//设置已经通过
void sensorClass::somethingPassedBy(){
    isPassedBy = true;
    isArrived = false;
}

//获取颜色状态
QColor sensorClass::getColorStatus(){
    if(isArrived==true){
        colorStatus = QColor(255, 0, 0);//red
    }else{
        colorStatus = QColor(50, 205, 50);//LimeGreen
    }
    return colorStatus;
}

//绘制传感器图形
void sensorClass::drawSensorItem()
{
    //画一个圆
    sensorItem = new QGraphicsEllipseItem(-radius, -radius, radius*2, radius*2);
    sensorItem->setPos(X0, Y0);//设置中心
    sensorItem->setBrush(QBrush(getColorStatus()));
    scene->addItem(sensorItem);
    scene->clearSelection();
}

//获取是否走过
bool sensorClass::getIsPassedBy()
{
    return isPassedBy;
}

