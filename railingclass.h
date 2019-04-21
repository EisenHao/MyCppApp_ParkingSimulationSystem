#ifndef RAILINGCLASS_H
#define RAILINGCLASS_H
#include "qglobal.h"
#include "qgraphicsscene.h"
#include "qgraphicsitem.h"
#include "qpen.h"

/**
 * 栏杆类
*/

#define TotalStep 3 //开门（或关门）总共需要调用（多少+1）次开门（或关门）函数（涉及延时时间长短）

class railingClass{
private:
    QGraphicsRectItem* railingItem;//栏杆绘图 图形项指针
    QGraphicsScene *scene;//场景指针
    bool status;//记录栏杆状态：0 -- 关； 1 -- 开
    qint8 open_step_i;//需要执行两次（执行时间两秒）
    bool isFullyOpen;//是否完全打开标志（存在半打开）
    qint8 close_step_i;//需要执行两次（执行时间两秒）
    bool isFullyClose;//是否完全关闭标志（存在半打开）
    qint32 X0;//X0坐标
    qint32 Y0;//Y0坐标
    qint32 carWidth;//车位宽度
public:
    railingClass(qint32 X0, qint32 Y0, qint32 carWidth, QGraphicsScene *scene);//构造函数
    void drawRailingItem();//绘制栏杆图形
    bool drawOpen();//图形表示栏杆状态：开门步骤（需至少调用两次）（打开时间：2秒）
    bool drawclose();//图形表示栏杆状态：关门步骤（需至少调用两次）（关闭时间：2秒）
    bool getIsFullyCoosed();//获取是否完全关门
};

#endif // RAILINGCLASS_H
