#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H

#include <QDebug>

#define KOT_LOG qDebug() << QString("[%1:%2]").arg(__FUNCTION__).arg(__LINE__)

#include <QString>

namespace NSCommonFunction {

    QString GetAppPath();
    QString GetLogPath();
}

#endif // COMMONFUNCTION_H
