#include "railingclass.h"


//构造函数
railingClass::railingClass(qint32 X0, qint32 Y0, qint32 carWidth, QGraphicsScene *scene)
{
    this->status = 0;
    this->X0 = X0;
    this->Y0 = Y0;
    this->carWidth = carWidth;
    isFullyOpen = false;//完全打开为：假
    open_step_i = TotalStep;//需要调用 TotalStep 次 open() 才能至完全打开状态
    isFullyClose = true;//完全关闭为：真
    close_step_i = 0;//需要调用 0 次 close() 才能至完全关闭状态
    this->scene = scene;
}

//绘制栏杆图形
void railingClass::drawRailingItem()
{
    //画一个矩形
    railingItem = new QGraphicsRectItem(X0, Y0, 4, 2*carWidth);//左上角坐标，宽度，高度
    railingItem->setBrush(QBrush(QColor(255, 0, 0)));
    railingItem->setFlags(QGraphicsItem::ItemIsMovable); /*可移动*/
    scene->addItem(railingItem);//添加图形项
    scene->clearSelection();
}

//图形表示栏杆状态：开门步骤
bool railingClass::drawOpen()
{
    //若没有完全打开：才执行开门步骤
    if(!isFullyOpen){
        //绘图：开(1/TotalStep)
        QPointF _pos = railingItem->pos();//获取当前栏杆实例在图中位置
        _pos.setY( -(2*carWidth)/(TotalStep-1) + _pos.y());//上移(1/TotalStep)个门长度
        railingItem->setPos(_pos);//设置新坐标点

        /************变量控制***************/
        open_step_i--;//至完全开门状态的剩余步骤减一
        close_step_i++;//至完全关门状态的剩余次数加一
        //完全关闭状态置：假
        if(isFullyClose == true){
            isFullyClose = false;
        }
        //判断是否完全打开
        if(open_step_i == 0){
            isFullyOpen = true;//完全打开标志
        }
    }
    return isFullyOpen;
}

//图形表示栏杆状态：关门步骤
bool railingClass::drawclose()
{
    //若没有完全关闭：才执行关门步骤
    if(!isFullyClose){
        //绘图：关(1/TotalStep)
        QPointF _pos = railingItem->pos();//获取当前栏杆实例在图中位置
        _pos.setY( +(2*carWidth)/(TotalStep-1) + _pos.y());//上移(1/TotalStep)个门长度
        railingItem->setPos(_pos);//设置新坐标点

        /************变量控制***************/
        close_step_i--;//至完全关门状态的剩余次数减一
        open_step_i++;//至完全开门状态的剩余步骤加一
        //完全开启状态置：假
        if(isFullyOpen == true){
            isFullyOpen = false;
        }
        //判断是否完全关闭
        if(close_step_i == 0){
            isFullyClose = true;//完全关闭标志
        }
    }
    return isFullyClose;
}

//获取是否完全关门
bool railingClass::getIsFullyCoosed()
{
    return isFullyClose;
}
















