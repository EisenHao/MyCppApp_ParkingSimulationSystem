#ifndef PARKCLASS_H
#define PARKCLASS_H
#include "qglobal.h"
#include "qvector.h"
#include "qgraphicsscene.h"
//#include "qgraphicsview.h"
#include "qgraphicsitem.h"
#include "qpen.h"
#include "railingclass.h"
#include "sensorclass.h"
#include "iccardreaderclass.h"

#define viewLong 450
#define viewWidth 300

/**
 * 停车场类
*/

class parkClass{
private:
    QGraphicsScene *scene;//绘图场景指针
    qint32 halfMaxParkingNum;//停车场最大车位数(/2)
    qint32 carWidth;//车位宽度
    qint32 carLong;//车位长度
    qint32 PositionOffset;//偏移（若单边车位为奇数则位置偏移为:车宽/2; 为偶数则为：0）
    railingClass* railingFront;//前门栏杆
    railingClass* railingBehind;//后门栏杆
    sensorClass* sensorFront;//前门传感器
    sensorClass* sensorBehind;//后门传感器
    ICcardReaderClass* icReader;//读卡器实例

    //停车场车数
    qint32 currentCarNum;
    //累计来的车辆
    qint32 calcNumOfComingCar;
    //累计离开车辆
    qint32 calcNumOfLeavingCar;
    //累计所有停车时长
    qint32 calcParkingTime;
    //每个车位是否停放标志（控制）
    QVector<bool> isParkedList;
    //停车场内停放号码 集合
    QVector<qint32> parkedEmployeeNumList;


public:
    parkClass();//构造函数1：默认最大车位数为：10
    parkClass(qint32 halfMaxParkingNum, QGraphicsScene *scene);//构造函数2：输入最大车位数
    qint32 getCloseinSpaceNo();//返回一个最近的空车位号
    bool hasFreeParkingSpace();//返回是否还有空车位
    qint32 getFreeCarSpace();//返回空闲车位数
    qint32 getCurrentCarNum();//返回停车场内车数
    qint32 getCalcNumOfComingCar();//返回累计入场车数
    qint32 getCalcNumOfLeavingCar();//返回累计出场车数
    qreal getAverageParkingTime();//返回平均停车时长（=calcParkingTime/calcNumOfComingCar）
    bool checkEmployeeNum(carClass* aCar);//检测该卡的信息有效性

    void PreOccupyOneCarSpace(qint32 employeeNum ,qint32 parkNum, qint32 parkingTime);//预占用某个车位
    void preFreeOneCarSpace(qint32 parkNum);//释放某块车位的占用
    bool isEmployeeIDExisted(qint32 employeeNum);//检测是否该号码已在停车场正在停放列表中
    void deleteAEmployNumInParkedEmployeeNumList(qint32 EmployeeNum);//停车场内停放号码 集合中删除某个

    /**
     * 几何方式绘制停车场图
     * 参数：停车位数量/2; //场景坐标
    */
    void drawParkPicture();

    //返回车位宽度
    qint32 getCarWidth();
    //获取前门栏杆实例指针
    railingClass* getRailingFront();
    //获取后门栏杆实例指针
    railingClass* getRailingBehind();
    //获取前门传感器实例指针
    sensorClass* getSensorFront();
    //获取后门传感器实例指针
    sensorClass* getSensorBehind();

};

#endif // PARKCLASS_H
