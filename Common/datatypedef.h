
#ifndef DATATYPEDEF_H
#define DATATYPEDEF_H

#include <QString>

namespace NSDataTypeDef
{
    extern const QString MARK_SPACE;
    extern const uint8_t MOTOR_DEGREE_MIN;
    extern const uint8_t MOTOR_DEGREE_MAX;

    enum EMotorID : int
    {
        eMotorID_From,
        eMotorID_1 = eMotorID_From,
        eMotorID_2,
        eMotorID_3,
        eMotorID_4,
        eMotorID_5,
        eMotorID_6,
        eMotorID_To,
    };
};

#endif // DATATYPEDEF_H
