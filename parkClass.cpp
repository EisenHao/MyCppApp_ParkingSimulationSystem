#include "parkclass.h"

//构造函数1：默认最大车位数为：10
parkClass::parkClass()
{
    //停车场最大车位数（/2）
    halfMaxParkingNum = 5;
    //停车场车数
    currentCarNum = 0;
    //累计来的车辆
    calcNumOfComingCar = 0;
    //累计离开车辆
    calcNumOfLeavingCar = 0;
    //累计所有停车时长
    calcParkingTime = 0;
    //每个车位是否停放标志（控制）
    for(qint8 i=0; i<halfMaxParkingNum*2; i++){
        isParkedList.append(false);
    }
}

//构造函数2：输入最大车位数
parkClass::parkClass(qint32 halfMaxParkingNum, QGraphicsScene *scene)
{
    this->scene = scene;

    //停车场最大车位数（偶数）
    this->halfMaxParkingNum = halfMaxParkingNum;
    //计算车位宽度、长度
    //当停车位数>4时，绘制停车场长度大于宽度，以长度为限制标准（对应车宽）
    if (halfMaxParkingNum > 4) {
        carWidth = viewLong / (halfMaxParkingNum + 6); //汽车宽度(计算得到（0.6倍视域长度用于绘停车场）)
        carLong = carWidth * 2; //汽车长度 （= 2*宽度）
    }
    //当停车位数<4时，绘制停车场宽度大于长度，以宽度为限制标准（对应车长）
    else{
        carLong = viewWidth * 4 / 5 / 3;//汽车长度(计算得到（0.6倍视域宽度用于绘停车场）)
        carWidth = carLong / 2;//汽车宽度 （= 长度/2）
    }
    PositionOffset = ((halfMaxParkingNum % 2) == 0) ? 0 : carWidth/2;

    //停车场车数
    currentCarNum = 0;
    //累计来的车辆
    calcNumOfComingCar = 0;
    //累计离开车辆
    calcNumOfLeavingCar = 0;
    //累计所有停车时长
    calcParkingTime = 0;
    //每个车位是否停放标志（控制）
    for(qint8 i=0; i<halfMaxParkingNum*2; i++){
        isParkedList.append(false);
    }


    //前门栏杆初始化
    railingFront = new railingClass(-(carWidth * (halfMaxParkingNum / 2 + 1) + PositionOffset), -carLong / 2, carWidth, scene);
    //后门栏杆初始化
    railingBehind = new railingClass((carWidth * (halfMaxParkingNum / 2 + 1) + PositionOffset), -carLong / 2, carWidth, scene);
    //前门传感器初始化
    sensorFront = new sensorClass(-(carWidth * (halfMaxParkingNum / 2 + 1) + PositionOffset) - carWidth/3, 0, carWidth, scene);
    //后门传感器初始化
    sensorBehind = new sensorClass((carWidth * (halfMaxParkingNum / 2 + 1) + PositionOffset) - carWidth/3, 0, carWidth, scene);
}

//返回一个最近的空车位号
qint32 parkClass::getCloseinSpaceNo()
{
    qint32 i = 0;
    //按列找
    while (i < halfMaxParkingNum) {
        if (isParkedList[i] == false){
            return i;
        } else if (isParkedList[halfMaxParkingNum+i] == false){
            return halfMaxParkingNum+i;
        } else {
            i++;
        }
    }
    return (qint32)(-1);//未找到最近空位，返回错误 -1
}

//返回平均停车时长（=calcParkingTime/calcNumOfComingCar）
qreal parkClass::getAverageParkingTime()
{
    //防止被除数为0 意外崩溃
    if(calcNumOfComingCar == 0){
        return 0;
    }
    qreal averageParkingTime = calcParkingTime/calcNumOfComingCar;
    return averageParkingTime;
}

bool parkClass::checkEmployeeNum(carClass *aCar)
{
    icReader->readICNumber(aCar);//调用停车场实例的 IC读卡器实例，读取汽车EmployeeID
    return (icReader->connectToMTocheck());//链接后台M(单例)检测有效性
}

//返回是否还有空车位
bool parkClass::hasFreeParkingSpace()
{
    qint32 i=0;
    while (isParkedList[i]==true && i<halfMaxParkingNum*2) {
        i++;
    }
    if(i<halfMaxParkingNum*2){
        return true;
    }
    return false;
}

//返回空闲车位数
qint32 parkClass::getFreeCarSpace()
{
    return (2*halfMaxParkingNum - currentCarNum);
}
//返回停车场内车数
qint32 parkClass::getCurrentCarNum()
{
    return currentCarNum;
}
//返回累计入场车数
qint32 parkClass::getCalcNumOfComingCar()
{
    return calcNumOfComingCar;
}
//返回累计出场车数
qint32 parkClass::getCalcNumOfLeavingCar()
{
    return calcNumOfLeavingCar;
}

//预占用某个车位
void parkClass::PreOccupyOneCarSpace(qint32 employeeNum ,qint32 parkNum, qint32 parkingTime){
    isParkedList[parkNum] = true;//标志为标识已被占用
    parkedEmployeeNumList.append(employeeNum);//在当场停放车辆名中添加

    //停车场车数
    currentCarNum++;
    //累计来的车辆
    calcNumOfComingCar++;
    //累计所有停车时长
    calcParkingTime += parkingTime;
}

//释放某块车位的占用
void parkClass::preFreeOneCarSpace(qint32 parkNum)
{
    //每个车位是否停放标志（控制）
    isParkedList[parkNum] = false;
    //停车场车数
    currentCarNum--;
    //累计离开车辆
    calcNumOfLeavingCar++;
}


//检测是否该号码已在停车场正在停放列表中
//若已存在==true；   不存在==false
bool parkClass::isEmployeeIDExisted(qint32 employeeNum)
{
    for(qint32 i=0; i< parkedEmployeeNumList.size(); i++){
        if(employeeNum == parkedEmployeeNumList[i]){
            return true;
        }
    }
    return false;
}

//停车场内停放号码 集合中删除某个
void parkClass::deleteAEmployNumInParkedEmployeeNumList(qint32 EmployeeNum)
{
    for(qint32 i=0; i<parkedEmployeeNumList.size(); i++){
        if(parkedEmployeeNumList[i] == EmployeeNum){//找到改元素
            parkedEmployeeNumList.erase(parkedEmployeeNumList.begin() + i);//删除该元素
        }
    }
}


/**
 * 几何方式绘制停车场图
 * 参数：停车位数量/2; //场景坐标
*/
void parkClass::drawParkPicture()
{
    QPen pen;//画笔
    qint32 half_half = halfMaxParkingNum / 2;

    pen.setWidth(4);
    pen.setColor(QColor(0, 0, 0));//Black
    //绘制上边
    scene->addLine(-(carWidth * (half_half + 1) + PositionOffset), -carLong * 3 / 2, //(x1, y1)
                    (carWidth * (half_half + 1) + PositionOffset), -carLong * 3 / 2, pen);//(x2, y2)
    //绘制下边
    scene->addLine(-(carWidth * (half_half + 1) + PositionOffset), carLong * 3 / 2, //(x1, y1)
                    (carWidth * (half_half + 1) + PositionOffset), carLong * 3 / 2, pen);//(x2, y2)
    //绘制入口门
    scene->addLine(-(carWidth * (half_half + 1) + PositionOffset), carLong / 2, //(x1, y1)
                   -(carWidth * (half_half + 1) + PositionOffset), carLong * 3 / 2, pen);//(x2, y2)
    scene->addLine(-(carWidth * (half_half + 1) + PositionOffset), -carLong * 3 / 2, //(x1, y1)
                   -(carWidth * (half_half + 1) + PositionOffset), -carLong / 2, pen);//(x2, y2)
    scene->addLine(-(carWidth * (half_half + 1) + PositionOffset) - carWidth/4, carLong / 2, //(x1, y1)
                   -(carWidth * (half_half + 1) + PositionOffset) + carWidth/4, carLong / 2, pen); //(x2, y2)
    scene->addLine(-(carWidth * (half_half + 1) + PositionOffset) - carWidth/4, -carLong / 2, //(x1, y1)
                   -(carWidth * (half_half + 1) + PositionOffset) + carWidth/4, -carLong / 2, pen); //(x2, y2)
    //绘制出口门
    scene->addLine((carWidth * (half_half + 1) + PositionOffset), carLong / 2, //(x1, y1)
                   (carWidth * (half_half + 1) + PositionOffset), carLong * 3 / 2, pen);//(x2, y2)
    scene->addLine((carWidth * (half_half + 1) + PositionOffset), -carLong * 3 / 2, //(x1, y1)
                   (carWidth * (half_half + 1) + PositionOffset), -carLong / 2, pen);//(x2, y2)
    scene->addLine((carWidth * (half_half + 1) + PositionOffset) - carWidth/4, carLong / 2, //(x1, y1)
                   (carWidth * (half_half + 1) + PositionOffset) + carWidth/4, carLong / 2, pen); //(x2, y2)
    scene->addLine((carWidth * (half_half + 1) + PositionOffset) - carWidth/4, -carLong / 2, //(x1, y1)
                   (carWidth * (half_half + 1) + PositionOffset) + carWidth/4, -carLong / 2, pen); //(x2, y2)

    pen.setWidth(2);
    pen.setColor(QColor(238, 180, 34));//Goldenrod2
    //绘制中上边
    scene->addLine(-(carWidth * half_half  + PositionOffset), -carLong / 2, //(x1, y1)
                    (carWidth * half_half  + PositionOffset), -carLong / 2, pen);//(x2, y2)
    //绘制中下边
    scene->addLine(-(carWidth * half_half  + PositionOffset), carLong / 2, //(x1, y1)
                    (carWidth * half_half  + PositionOffset), carLong / 2, pen);//(x2, y2)
    //绘制车位线
    qint32 LoopDraw =  half_half  + 1;//循环绘制多条停车位间隔线。循环次数
    for(qint32 i=0; i < LoopDraw; i++){
        //1象限
        scene->addLine((carWidth * i + PositionOffset), carLong * 3 / 2, //(x1, y1)
                       (carWidth * i + PositionOffset), carLong / 2, pen); //(x2, y2)
        //2象限
        scene->addLine(-(carWidth * i + PositionOffset), carLong / 2, //(x1, y1)
                       -(carWidth * i + PositionOffset), carLong * 3 / 2, pen); //(x2, y2)
        //3象限
        scene->addLine(-(carWidth * i + PositionOffset), -carLong * 3 / 2, //(x1, y1)
                       -(carWidth * i + PositionOffset), -carLong / 2, pen); //(x2, y2)
        //4象限
        scene->addLine((carWidth * i + PositionOffset), -carLong * 3 / 2, //(x1, y1)
                       (carWidth * i + PositionOffset), -carLong / 2, pen); //(x2, y2)
    }

    //车位添车位号码
    QVector<QGraphicsTextItem*> TextList(2 * halfMaxParkingNum);
    qint32 fontSize = carWidth/2 - 2;//计算得到停车位字体大小
    //计算得到初始第0个车位的中心坐标
    qint32 initial_X0 = - (carWidth * (half_half) - (carWidth/2 - PositionOffset)); //X0，计算得到左上方第一个车位的坐标
    qint32 initial_Y0 = - carLong; //Y0
    //减去字体偏移
    initial_X0 -= fontSize/2;
    QString showPostNumBuf;
    for(qint32 i=0; i<halfMaxParkingNum; i++){
        //添加。上方第 i 个 车位号
        showPostNumBuf.sprintf("%d", i);
        (TextList[i]) = scene->addText(showPostNumBuf, QFont("Consolas", fontSize));//车位字体字号根据车位宽度自动调整
        (TextList[i])->setPos(initial_X0 + carWidth * i - fontSize/2*(halfMaxParkingNum/10), initial_Y0 - fontSize*3/4);
        scene->addItem((TextList[i]));

        //添加。下方第 halfMaxParkingNum + i 个 车位号
        showPostNumBuf.sprintf("%d", halfMaxParkingNum + i);
        (TextList[halfMaxParkingNum + i]) = scene->addText(showPostNumBuf, QFont("Consolas", fontSize));//车位字体字号根据车位宽度自动调整
        (TextList[halfMaxParkingNum + i])->setPos(initial_X0 + carWidth * i - fontSize/2*(halfMaxParkingNum/10), - initial_Y0 - fontSize*3/4);
        scene->addItem((TextList[halfMaxParkingNum + i]));
    }
    scene->clearSelection();

    //绘制前门栏杆
    railingFront->drawRailingItem();
    //绘制后门栏杆
    railingBehind->drawRailingItem();
    //绘制前门传感器图形
    sensorFront->drawSensorItem();
    //绘制后门传感器图形
    sensorBehind->drawSensorItem();
}

//返回车位宽度
qint32 parkClass::getCarWidth(){
    return carWidth;
}

//获取前门栏杆实例指针
railingClass *parkClass::getRailingFront(){
    return railingFront;
}
//获取后门栏杆实例指针
railingClass *parkClass::getRailingBehind(){
    return railingBehind;
}
//获取前门传感器实例指针
sensorClass *parkClass::getSensorFront(){
    return sensorFront;
}
//获取后门传感器实例指针
sensorClass *parkClass::getSensorBehind(){
    return sensorBehind;
}




