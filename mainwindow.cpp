#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    min_counter = 0;
    ui->setupUi(this);
    calcRuningTime = 0;//仿真已运行时间为0
    fTimer = new QTimer(this);
    fTimer->stop();
    fTimer->setInterval(1000);//设置定时周期，单位毫秒
    connect(fTimer, SIGNAL(timeout()), this, SLOT(on_timer_timout()));

    ui->stopBut->setEnabled(false);//设置停止仿真按钮不可用
    ui->creatCarBut->setEnabled(false);//设置生成汽车按钮不可用
    ui->InputEmployeeNumFirm->setValue(10);//设置输入员工数量框初值：10
    ui->inputHalfMaxParkingNum->setValue(5);//设置输入总车位数/2框初值：5

    //初始化LCD显示屏
    ui->lcdNumber->setStyleSheet("border: 1px solid black; color: red; background: yellow;");
    ui->lcdNumber->hide();//隐藏lcdNumber组件
    bindingViewAndSence();//绑定UI界面的view 与 场景 sence
}

MainWindow::~MainWindow()
{
    delete ui;
}

//将当前时间，和运行时间显示在界面上
void MainWindow::showOrRefreshOnScreen(QString showTime, qint32 RuningTime)
{
    QString showStr;
    //将数据更新到对应显示框
    ui->currentTimeFirm->setText(showTime);//将当前时间数据更新到时间显示框
    ui->currentTimeFirm->clearFocus();

    showStr.sprintf("%d s", RuningTime);
    ui->runingTimeCountFirm->setText(showStr);//将运行时间数据更新到运行时间显示框
    ui->runingTimeCountFirm->clearFocus();

    qint32 freeCarSpaces = park->getFreeCarSpace();

    showStr.sprintf("%d 个", freeCarSpaces);
    ui->leavingParkingSpaceFirm->setText(showStr);//将剩余空位数更新到剩余数据中
    ui->leavingParkingSpaceFirm->clearFocus();
    ui->lcdNumber->display(freeCarSpaces);////将剩余空位数更新到LCD显示屏中
}


//添加【定时播报】信息到信息显示框
void MainWindow::addReportMsgPreMin()
{
    /**应至少包括：
     * 当前时刻:（包括时、分、秒）
     * 当前场内车数：
     * “累计入场车数”；
     * “累计出场车数”；
     * “平均停车时间”。
    */
    qint32 getCurrentCarNum = park->getCurrentCarNum();
    qint32 getCalcNumOfComingCar = park->getCalcNumOfComingCar();
    qint32 getCalcNumOfLeavingCar = park->getCalcNumOfLeavingCar();
    qreal  averageTime = park->getAverageParkingTime();

    //数据更新到对应显示框
    ui->reportText->append(QDateTime::currentDateTime().toString("hh:mm:ss"));

    QString showStr;
    showStr.sprintf("System report(/min): CurrentCarNum: %d; CalcNumOfComingCar: %d; CalcNumOfLeavingCar: %d; AverageParkingTime: %.1f s",
                    getCurrentCarNum, getCalcNumOfComingCar, getCalcNumOfLeavingCar, averageTime);
    //数据更新到对应显示框
    ui->reportText->append(showStr);//换行追加信息到定时播报框
}

//绑定UI界面的view 与 场景 sence
void MainWindow::bindingViewAndSence()
{
    QRectF rect(-viewLong/2, -viewWidth/2, viewLong, viewWidth);//左上角坐标，宽度，高度
    this->scene = new QGraphicsScene(rect);//scene逻辑定义
    ui->view->setScene(scene);//绑定scene
}

//绘图(viewLong x viewWidth)
void MainWindow::drawGraphicsItem()
{
    QRectF rect(-viewLong/2, -viewWidth/2, viewLong, viewWidth);//左上角坐标，宽度，高度
    //画一个矩形框，大小等于scene
    QGraphicsRectItem* item1 = new QGraphicsRectItem(rect);
    item1->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);//设置flags

    //设置并绑定画笔
    QPen pen;
    pen.setWidth(2);
    item1->setPen(pen);
    scene->addItem(item1);//添加图形项

    //画一个位于scen中心的椭圆
    QGraphicsEllipseItem* item2 = new QGraphicsEllipseItem(-100, -50, 200, 100);
    item2->setPos(0, 0);//中心
    item2->setBrush(QBrush(QColor(0, 0, 255)));
    item2->setFlags(QGraphicsItem::ItemIsMovable| /*可移动*/
                    QGraphicsItem::ItemIsSelectable| /*可选择*/
                    QGraphicsItem::ItemIsFocusable); /*可成为焦点的*/
    scene->addItem(item2);

    //画一个圆，中心位于scene的边缘(部分显示)
    QGraphicsEllipseItem* item3 = new QGraphicsEllipseItem(-50, -50, 100, 100);
    item3->setPos(rect.right(), rect.bottom());
    item3->setBrush(QBrush(QColor(255, 0, 0)));
    item3->setFlags(QGraphicsItem::ItemIsMovable| /*可移动*/
                    QGraphicsItem::ItemIsSelectable| /*可选择*/
                    QGraphicsItem::ItemIsFocusable); /*可成为焦点的*/
    scene->addItem(item3);

    scene->clearSelection();
}


//当点击开始按钮，响应函数
void MainWindow::on_startBut_clicked()
{
    ui->startBut->setEnabled(false);
    ui->stopBut->setEnabled(true);//设置停止仿真按钮可用
    ui->creatCarBut->setEnabled(true);//设置生成汽车按钮可用
    ui->InputEmployeeNumFirm->setEnabled(false);//设置输入员工数量框不可用
    ui->inputHalfMaxParkingNum->setEnabled(false);//设置输入员工数量框不可用

    //实例化停车场类
    halfMaxParkingNum = ui->inputHalfMaxParkingNum->value();
    park = new parkClass(halfMaxParkingNum, scene);
    carWidth = park->getCarWidth();

    //实例化后台 M 管理系统
    employeeNum = ui->InputEmployeeNumFirm->value();
    MSingleTon* mPtr = MSingleTon::getInstance();//获取唯一实例
    mPtr->setEmployeeNum(employeeNum);//设置单例卡管理系统 M (单例模式)的有效员工数
    ui->view->show();//显示Graphics View组件
    ui->lcdNumber->show();//显示lcdNumber组件

    //获取当前时间
    QDateTime CurrentTime = QDateTime::currentDateTime();//获取系统现在的时间
    simulateStartTimeStr = CurrentTime.toString("yyyyMMdd-hhmmss");

    /**
     * 几何方式绘制停车场图
     * 参数：停车位数量/2; //场景坐标
    */
    park->drawParkPicture();//几何方式绘制停车场图
    ui->reportText->append(CurrentTime.toString("hh:mm:ss"));//将当前时间显示到显示框
    ui->reportText->append("============Start simulation============");
    //定时器开始工作
    fTimer->start();
}

//当点击停止仿真按钮，响应函数
void MainWindow::on_stopBut_clicked()
{
    ui->startBut->setEnabled(true);//设置开始仿真按钮可用
    ui->stopBut->setEnabled(false);//设置停止仿真按钮不可用
    ui->creatCarBut->setEnabled(false);//设置生成汽车按钮不可用
    ui->InputEmployeeNumFirm->setEnabled(true);//设置输入员工数量框可用
    ui->inputHalfMaxParkingNum->setEnabled(true);//设置输入员工数量框可用
    scene->clear();//清楚场景中的所有图形项
    ui->view->hide();//隐藏Graphics View组件
    ui->lcdNumber->hide();//隐藏lcdNumber组件
    fTimer->stop();//定时器停止
    calcRuningTime = 0;//累计运行时间置0

    ui->reportText->append(QDateTime::currentDateTime().toString("hh:mm:ss"));//将当前时间显示到显示框
    ui->reportText->append("============End simulation============");

    QString saveStr = ui->reportText->toPlainText();//获取显示框的所有内容
    saveMsgDataToFile(saveStr);//写入到文件
}


//点击生成汽车按钮事件
void MainWindow::on_creatCarBut_clicked()
{
    //生成随即数据
    QTime time;
    time = QTime::currentTime();
    qsrand(time.msec() + time.second()*1000);

    //随机生成 场内没有停放过的 employeeNum(有1/4几率生成 无效号码)
    qint32 randEmployeeID;
    do{
        randEmployeeID = qrand() % (employeeNum*4/3);//随机生成具有 employeeNum 133%的员工号，后33%为错误员工号
    }while(park->isEmployeeIDExisted(randEmployeeID));
    //随机生成停放时间
    qint32 randParkingTime = qrand() % 10 + 15;// 最多停放180秒
    //获取最近空余车位号
    qint32 theShortestCarSpaceNum = park->getCloseinSpaceNo();
    //若无空余位，退出
    if(theShortestCarSpaceNum == -1){
        return;
    }
    //生成一个汽车类
    carClass* aNewCar = new carClass(carWidth, halfMaxParkingNum,
                                     randEmployeeID, randParkingTime, calcRuningTime+randParkingTime,
                                     scene);
    //添加该实例到容器
    creatCarList.append(aNewCar);
    //绘制汽车图形项
    aNewCar->drawCarItem();
}

//槽函数: 定时器中断响应【每秒】
void MainWindow::on_timer_timout(){
    calcRuningTime += 1;// 运行时间 +1
    //获取当前时间
    QDateTime CurrentTime = QDateTime::currentDateTime();//获取系统现在的时间
    QString currentTimeStr = CurrentTime.toString("hh:mm:ss");

    showOrRefreshOnScreen(currentTimeStr, calcRuningTime);//更新界面左侧显示窗口信息
    simulateControlAllCar();//仿真控制所有生成车的运动的函数

    checkForAndSolveClash();//检测下1秒有无争道并处理(让弱势方延时1秒)

    autoCloseDoorFrontPreSec();//自动关前门
    autoCloseDoorBehindPreSec();//自动关后门


    //每10s//59
    if(min_counter > 59){
        //定时播报【每分钟】
        addReportMsgPreMin();
        min_counter = 0;
    }else{
        min_counter++;
    }
}

//仿真控制所有生成车的函数
void MainWindow::simulateControlAllCar()
{
    //1.每辆车进行一次检测是否停车时间到
    for(qint32 i=0; i<creatCarList.size(); i++){
        //判断是否停车时间到
        if(creatCarList[i]->ifChangeStatusToLeave(calcRuningTime)){
            ////将汽车状态改为出，重新设置目的坐标，【已在】ifChangeStatusToLeave()中实现
        }
    }

    //2.每辆汽车执行一次运动控制（或争道，Waiting -- 不控制）
    for(qint32 i=0; i<creatCarList.size(); i++){

        /*********************每辆车都执行以下操作**********************************/
        //【一】若到达入口传感器(1, 0)
        if(1 == creatCarList[i]->getNow_x() && 0 == creatCarList[i]->getNow_y()){
            //判断当前状态：

            qint8 Car_i_status = creatCarList[i]->getCarStatus();
            switch(Car_i_status){

                //1.若为正在检测状态
                case carClass::carStatusFlag::carChecking:
                    //到传感器，已等一秒，调用读卡器检测
                    ////检测本车 号码有效性
//                    if(park->checkEmployeeNum(creatCarList[i])){//后台连接唯一实例M判断
                    if(creatCarList[i]->getEmployeeID() < employeeNum){//直接判断
                        //a.若有效，分配一个最近空余车位，执行开门(过程量，有可能没完全打开)
                        creatCarList[i]->setParkSpaceNum(park->getCloseinSpaceNo());
                        ////开门，检测是否完全打开
                        if(park->getRailingFront()->drawOpen()){
                            //标记改位置已被占用
                            park->PreOccupyOneCarSpace(creatCarList[i]->getEmployeeID(),
                                                         creatCarList[i]->getParkSpaceNum(), creatCarList[i]->getParkingTime());
                            //若门已完全打开
                            //右移【走过传感器，防止再次触发】
                            creatCarList[i]->move_hor(1, carWidth);//右移
                            creatCarList[i]->Inc_Now_x();//相对坐标x 自加
                            //将状态改为进入状态
                            creatCarList[i]->setCarStatus(carClass::carStatusFlag::carIn);//标记恢复为进入
                            //触发前门传感器，有车离开
                            park->getSensorFront()->somethingPassedBy();
                        }
                    }
                    //b. 若为无效卡
                    else{
                        creatCarList[i]->setCarStatus(carClass::carStatusFlag::carRefuseed);//标记为不允许进入，等待回收
                        //告警
                        ui->reportText->append(QDateTime::currentDateTime().toString("hh:mm:ss"));//将当前时间显示到显示框
                        QString showStr;
                        showStr.sprintf("Error Comein: employee num %d is invaild!!!",creatCarList[i]->getEmployeeID());
                        ui->reportText->append(showStr);
                    }
                    break;

            //2.若为进入状态()
            case carClass::carStatusFlag::carIn:
                //刚到传感器，这次不操作，等一秒
                park->getSensorFront()->somethingArrived();//触发前门传感器，有车到
                //将车状态改为检测
                creatCarList[i]->setCarStatus(carClass::carStatusFlag::carChecking);
                break;
            }
        }

        //【二】若到达出口传感器(halfMaxParkingNum+3, 0)
        else if(halfMaxParkingNum+3 == (qint32)(creatCarList[i]->getNow_x()) && 0 == creatCarList[i]->getNow_y()){
            //判断当前状态
            switch (creatCarList[i]->getCarStatus()) {
                //2.若为出门状态()
                case (carClass::carStatusFlag::carOut):
                    //第一次到到后门传感器，这次不操作，等一秒
                    park->getSensorBehind()->somethingArrived();//触发后门传感器，有车到
                    //将车状态改为正在检测
                    creatCarList[i]->setCarStatus(carClass::carStatusFlag::carChecking);
                    break;

                //1.若为正在检测状态
                case (carClass::carStatusFlag::carChecking):
                    //到传感器，已等一秒，调用读卡器检测
                    ////检测本车 号码有效性
//                    if(park->checkEmployeeNum(creatCarList[i])){//后台连接唯一实例M判断
                    if(creatCarList[i]->getEmployeeID() < employeeNum){//直接判断
                        //a.若有效，执行开门(过程量，有可能没完全打开)
                        ////开门，并检测是否完全打开
                        if(park->getRailingBehind()->drawOpen()){
                            //若门已完全打开
                            // 右移【走过传感器，防止再次触发检测】
                            creatCarList[i]->move_hor(1, carWidth);//右移
                            creatCarList[i]->Inc_Now_x();//相对坐标x 自加
                            //将状态改为出门状态
                            creatCarList[i]->setCarStatus(carClass::carStatusFlag::carOut);//标记为离开
//                            ////触发后门传感器，有车离开
                            park->getSensorBehind()->somethingPassedBy();
                        }
                    }
                    //b. 若为无效卡
                    else{
                        creatCarList[i]->setCarStatus(carClass::carStatusFlag::carRefuseed);//标记为无效车，等待回收
                        //告警
                        QString showStr;
                        showStr.sprintf("Error ComeOut: employee num %d is invaild!!!",creatCarList[i]->getEmployeeID());
                        ui->reportText->append(showStr);
                    }
                    break;

                default:
                    break;
            }
        }

        //【三】到其他非前后门处
        else{
            creatCarList[i]->move_control_pre_second();
        }
    }


    //3.删除列表中已离开，或无效车停车场的汽车
    for(qint32 i=0; i<creatCarList.size(); i++){
        //判断汽车状态是否为已离开
        if(creatCarList[i]->ifcarStatusLeavedOrInvaild()){
            //只有正常离开的车才释放掉车位占用
            if(carClass::carStatusFlag::carLeaved == creatCarList[i]->getCarStatus()){
                //释放掉车位占用
                park->preFreeOneCarSpace(creatCarList[i]->getParkSpaceNum());
                //在场内停放汽车号集合内 删除改元素
                park->deleteAEmployNumInParkedEmployeeNumList(creatCarList[i]->getEmployeeID());
            }
            //删除画布中的该车
            scene->removeItem(creatCarList[i]->getCarBodyItem());//移除该车Body图形项
            scene->removeItem(creatCarList[i]->getCarNumItem());//移除该车的数字编号图形项

            //在本容器中删除 改生成汽车变量
            creatCarList.erase(creatCarList.begin() + i);//删除该生成的汽车元素
        }
    }
}

//自动控制关门（若车已通过传感器且门未完全关时）
void MainWindow::autoCloseDoorFrontPreSec()
{
    //传感器检测到若车已通过  且  门未完全关时
    if(park->getSensorFront()->getIsPassedBy() && !park->getRailingFront()->getIsFullyCoosed()){
        park->getRailingFront()->drawclose();//关前门
    }
}

//自动控制关后门（若车已通过传感器且门未完全关时）
void MainWindow::autoCloseDoorBehindPreSec()
{
    //传感器检测到若车已通过  且  门未完全关时
    if(park->getSensorBehind()->getIsPassedBy() && !park->getRailingBehind()->getIsFullyCoosed()){
        park->getRailingBehind()->drawclose();//关后门
    }
}

//将系统报告消息存到文件中
void MainWindow::saveMsgDataToFile(QString msg)
{
    QString fileDir = QApplication::applicationDirPath() + "/LogFile";
    //判断是否有LogFile文件夹
    //实例QDir
    QDir *folder = new QDir;
    //判断创建文件夹是否存在
    bool exist = folder->exists(fileDir);
    //如果不存在，创建文件夹
    if (!exist) {
        folder->mkdir(fileDir);//创建文件夹
    }

    //保存数据的文件路径
    QString fileName = fileDir + "/" + simulateStartTimeStr + ".TXT";
    QFile file(fileName);//初试化文件变量
    //若文件不存在，会自动新建
    file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);//追加写
    QTextStream txtOutPut(&file);

    txtOutPut << msg << endl;
    file.flush();
    file.close();//关闭文件

    ui->reportText->append("successfully save log file to:");
    ui->reportText->append(fileName);
}

//检测下1秒有无争道并处理(让弱势方延时1)
void MainWindow::checkForAndSolveClash()
{
    //对每辆车(i)进行检测:
    for(qint32 i=0; i<creatCarList.size(); i++){
        pointClass car_i_next_point = creatCarList[i]->getNext_Point();//当前车(i)的下一目标点相对坐标
        //与其他车(j)做冲突比较
        for(qint32 j=i+1; j<creatCarList.size(); j++){
            pointClass car_j_next_point = creatCarList[j]->getNext_Point();//其他车(j)的下一目标点相对坐标

            //【车(i) 与 车(j)冲突判断】
            if(car_j_next_point == car_i_next_point){
                //下一秒将会发生争道冲突, 将弱势方延时
                ui->reportText->append(QDateTime::currentDateTime().toString("hh:mm:ss"));//将当前时间显示到显示框
                QString showStr;

                //1. 出场车(i) vs 进场车(j)
                if(carClass::carStatusFlag::carOut == creatCarList[i]->getCarStatus()
                        && carClass::carStatusFlag::carIn == creatCarList[j]->getCarStatus()){
                    //出场车(i)先行（优势方），进场车(j)等待（劣势方）
                    creatCarList[j]->INC_WatingForClash();//等待1s
                    showStr.sprintf("Crash Error: car(%d) waitting for car(%d) to avoid!!!", creatCarList[j]->getEmployeeID(), creatCarList[i]->getEmployeeID());
                    ui->reportText->append(showStr);
                }
                //2. 进场车(i) vs 出场车(j)
                else if(carClass::carStatusFlag::carIn == creatCarList[i]->getCarStatus()
                        && carClass::carStatusFlag::carOut == creatCarList[j]->getCarStatus()){
                    //出场车(j)先行（优势方），进场车(i)等待（劣势方）
                    creatCarList[i]->INC_WatingForClash();//等待1s
                    showStr.sprintf("Crash Error: car(%d) waitting for car(%d) to avoid!!!", creatCarList[i]->getEmployeeID(), creatCarList[j]->getEmployeeID());
                    ui->reportText->append(showStr);
                }
                //3. 车(i)车(j)同为进场或出厂
                else {
                    //让编号小的(i)先行，车(j)等待
                    creatCarList[j]->INC_WatingForClash();//等待1s
                    showStr.sprintf("Crash Error: car(%d) waitting for car(%d) to avoid!!!", creatCarList[j]->getEmployeeID(), creatCarList[i]->getEmployeeID());
                    ui->reportText->append(showStr);
                }
            }
        }
    }
}
