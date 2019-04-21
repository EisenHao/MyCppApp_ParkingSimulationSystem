#include "colorListSingleTon.h"

// 静态成员变量需要在类体的外面进行初始化
colorListSingleTon* colorListSingleTon::m_pInstance = NULL;

//构造函数（私有）
colorListSingleTon::colorListSingleTon(){
    colorList.append(QColor(3, 168, 158));//锰蓝
    colorList.append(QColor(255, 99, 71));//番茄红
    colorList.append(QColor(255, 235, 205));//杏仁灰
    colorList.append(QColor(255, 105, 180));//HotPink
    colorList.append(QColor(50, 205, 50));//LimeGreen
    colorList.append(QColor(64, 224, 205));//青绿色
    colorList.append(QColor(255, 128, 0));//橘黄
    colorList.append(QColor(192, 192, 192));//灰色
    colorList.append(QColor(218, 112, 214));//淡紫色
    colorList.append(QColor(238, 233, 233));//灰白：地板
}

//获取单例 实例
colorListSingleTon *colorListSingleTon::getInstance(){
    if(m_pInstance == NULL){
        colorListSingleTon* pIn = new colorListSingleTon();
        m_pInstance = pIn;
    }
    return m_pInstance;
}

//销毁实例
void colorListSingleTon::Release(){
    if(m_pInstance != NULL){
        colorList.clear();
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//获取一种随机颜色
QColor colorListSingleTon::getARandColor()
{
    QTime time;
    time = QTime::currentTime();
    qsrand(time.msec() + time.second()*1000);
    qint32 randNum = qrand() % (colorList.size()-1);//生成颜色下标 0 ~ size-1
    return colorList[randNum];
}
