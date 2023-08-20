#include "CommonFunction.h"
#include "Common/datatypedef.h"
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

    bool IsValidMotorDegree(const uint8_t degree)
    {
        return (degree >= NSDataTypeDef::MOTOR_DEGREE_MIN
                && degree <= NSDataTypeDef::MOTOR_DEGREE_MAX);
    }
};
