
#ifndef MSGPROCESS_H
#define MSGPROCESS_H

#include "Msg/CMsgParser.h"
#include <QObject>
#include <QList>
#include "Communication/PortBase.h"

class QSerialPort;
class QTcpServer;
class QTcpSocket;
class MsgProcess : public QObject
{
    Q_OBJECT
public:
    static MsgProcess& Instance();
    bool StartProcess(const QVariant& param);
    void Send(const QByteArray& dat);
    uint8_t Send(const uint8_t cmd, const QByteArray& arrParam);
    static uint8_t GetSN();
    static void BuildCMD(const uint8_t type, const uint8_t sn, const uint8_t cmd, const QByteArray& vecParam, QByteArray& vecCMD);
    static int MSGEndDiff(const int paramLen);
    static uint8_t CRC8Check(const std::vector<uint8_t>& vecData);
    static int CRCDiff(const int paramLen);

signals:
    void SigRecv(const NSProtocol::SCMD stuCMD);

private:
    explicit MsgProcess(QObject *parent = nullptr);
    void Init();
    void HandleDat(const QByteArray& dat);

private:
    CMsgParser objMsgParser;
    PortBase* m_Port;
};

Q_DECLARE_METATYPE(NSProtocol::SCMD)

#endif // MSGPROCESS_H
