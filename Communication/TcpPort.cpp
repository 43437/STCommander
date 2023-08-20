#include "TcpPort.h"
#include <QTcpSocket>
#include <QTcpServer>
#include "Common/CommonFunction.h"
#include "infohelper.h"

TcpPort::TcpPort(QObject* parent/* = nullptr*/) : PortBase(parent)
{
    Init();
}

void TcpPort::Send(const QByteArray& dat)
{
    for (auto& socket : m_lstTcpSocket){
        socket->write(dat);
    }
}

bool TcpPort::StartProcess(const QVariant& var)
{
    bool bOk = true;
    auto port = var.toInt(&bOk);
    if (!bOk)
    {
        KOT_LOG << "invalid " << var;
    }
    else
    {
        if (m_tcpServer->isListening())
        {
            CloseServer();
        }

        if (m_tcpServer->listen(QHostAddress::Any, port))
        {
            connect(m_tcpServer, &QTcpServer::newConnection, this, [=](){

                while (m_tcpServer->hasPendingConnections())
                {
                    auto socket = m_tcpServer->nextPendingConnection();
                    m_lstTcpSocket.append(socket);

                    QString connInfo = QString("connect from %1 %2").arg(socket->peerAddress().toString())
                                           .arg(socket->peerPort());
                    InfoHelper::Instance().Info(connInfo);

                    connect(socket, &QTcpSocket::readyRead, this, [=](){

                        emit SigRecv(socket->readAll());
                    });

                    connect(socket, &QAbstractSocket::errorOccurred, this, [=](){

                        KOT_LOG << socket->peerAddress().toString()
                                << socket->peerPort()
                                << socket->errorString();
                    });

                    connect(socket, &QTcpSocket::disconnected, this, [=](){

                        socket->deleteLater();
                        m_lstTcpSocket.removeAll(socket);
                    });
                }
            });
        }
        else
        {
            bOk = false;
            KOT_LOG << "tcp server listen failed";
        }
    }

    return bOk;
}

void TcpPort::Init()
{
    m_tcpServer = new QTcpServer(this);
}

void TcpPort::CloseServer()
{
    KOT_LOG;
    m_tcpServer->close();
    for (auto& socket : m_lstTcpSocket)
    {
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState)
        {
            socket->abort();
        }
    }
}
