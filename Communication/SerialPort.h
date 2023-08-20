#ifndef __SERIAL_PORT_2308191959_H__
#define __SERIAL_PORT_2308191959_H__

#include "PortBase.h"

class QSerialPort;
class SerialPort : public PortBase
{
    Q_OBJECT
public:
    SerialPort(QObject* parent = nullptr);

    virtual void Send(const QByteArray& dat);
    virtual bool StartProcess(const QVariant& var);

private:
    void Init();

private:
    QSerialPort* m_serial;
};

#endif
