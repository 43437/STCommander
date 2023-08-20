#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H

#include <QDebug>

#define KOT_LOG qDebug() << QString("[%1:%2]").arg(__FUNCTION__).arg(__LINE__)

#include <QString>

namespace NSCommonFunction {

    QString GetAppPath();
    QString GetLogPath();

    bool IsValidMotorDegree(const uint8_t degree);

    template<typename T>
    QString ToHexStr(const T t)
    {
        return QString("%1").arg(t, 2, 16, QChar('0')).last(2);
    }
}

#endif // COMMONFUNCTION_H
