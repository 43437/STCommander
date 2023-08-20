
#ifndef DYNAMICQMLWIDGET_H
#define DYNAMICQMLWIDGET_H


#include <QWidget>
#include <QUrl>
#include "Msg/Protocol.h"

class QQuickWidget;
class QVBoxLayout;

class CMDProxy : public QObject
{
    Q_OBJECT
public:
    explicit CMDProxy(QObject* parent = nullptr);
    Q_INVOKABLE uint8_t sendCMD(const QString& jsonCMD);

public:
    const static QString KEY_CMD;
    const static QString KEY_PARAM;
    const static QString KEY_SN;
    const static QString KEY_TYPE;
};

class DynamicQMLWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DynamicQMLWidget(const QString& fileName, QWidget *parent = nullptr);
    virtual ~DynamicQMLWidget();

private:
    void Init();
    void LoadView();
    void AttachConnection();
    void OnRecvMsg(const NSProtocol::SCMD& stuCMD);

signals:

private:
    QString m_fileName;
    QQuickWidget* m_pQQuickWidget;
    QUrl m_url;
    QString m_defaultPath;
    QVBoxLayout* m_pMainLayout;
    CMDProxy* m_cmdProxy;
};

#endif // DYNAMICQMLWIDGET_H
