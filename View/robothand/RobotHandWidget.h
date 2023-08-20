#ifndef __ROBOT_HAND_2308201455_H__
#define __ROBOT_HAND_2308201455_H__

#include <QWidget>
#include "Common/datatypedef.h"

class RobotHandCMDWidget;
class RobotHandWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RobotHandWidget(QWidget* parent = nullptr);

private:
    void Init();

private:
    QMap<NSDataTypeDef::EMotorID, RobotHandCMDWidget*> m_mpCMDWidget;
};

#endif
