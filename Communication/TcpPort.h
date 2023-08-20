#ifndef __TCP_PORT_2308191958_H__
#define __TCP_PORT_2308191958_H__

#include "PortBase.h"
#include <QList>

class QTcpServer;
class QTcpSocket;
class TcpPort : public PortBase
{
    Q_OBJECT
public:
    TcpPort(QObject* parent = nullptr);
    virtual void Send(const QByteArray& dat);
    virtual bool StartProcess(const QVariant& var);

private:
    void Init();
    void CloseServer();

private:
    QTcpServer* m_tcpServer;
    QList<QTcpSocket*> m_lstTcpSocket;
};

#endif
