
#include "qmlinwidget.h"
#include "Common/CommonFunction.h"
#include "dynamicqmlwidget.h"
#include <QTabWidget>
#include <QHBoxLayout>
#include <QDir>

QmlInWidget::QmlInWidget(QWidget *parent)
    : QWidget{parent}
{
    Init();
}

void QmlInWidget::Init()
{
    m_pTabWidget = new QTabWidget;
    auto pLayout = new QHBoxLayout;
    this->setLayout(pLayout);
    pLayout->addWidget(m_pTabWidget);

    auto qmlPath = NSCommonFunction::GetAppPath() + "/qml";

    QDir dir(qmlPath);
    auto lstFile = dir.entryList({"*.qml"}, QDir::Files | QDir::NoDotAndDotDot);
    for (auto& qmlFile : lstFile)
    {
        auto fullFileName = qmlPath + "/" + qmlFile;
        auto* pWidget = new DynamicQMLWidget(fullFileName);
        qmlFile.chop(4);
        m_pTabWidget->addTab(pWidget, qmlFile);
    }
}
