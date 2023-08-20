
#include "dynamicqmlwidget.h"
#include "msgprocess.h"
#include "Common/datatypedef.h"
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQuickItem>

const QString CMDProxy::KEY_CMD("cmd");
const QString CMDProxy::KEY_PARAM("param");
const QString CMDProxy::KEY_SN("sn");
const QString CMDProxy::KEY_TYPE("type");
CMDProxy::CMDProxy(QObject* parent /*= nullptr*/) : QObject(parent)
{

}

uint8_t CMDProxy::sendCMD(const QString& jsonCMD)
{
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonCMD.toLatin1(), &jsonError);
    auto jsonObj = doc.object();
    auto cmdVal = jsonObj.value(KEY_CMD);

    bool bOk = true;
    uint8_t cmd = 0;
    QByteArray arrParam;
    if (!cmdVal.isUndefined()
        && cmdVal.isString())
    {
        cmd = cmdVal.toString().toUInt(&bOk, 16);
    }

    auto paramVal = jsonObj.value(KEY_PARAM);
    if (!paramVal.isUndefined()
        && paramVal.isString())
    {
        auto lstParam = paramVal.toString().split(NSDataTypeDef::MARK_SPACE);
        for (auto& param : lstParam)
        {
            arrParam.append((uint8_t)param.toUInt(&bOk, 16));
        }
    }

    if (bOk)
    {
        return MsgProcess::Instance().Send(cmd, arrParam);
    }
    else
    {
        return 0;
    }
}

DynamicQMLWidget::DynamicQMLWidget(const QString& fileName, QWidget *parent)
    : QWidget{parent}
    , m_fileName(fileName)
{
    m_url = QUrl::fromLocalFile(m_fileName);
    m_cmdProxy = new CMDProxy(this);
    Init();

    LoadView();
}

DynamicQMLWidget::~DynamicQMLWidget()
{

}

void DynamicQMLWidget::Init()
{
    m_pQQuickWidget = new QQuickWidget;
    m_pMainLayout = new QVBoxLayout;
    this->setLayout(m_pMainLayout);
    m_pMainLayout->addWidget(m_pQQuickWidget);

    AttachConnection();
}

void DynamicQMLWidget::AttachConnection()
{
    connect(&MsgProcess::Instance(), &MsgProcess::SigRecv, this, [this](const NSProtocol::SCMD stuCMD){
        OnRecvMsg(stuCMD);
    });
}

void DynamicQMLWidget::OnRecvMsg(const NSProtocol::SCMD& stuCMD)
{
    QVariantHash varHash;
    varHash[CMDProxy::KEY_CMD] = stuCMD.m_cmd;
    varHash[CMDProxy::KEY_TYPE] = stuCMD.m_type;
    varHash[CMDProxy::KEY_SN] = stuCMD.m_sn;

    QList<QVariant> lstParam(stuCMD.m_vecParam.begin(), stuCMD.m_vecParam.end());
    varHash[CMDProxy::KEY_PARAM] = QVariant(lstParam);

    QJsonObject objJson = QJsonObject::fromVariantHash(varHash);
    QJsonDocument doc;
    doc.setObject(objJson);

    QMetaObject::invokeMethod(m_pQQuickWidget->rootObject(), "onRecvMsg", Qt::AutoConnection,
                              Q_ARG(QString, doc.toJson()));
}

void DynamicQMLWidget::LoadView()
{
    delete m_pQQuickWidget;
    m_pQQuickWidget = new QQuickWidget;
    m_pQQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_pMainLayout->insertWidget(0, m_pQQuickWidget);
    m_pQQuickWidget->setSource(m_url);

    m_pQQuickWidget->rootContext()->setContextProperty("MsgProxy", m_cmdProxy);
}
