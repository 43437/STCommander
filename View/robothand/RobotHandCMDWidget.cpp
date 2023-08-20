#include "RobotHandCMDWidget.h"
#include "Common/CommonFunction.h"
#include "Msg/Protocol.h"
#include "msgprocess.h"
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QSlider>

RobotHandCMDWidget::RobotHandCMDWidget(QWidget* parent /*= nullptr*/) : QWidget(parent)
{
    Init();
}

void RobotHandCMDWidget::Init()
{
    m_cbMotorID = new QComboBox;
    m_edtDegree = new QLineEdit;
    m_btnSend = new QPushButton;
    m_sldrDegree = new QSlider(Qt::Horizontal);

    for (int i = NSDataTypeDef::eMotorID_From; i < NSDataTypeDef::eMotorID_To; ++i)
    {
        m_cbMotorID->addItem(QString::number(i), i);
    }

    m_edtDegree->setValidator(new QIntValidator(NSDataTypeDef::MOTOR_DEGREE_MIN,
                                                NSDataTypeDef::MOTOR_DEGREE_MAX));
    m_btnSend->setText("send");

    m_sldrDegree->setMaximum(NSDataTypeDef::MOTOR_DEGREE_MAX);
    m_sldrDegree->setMinimum(NSDataTypeDef::MOTOR_DEGREE_MIN);

    auto mainLayout = new QHBoxLayout;
    this->setLayout(mainLayout);

    mainLayout->addWidget(m_cbMotorID);
    mainLayout->addWidget(m_edtDegree);
    mainLayout->addWidget(m_sldrDegree);
    mainLayout->addWidget(m_btnSend);
    mainLayout->addStretch();

    AttachConnection();
}

void RobotHandCMDWidget::AttachConnection()
{
    connect(m_btnSend, &QPushButton::clicked, this, [=](){

        SendMotorCMD();
    });

    connect(m_sldrDegree, &QAbstractSlider::valueChanged, this, [=](int val){
        m_edtDegree->setText(QString::number(val));
    });
}

void RobotHandCMDWidget::SendMotorCMD()
{
    auto motorID = m_cbMotorID->currentData().toUInt();
    auto degree = m_edtDegree->text().toUInt();

    QByteArray arrParam;
    arrParam.append(motorID);
    arrParam.append(degree);

    QByteArray arrCMD;
    MsgProcess::BuildCMD(NSProtocol::TYPE_CMD, MsgProcess::GetSN(), NSProtocol::CMDID.PWM_N_DEGREE, arrParam, arrCMD);
    MsgProcess::Instance().Send(arrCMD);
}

void RobotHandCMDWidget::SetDegree(const uint8_t degree)
{
    if (NSCommonFunction::IsValidMotorDegree(degree))
    {
        m_edtDegree->setText(QString::number(degree));
        m_sldrDegree->setValue(degree);
    }
}

void RobotHandCMDWidget::SetMotorID(const NSDataTypeDef::EMotorID motorID)
{
    auto index = m_cbMotorID->findData(motorID);
    m_cbMotorID->setCurrentIndex(index);
}
