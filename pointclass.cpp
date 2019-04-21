#include "pointclass.h"

//构造函数
pointClass::pointClass(qint8 x, qint8 y)
{
    this->x = x;
    this->y = y;
}

//设置x
void pointClass::setX(qint8 x)
{
    this->x = x;
}
//设置y
void pointClass::setY(qint8 y)
{
    this->y = y;
}

qint8 &pointClass::getX()
{
    return x;
}

qint8 &pointClass::getY()
{
    return y;
}

//重载操作符判等
bool pointClass::operator ==(pointClass p)
{
    return (this->x == p.getX() && this->y == p.getY());
}
