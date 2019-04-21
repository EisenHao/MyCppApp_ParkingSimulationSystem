#ifndef MSINGLETON_H
#define MSINGLETON_H
#include "QtGlobal"

/** 卡管理系统 M (单例模式)
 * 接口：验证卡的有效性
*/

class MSingleTon{
private:
    MSingleTon();//构造函数（私有）
    static MSingleTon* m_pInstance;//唯一实例
    qint32 EmployeeNum;//员工数（验证有效ID不能超过EmployeeNum）

public:
    //获取单例 实例
    static MSingleTon* getInstance();
    //销毁实例
    void Release();
    void setEmployeeNum(qint32 val);//设置有效员工数
    bool checkIDValid(qint32 IDNum);//后台检测IDNum的有效性
};

#endif // MSINGLETON_H
