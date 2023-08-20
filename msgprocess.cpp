#include "msgprocess.h"
#include "Communication/SerialPort.h"
#include "Communication/TcpPort.h"
#include "Common/CommonFunction.h"
#include "Config.h"

MsgProcess::MsgProcess(QObject *parent)
    : QObject{parent}
{
    Init();
}

MsgProcess& MsgProcess::Instance()
{
    static MsgProcess __instance;
    return __instance;
}

bool MsgProcess::StartProcess(const QVariant& param)
{
    return m_Port->StartProcess(param);
}

void MsgProcess::Init()
{
    switch (NSConfig::ePortType)
    {
    case NSConfig::ePortType_Serial:
        m_Port = new SerialPort(this);
        break;
    case NSConfig::ePortType_Tcp:
        m_Port = new TcpPort(this);
        break;
    default:
        KOT_LOG << "invalid port type";
        break;
    }

    connect(m_Port, &PortBase::SigRecv, this, [=](QByteArray dat){

        HandleDat(dat);
    });

    objMsgParser.SetCMDRecvHandle([this](const NSProtocol::SCMD& stuCMD){

        emit SigRecv(stuCMD);
    });
}

void MsgProcess::Send(const QByteArray& dat)
{
    m_Port->Send(dat);
}

uint8_t MsgProcess::Send(const uint8_t cmd, const QByteArray& arrParam)
{
    QByteArray arrCMD;
    uint8_t sn = GetSN();
    MsgProcess::BuildCMD(NSProtocol::TYPE_CMD, sn, cmd, arrParam, arrCMD);
    MsgProcess::Instance().Send(arrCMD);
    return sn;
}

void MsgProcess::HandleDat(const QByteArray& dats)
{
    std::vector<uint8_t> vecDat;
    for (auto& dat : dats)
    {
        vecDat.push_back((uint8_t)dat);
    }
    objMsgParser.Cache(vecDat);
    objMsgParser.LoopOnce();
}


int MsgProcess::CRCDiff(const int paramLen)
{
    const int diff = NSProtocol::MSG_PARAM_DIFF;
    if (paramLen < 0)
    {
        //error len
    }
    else
    {
        return diff + paramLen;
    }
    return diff;
}

uint8_t MsgProcess::CRC8Check(const std::vector<uint8_t>& vecData)
{
    uint8_t crc = 0;

    for (auto& dat : vecData)
    {
        crc ^= dat;
        for (int i = 0; i < 8; ++i)
        {
            if ((crc & 0x01) != 0)
            {
                crc >>= 1;
                crc ^= 0x8c;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

int MsgProcess::MSGEndDiff(const int paramLen)
{
    return CRCDiff(paramLen) + NSProtocol::MSG_CRC_SIZE;
}

uint8_t MsgProcess::GetSN()
{
    static uint8_t sn = 1;
    if (sn == 0)
    {
        ++sn;
    }
    return sn++;
}

void MsgProcess::BuildCMD(const uint8_t type, const uint8_t sn, const uint8_t cmd, const QByteArray& vecParam, QByteArray& vecCMD)
{
    const uint8_t paramCnt = vecParam.size();
    const int len = NSProtocol::MSG_MIN_SIZE + paramCnt;
    vecCMD.resize(len);
    vecCMD[NSProtocol::MSG_HEAD_DIFF] = NSProtocol::MSG_HEAD;
    vecCMD[NSProtocol::MSG_TYPE_DIFF] = type;
    vecCMD[NSProtocol::MSG_SN_DIFF] = sn;
    vecCMD[NSProtocol::MSG_PARAM_LEN_DIFF] = paramCnt;
    vecCMD[NSProtocol::MSG_CMD_DIFF] = cmd;
    std::copy(vecParam.begin(), vecParam.end(), vecCMD.begin() + NSProtocol::MSG_PARAM_DIFF);

    uint8_t crcDiff = (uint8_t)CRCDiff((int)paramCnt);
    std::vector<uint8_t> vecCrcDat;
    vecCrcDat.push_back(cmd);
    vecCrcDat.insert(vecCrcDat.end(), vecParam.begin(), vecParam.end());
    vecCMD[crcDiff] = CRC8Check(vecCrcDat);

    uint8_t endDiff = (uint8_t)MSGEndDiff((int)paramCnt);
    vecCMD[endDiff] = NSProtocol::MSG_END;
}
