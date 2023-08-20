#include "RobotHandWidget.h"
#include "RobotHandCMDWidget.h"
#include <QVBoxLayout>

RobotHandWidget::RobotHandWidget(QWidget* parent/* = nullptr*/) : QWidget(parent)
{
    Init();
}

void RobotHandWidget::Init()
{
    auto mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    const uint defaultDegree = 90;
    for (int i = NSDataTypeDef::eMotorID_From; i < NSDataTypeDef::eMotorID_To; ++i)
    {
        auto motorID = (NSDataTypeDef::EMotorID)i;
        auto pCMDWidget = new RobotHandCMDWidget(this);

        m_mpCMDWidget[motorID] = pCMDWidget;
        pCMDWidget->SetMotorID(motorID);
        pCMDWidget->SetDegree(defaultDegree);

        mainLayout->addWidget(pCMDWidget);
    }
    mainLayout->addStretch();
}
