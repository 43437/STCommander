#include "CommonFunction.h"
#include <QCoreApplication>

namespace NSCommonFunction {

    QString GetAppPath()
    {
        return QCoreApplication::applicationDirPath();
    }

    QString GetLogPath()
    {
        return GetAppPath() + "/Logs";
    }
};
