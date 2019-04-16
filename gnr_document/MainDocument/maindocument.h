#ifndef MAINDOCUMENT_H
#define MAINDOCUMENT_H

#include "maindocument_global.h"
#include "../common_includes/commonIncludes.h"
#include "../PF_interface/PF_defines.h"
#include <vector>
#include <string>

namespace DocuAPI {


#ifdef __cplusplus
extern "C" {
#endif //cpp

DLLCAL void * CreateDocument();
DLLCAL t_int32 FreeDocument(void * doc);

#ifdef __cplusplus
} //end extern "C"
#endif //cpp
}

class C_VariableDataInterface;

class DLLCAL C_channel
{
public:
    enum EChanelData
    {
        eInt32 = 0,
        eUint32,
        eInt64,
        eUint64,
        eDouble,
        eByte
    };
    C_channel(std::vector<t_int32> vShape, EChanelData eDataType);
    virtual ~C_channel();

    virtual void getInterfaceAt(t_int32 iIndex, C_VariableDataInterface &aInterface);

private:
    t_int32 m_iDim = -1;
    std::vector<t_int32> m_vShape;
    t_uint64 m_uiBaseSize = 0;
    t_uint64 m_uiElementSize = 0;
    EChanelData m_eType = eInt32;
    t_byte * m_pData = nullptr;
    t_uint32 m_uiIncrement = 10;
    t_uint64 m_uiCount = 0;
};

class DLLCAL C_VariableDataInterface
{
public:
    C_VariableDataInterface(std::vector<t_int32> vShape, C_channel::EChanelData eDataType);
    virtual ~C_VariableDataInterface();
    virtual void setData(void * pData, t_uint64 uiSize);
    //virtual getPoint(std::vector<t_int32> vShape);

private:
    t_int32 m_iDim = -1;
    std::vector<t_int32> m_vShape;
    t_uint32 m_uiBaseSize = 0;
    t_uint64 m_uiElementSize = 0;
    C_channel::EChanelData m_eType = C_channel::eInt32;
    void * m_pData = nullptr;
};

class DLLCAL C_event
{
public:
    C_event();
    virtual ~C_event();

    double dTime = 0;
};

class DLLCAL C_timeAxis
{
public:
    C_timeAxis();
    virtual ~C_timeAxis();

    virtual void addEvent(C_event & event);
    virtual C_event *getEvent(t_int32 iOrder);
    virtual C_event *getEvent(double dTime);

private:
    std::vector<C_event> m_vEvents;
    std::vector<t_int32> m_vOrder;
};

class DLLCAL MainDocument
{

public:
    MainDocument();
    virtual ~MainDocument();

    C_timeAxis m_axis;
    std::string m_sName;
    std::string m_sPath;
};

#endif // MAINDOCUMENT_H
