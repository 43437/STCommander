#ifndef CONFIG_H
#define CONFIG_H

namespace NSConfig
{
enum EPortType{
    ePortType_From,
    ePortType_Serial,
    ePortType_Tcp,
    ePortType_To,
};

extern EPortType ePortType;
};

#endif // CONFIG_H
