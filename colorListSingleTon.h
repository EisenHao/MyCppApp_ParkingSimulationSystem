#ifndef COLORLISTCLASS_H
#define COLORLISTCLASS_H
#include "qcolor.h"
#include "colorListSingleTon.h"
#include "QtGlobal"
#include "qdatetime.h"
/**
 * 颜色列表类
 * 选择常用的颜色
*/

class colorListSingleTon{
private:
    colorListSingleTon();//构造函数(私有)
    static colorListSingleTon* m_pInstance;//唯一实例
    QList<QColor> colorList;//选定的颜色列表
public:

    //获取单例 实例
    static colorListSingleTon* getInstance();
    //销毁实例
    void Release();
    //获取一种随机颜色
    QColor getARandColor();
};

#endif // COLORLISTCLASS_H
