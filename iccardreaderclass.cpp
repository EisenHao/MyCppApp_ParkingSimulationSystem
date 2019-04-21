#include "ICcardReaderClass.h"

//构造函数
ICcardReaderClass::ICcardReaderClass()
{
    readEmployeeID = -1;
}

//读取IC卡号
void ICcardReaderClass::readICNumber(carClass* car)
{
    readEmployeeID = car->getEmployeeID();
}

//连接M以验证读取卡的有效性
bool ICcardReaderClass::connectToMTocheck()
{
    MSingleTon* m_P = MSingleTon::getInstance();//获取唯一实例
    return (m_P->checkIDValid(readEmployeeID));
}


