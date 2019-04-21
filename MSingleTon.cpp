#include "MSingleTon.h"

/**卡管理系统 M
 * 接口：验证卡的有效性
 * */

// 静态成员变量需要在类体的外面进行初始化
MSingleTon* MSingleTon::m_pInstance = NULL;


//构造函数（私有）
MSingleTon::MSingleTon()
{}

//获取单例 实例
MSingleTon *MSingleTon::getInstance(){
    if(m_pInstance == NULL){
        MSingleTon* pIn = new MSingleTon();
        m_pInstance = pIn;
    }
    return m_pInstance;
}

//销毁实例
void MSingleTon::Release()
{
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//设置有效员工数
void MSingleTon::setEmployeeNum(qint32 val)
{
    this->EmployeeNum = val;
}

//后台检测IDNum的有效性
bool MSingleTon::checkIDValid(qint32 IDNum)
{
    if(IDNum < EmployeeNum){
        return true;
    }
    return false;
}
