#include "carclass.h"

//构造函数
carClass::carClass(qint32 carWidth, qint32 halfMaxParkingNum,
                   qint32 employeeID, qint32 parkingTime, qint32 leavingTime,
                   QGraphicsScene *scene)
{
    colorListSingleTon* colorP = colorListSingleTon::getInstance();
    //生成随机颜色
    carColor = colorP->getARandColor();//随机获取一种汽车颜色

    this->carWidth = carWidth;
    this->halfMaxParkingNum = halfMaxParkingNum;
    this->employeeID = employeeID;
    this->parkingTime = parkingTime;
    this->leavingTime = leavingTime;
    this->scene = scene;

    now_x = 0;//当前所在[相对位置]x, 0
    now_y = 0;//当前所在[相对位置]y, 0
    //未分配目的停车位置时的默认目的地
    destination_x = halfMaxParkingNum + 3;
    destination_y = 0;
    waitingForClash = 0;//与其他车发生冲突，等待时间
    carStatus = carStatusFlag::carIn;//初始状态为进入
    PositionOffset = ((halfMaxParkingNum % 2) == 0) ? 0 : carWidth/2;
}

//获取车辆的驾驶者的ID卡号
qint32 carClass::getEmployeeID()
{
    return employeeID;
}

//判断是否是时候离开
bool carClass::ifChangeStatusToLeave(qint32 nowTime)
{
    if(nowTime >= leavingTime){
        if(carStatus != carStatusFlag::carOut && carStatus != carStatusFlag::carChecking){
            carStatus = carStatusFlag::carOut;//改变汽车状态为出
            //改变目的地址
            destination_x = halfMaxParkingNum + 6;
            destination_y = 0;
            return true;
        }
    }
    return false;
}

//判断是否已经离开场，或拒绝进入的无效车
bool carClass::ifcarStatusLeavedOrInvaild()
{
    if(carStatusFlag::carLeaved == carStatus || carStatusFlag::carRefuseed == carStatus){
        return true;
    }
    return false;
}

//获取预停车位
qint32 carClass::getParkSpaceNum()
{
    return parkSpaceNum;
}

//获取汽车状态
qint8 carClass::getCarStatus()
{
    return carStatus;
}

//获取parkingTime
qint32 carClass::getParkingTime()
{
    return parkingTime;
}

//分配预停车位
void carClass::setParkSpaceNum(qint32 val)
{
    this->parkSpaceNum = val;
    //根据分配停车位计算相对目的地址
    destination_x = parkSpaceNum % halfMaxParkingNum + 3;
    destination_y = parkSpaceNum >= halfMaxParkingNum ? 2 : -2;
}

//获取汽车车体绘图 图形项指针
QGraphicsRectItem *carClass::getCarBodyItem()
{
    return carBodyItem;
}

//获取汽车绘图 图形项指针
QGraphicsSimpleTextItem *carClass::getCarNumItem()
{
    return carNumItem;
}

//设置汽车状态
void carClass::setCarStatus(qint8 val)
{
    carStatus = val;
}

////绘制汽车图形项
void carClass::drawCarItem()
{
    //计算得到初始进入门口的坐标
    qint32 initial_X0 = -(carWidth * (halfMaxParkingNum/2 + 3) + PositionOffset); //X0，计算得到左方入口一车身外坐标
    qint32 initial_Y0 = 0; //Y0
    QString showPostNumBuf;

    //画 车外框（正方形）
    carBodyItem = new QGraphicsRectItem(initial_X0, initial_Y0-carWidth/2, carWidth, carWidth);
    carBodyItem->setBrush(carColor);
    carBodyItem->setFlags(QGraphicsItem::ItemIsMovable);/*可移动*/
    scene->addItem(carBodyItem);//添加图形项
    scene->clearSelection();

    //画 车 名
    qint32 fontSize = carWidth/2 - 2;//计算得到停车位字体大小
    showPostNumBuf.sprintf("%d", employeeID);
    carNumItem = new QGraphicsSimpleTextItem(showPostNumBuf);
    carNumItem->setFont(QFont("Consolas", fontSize));
    carNumItem->setFlags(QGraphicsItem::ItemIsMovable); /*可移动*/
    carNumItem->setPos(initial_X0 - fontSize/2*(employeeID/10 - 1), initial_Y0 - fontSize*3/4);
    scene->addItem(carNumItem);//添加图形项
    scene->clearSelection();
}

//水平方向移动: 0==向左移动；  1==向右移动
void carClass::move_hor(bool dir, qint32 distance)
{
    ////汽车车体移动：
    QPointF _pos = carBodyItem->pos();
    //计算下一步x坐标
    qreal body_x = dir ? distance + _pos.x() : -distance + _pos.x();
    _pos.setX(body_x);
    carBodyItem->setPos(_pos);////设置新坐标点

    ////汽车编号移动：
    QPointF _pos2 = carNumItem->pos();
    //计算下一步x坐标
    qreal body_x2 = dir ? distance + _pos2.x() : -distance + _pos2.x();
    _pos2.setX(body_x2);
    carNumItem->setPos(_pos2);////设置新坐标点
}

//垂直方向移动：  0==向上移动；  1==向下移动
void carClass::move_Ver(bool dir, qint32 distance)
{
    ////汽车车体移动：
    QPointF _pos = carBodyItem->pos();
    //计算下一步y坐标
    qreal body_y = dir ? distance + _pos.y() : -distance + _pos.y();
    _pos.setY(body_y);
    carBodyItem->setPos(_pos);////设置新坐标点

    ////汽车编号移动：
    QPointF _pos2 = carNumItem->pos();
    //计算下一步y坐标
    qreal body_y2 = dir ? distance + _pos2.y() : -distance + _pos2.y();
    _pos2.setY(body_y2);
    carNumItem->setPos(_pos2);////设置新坐标点
}

//等待时间加1；（默认为0->1 等待1s）
void carClass::INC_WatingForClash()
{
    waitingForClash++;
}

//返回当前所在位置相对坐标x
qint8 carClass::getNow_x()
{
    return now_x;
}
//相对坐标x 自加
void carClass::Inc_Now_x()
{
    now_x ++;
}

//返回当前所在位置相对坐标y
qint8 carClass::getNow_y()
{
    return now_y;
}

/**
 * 获取下一秒时的相对坐标点（可能原地）
*：一次最多移动一个 x 或 y (改变绝对值为1)
*/
pointClass carClass::getNext_Point()
{
    //一、如果需要等待时间大于1
    if(waitingForClash > 0){
        return pointClass(now_x, now_y);//直接返回now_x，这1秒不动
    }
    switch (carStatus) {
        //二、若状态为进入停车场：
        case carStatusFlag::carIn:
            //1.优先判断是否需要左右移（每次最多进行一次移动）
            if(now_x != destination_x){
                bool dir = now_x < destination_x ? true : false;
                 return (dir ? pointClass(now_x+1, now_y) : pointClass(now_x-1, now_y));
            }
            //2.若不需要左右移，再判断是否上下移动
            else if(now_y != destination_y){
                bool dir = now_y < destination_y ? true : false;
                return (dir ? pointClass(now_x, now_y+1) : pointClass(now_x, now_y-1));
            }
            break;

        //三、若状态为离开停车场：
        case carStatusFlag::carOut:
            //1.优先判断是否需要上下移（每次最多进行一次移动）
            if(now_y != destination_y){
                bool dir = now_y < destination_y ? true : false;
                return (dir ? pointClass(now_x, now_y+1) : pointClass(now_x, now_y-1));
            }
            //2.若不需要上下移，再判断是否左右移动
            else if(now_x != destination_x){
                bool dir = now_x < destination_x ? true : false;
                return (dir ? pointClass(now_x+1, now_y) : pointClass(now_x-1, now_y));
            }
            break;

        default:
            return pointClass(now_x, now_y);//不动
            break;
    }
    return pointClass(now_x, now_y);//不动
}

//移动汽车位置：每秒调用一次（每次最多进行一次移动）
void carClass::move_control_pre_second()
{
    //如果需要等待时间大于1
    if(waitingForClash > 0){
        waitingForClash--;
        return;//直接返回，这1秒不执行
    }
    switch (carStatus) {
        //一、若状态为停放中：
        case carStatusFlag::carparking:
        //二、若状态为等待中：
        case carStatusFlag::carwaitting:
            break;
        case carStatusFlag::carChecking:
            break;

        //三、若状态为进入停车场：
        case carStatusFlag::carIn:
            //1.优先判断是否需要左右移（每次最多进行一次移动）
            if(now_x != destination_x){
                bool dir = now_x < destination_x ? true : false;
                now_x = dir ? now_x+1 : now_x-1;
                this->move_hor(dir, carWidth);//左右移
                //若已到达目的地，改变汽车状态
                if(now_x == destination_x && now_y ==destination_y){
                    carStatus = carStatusFlag::carparking;//改变状态为停放中
                }
            }
            //2.若不需要左右移，再判断是否上下移动
            else{
                if(now_y != destination_y){
                    bool dir = now_y < destination_y ? true : false;
                    now_y = dir ? now_y+1 : now_y-1;
                    this->move_Ver(dir, carWidth);//上下移
                    //若已到达目的地，改变汽车状态
                    if(now_x == destination_x && now_y ==destination_y){
                        carStatus = carStatusFlag::carparking;//改变状态为停放中
                    }
                }
            }
            break;

            //四、若状态为离开停车场：
        case carStatusFlag::carOut:
            //1.优先判断是否需要上下移（每次最多进行一次移动）
            if(now_y != destination_y){
                bool dir = now_y < destination_y ? true : false;
                now_y = dir ? now_y+1 : now_y-1;
                this->move_Ver(dir, carWidth);//上下移
                //若已到达目的地，改变汽车状态
                if(now_x == destination_x && now_y ==destination_y){
                    carStatus = carStatusFlag::carLeaved;//改变状态为已离开
                }
            }
            //2.若不需要上下移，再判断是否左右移动
            else{
                if(now_x != destination_x){
                    bool dir = now_x < destination_x ? true : false;
                    now_x = dir ? now_x+1 : now_x-1;
                    this->move_hor(dir, carWidth);//左右移
                    //若已到达目的地，改变汽车状态
                    if(now_x == destination_x && now_y ==destination_y){
                        carStatus = carStatusFlag::carLeaved;//改变状态为已离开
                    }
                }
            }
            break;

        //五、若状态为已离开
        case carStatusFlag::carLeaved:
               break;
        default:
            break;
    }
}
