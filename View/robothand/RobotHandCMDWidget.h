#ifndef __ROBOT_HAND_CMD_WIDGET_230820_H__
#define __ROBOT_HAND_CMD_WIDGET_230820_H__

#include <QWidget>
#include "Common/datatypedef.h"

class QComboBox;
class QPushButton;
class QLineEdit;
class QSlider;
class RobotHandCMDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RobotHandCMDWidget(QWidget* parent = nullptr);
    void SetDegree(const uint8_t degree);
    void SetMotorID(const NSDataTypeDef::EMotorID motorID);

private:
    void Init();
    void AttachConnection();
    void SendMotorCMD();

private:
    QComboBox* m_cbMotorID;
    QPushButton* m_btnSend;
    QLineEdit* m_edtDegree;
    QSlider* m_sldrDegree;
};

#endif
