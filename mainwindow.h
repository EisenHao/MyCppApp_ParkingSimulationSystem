#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QTextStream"
//#include "QMessageBox"
#include "QDir"
#include "QFile"
#include "qglobal.h"
#include "parkclass.h"
#include "qtimer.h"
#include "qdatetime.h"
#include <QMainWindow>
#include "MSingleTon.h"
#include "qgraphicsscene.h"
//#include "qgraphicsview.h"
#include "qgraphicsitem.h"
#include "qpen.h"
#include "carclass.h"
#include "pointclass.h"


/**
 * 仿真管理类(主窗口 + 总控制类)
*/


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    qint8 min_counter;
    Ui::MainWindow *ui;//窗口
    QGraphicsScene* scene;//场景坐标
    QString simulateStartTimeStr;//记录仿真开始时间
    qint32 halfMaxParkingNum;//最大停车位数量/2
    qint32 employeeNum;//员工数量
    qint32 carWidth;//车位宽度

    //定时器实例
    QTimer *fTimer;
    //仿真已运行时间
    qint32 calcRuningTime;

    //停车场类-实例
    parkClass *park;
    //生成汽车容器
    QVector<carClass*> creatCarList;

    void bindingViewAndSence();//绑定UI界面的view 与 场景 sence
    void drawGraphicsItem();//绘图

private slots:
    void on_timer_timout();//定时溢出函数
    void on_startBut_clicked();//当点击开始仿真按钮，响应函数
    void on_stopBut_clicked();//当点击停止仿真按钮，响应函数
    void on_creatCarBut_clicked();//点击生成汽车按钮，响应函数

protected:
    void resizeEvent();//改变大小事件

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showOrRefreshOnScreen(QString showTime, qint32 RuningTime);//将当前时间，运行时间，剩余车位数显示在界面上

    void addReportMsgPreMin();//添加【定时播报】信息到信息显示框

    void autoCloseDoorFrontPreSec();//自动控制关前门（若车已通过传感器且门未完全关时）

    void autoCloseDoorBehindPreSec();//自动控制关后门（若车已通过传感器且门未完全关时）

    void saveMsgDataToFile(QString msg);//将系统报告消息存到文件中

    void checkForAndSolveClash();//检测下1秒有无争道并处理(让弱势方延时1秒)

    void simulateControlAllCar();//仿真控制所有生成车的函数

};

#endif // MAINWINDOW_H
