
#include "infohelper.h"

InfoHelper::InfoHelper(QObject* parent/* = nullptr*/) : QObject(parent)
{

}

InfoHelper& InfoHelper::Instance()
{
    static InfoHelper __instance;
    return __instance;
}

void InfoHelper::Info(const QString& info)
{
    emit SigInfo(info);
}
