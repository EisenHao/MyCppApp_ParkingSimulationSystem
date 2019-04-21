#ifndef CARCLASS_H
#define CARCLASS_H
#include "qglobal.h"
#include "qcolor.h"
#include "colorListSingleTon.h"
#include "qgraphicsscene.h"
#include "qgraphicsitem.h"
#include "qpen.h"
#include "pointclass.h"

/**
 * 汽车类
*/

class carClass {
private:
    QGraphicsRectItem* carBodyItem;//汽车车体绘图 图形项指针
    QGraphicsSimpleTextItem* carNumItem;//汽车绘图 图形项指针
    QColor carColor;//汽车颜色

    qint32 carWidth;//车位宽度
    qint32 halfMaxParkingNum;//停车场最大车位数(/2)
    qint32 PositionOffset;//奇数偶数偏移

    qint32 employeeID; //驾驶者的ID卡号
    qint32 parkSpaceNum;//预停车位
    qint32 parkingTime;//预停车时间
    qint32 leavingTime;//离开时间（运行时间的秒数）

    //汽车移动控制(改变汽车状态：每秒调用一次move_control_pre_second)
    qint8 now_x;//当前所在[相对位置]x, 0
    qint8 now_y;//当前所在[相对位置]y, 0
    qint8 destination_x; //目的地[相对位置]x, 0
    qint8 destination_y; //目的地[相对位置]y, 0
    volatile qint8 carStatus;//汽车状态：;1==in; 2==waitting; 3==parking; 4==out
    qint8 waitingForClash;//与其他车发生冲突，等待时间

    QGraphicsScene* scene;//场景指针
public:
    carClass(qint32 carWidth, qint32 halfMaxParkingNum,
             qint32 employeeID, qint32 parkingTime, qint32 leavingTime,
             QGraphicsScene* scene);//构造函数
    void generateACar();//生成一辆汽车信息
    qint32 getEmployeeID();//获取车辆的驾驶者的ID卡号
    bool ifChangeStatusToLeave(qint32 nowTime);//判断是否是时候离开
    bool ifcarStatusLeavedOrInvaild();//判断是否已经离开场
    qint32 getParkSpaceNum();//获取预停车位
    qint8 getCarStatus();//获取汽车状态
    qint32 getParkingTime();//获取parkingTime
    void setParkSpaceNum(qint32 val);//分配预停车位


    QGraphicsRectItem* getCarBodyItem();//获取汽车车体绘图 图形项指针
    QGraphicsSimpleTextItem* getCarNumItem();//获取汽车绘图 图形项指针

    void setCarStatus(qint8 val);//设置汽车状态
    void drawCarItem();////绘制汽车图形项
    void move_hor(bool dir, qint32 distance);//水平方向移动: 0==向左移动；  1==向右移动
    void move_Ver(bool dir, qint32 distance);//垂直方向移动：  0==向上移动；  1==向下移动
    void INC_WatingForClash();//等待时间加1；（默认为0->1 等待1s）
    void Inc_Now_x();//相对坐标x 自加
    qint8 getNow_x();//返回当前所在位置相对坐标x
    qint8 getNow_y();//返回当前所在位置相对坐标y

    pointClass getNext_Point();//获取下一秒时的相对坐标点（可能原地）

    //汽车状态：;1==in; 2==waitting; 3==parking; 4==out
    enum carStatusFlag {
        carIn = 1,
        carwaitting = 2,
        carparking = 3,
        carOut = 4,
        carLeaved = 5,
        carChecking = 6,
        carRefuseed=7
    };

    //移动汽车位置：每秒调用一次
    void move_control_pre_second();
};

#endif // CARCLASS_H
