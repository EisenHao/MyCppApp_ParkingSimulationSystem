#ifndef POINTCLASS_H
#define POINTCLASS_H
#include "qglobal.h"

/**
 * 相对坐标点
 * ：(x, y)
*/

class pointClass{
private:
    qint8 x;
    qint8 y;
public:
    pointClass(qint8 x, qint8 y);//构造函数
    void setX(qint8 x);//设置x
    void setY(qint8 y);//设置y
    qint8& getX();//获取x
    qint8& getY();//获取y
    bool operator == (pointClass p);//重载操作符判断
};

#endif // POINTCLASS_H
