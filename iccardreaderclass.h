#ifndef ICCARDREADERCLASS_H
#define ICCARDREADERCLASS_H
#include "carclass.h"
#include "MSingleTon.h"

/**
 * IC读卡器类
*/

class ICcardReaderClass{
private:
    qint32 readEmployeeID; //读取的驾驶者的ID卡号
public:
    ICcardReaderClass();//构造函数
    void readICNumber(carClass* car);//读取IC卡号
    bool connectToMTocheck();//连接M以验证读取卡的有效性
};

#endif // ICCARDREADERCLASS_H
