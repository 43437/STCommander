#include "SerialPort.h"
#include <QSerialPort>
#include <QVariant>
#include "Common/CommonFunction.h"

SerialPort::SerialPort(QObject* parent/* = nullptr*/) : PortBase(parent)
{
    Init();
}

void SerialPort::Send(const QByteArray& dat)
{
    m_serial->write(dat);
}

bool SerialPort::StartProcess(const QVariant& var)
{
    bool bRet = true;
    auto portName = var.toString();

    if (m_serial->isOpen())
    {
        m_serial->clear();
        m_serial->close();
    }

    m_serial->setPortName(portName);
    if (!m_serial->open(QIODevice::ReadWrite))
    {
        KOT_LOG << "open failed" << portName;
        bRet = false;
    }
    else
    {
        m_serial->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
        m_serial->setDataBits(QSerialPort::Data8);
        m_serial->setFlowControl(QSerialPort::NoFlowControl);
        m_serial->setParity(QSerialPort::NoParity);
        m_serial->setStopBits(QSerialPort::OneStop);

        connect(m_serial, &QSerialPort::readyRead, this, [=](){
            emit SigRecv(m_serial->readAll());
        });
    }

    return bRet;
}

void SerialPort::Init()
{
    m_serial = new QSerialPort(this);
}
