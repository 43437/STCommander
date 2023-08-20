#ifndef __PORT_BASE_2308192004_H__
#define __PORT_BASE_2308192004_H__

#include <QObject>

class PortBase : public QObject
{
    Q_OBJECT
public:
    PortBase(QObject* parent = nullptr);
    virtual void Send(const QByteArray& dat) = 0;
    virtual bool StartProcess(const QVariant& var) = 0;

signals:
    void SigRecv(QByteArray dat);
};

#endif
