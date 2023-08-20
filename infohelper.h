#ifndef INFOHELPER_H
#define INFOHELPER_H

#include <QObject>

class InfoHelper : public QObject
{
    Q_OBJECT
public:
    static InfoHelper& Instance();
    void Info(const QString& info);

private:
    InfoHelper(QObject* parent = nullptr);

signals:
    void SigInfo(QString info);
};

#endif // INFOHELPER_H
