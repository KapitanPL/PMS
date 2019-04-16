#include "maindocument.h"


MainDocument::MainDocument()
{
}

MainDocument::~MainDocument()
{

}

void *CreateDocument()
{
    return new MainDocument();
}

t_int32 DocuAPI::FreeDocument(void *doc)
{
    MainDocument *pDoc = static_cast<MainDocument*>(doc);
    if (pDoc)
    {
        delete pDoc;
        return RES_OK;
    }
    else
        return RES_ERR_FAIL;
}

C_timeAxis::C_timeAxis()
{

}

C_timeAxis::~C_timeAxis()
{

}

void C_timeAxis::addEvent(C_event &event)
{
    if (m_vEvents.size() == m_vOrder.size())
    {
        m_vEvents.push_back(event);
        t_int32 iOrder = 0;
        for (auto & itOrder : m_vOrder)
        {
            if (m_vEvents[itOrder].dTime < event.dTime)
            {
                iOrder++;
            }
            else
            {
                itOrder++;
            }
        }
        m_vOrder.push_back(iOrder);
    }
}

C_event * C_timeAxis::getEvent(t_int32 iOrder)
{
    return &m_vEvents[m_vOrder[iOrder]];
}

C_event *C_timeAxis::getEvent(double dTime)
{
    double dDiff = std::numeric_limits<double>::max();
    double dLastDiff = std::numeric_limits<double>::max();
    auto itReturn = 0;
    for (auto itOrder : m_vOrder)
    {
        dDiff = std::abs(m_vEvents[itOrder].dTime - dTime);
        if (dTime < dLastDiff)
        {
            dLastDiff = dDiff;
            itReturn = itOrder;
        }
        else
            break;
    }
    return &m_vEvents[itReturn];
}

C_event::C_event()
{

}

C_event::~C_event()
{

}

C_channel::C_channel(std::vector<t_int32> vShape, EChanelData eDataType)
    : m_vShape(vShape)
    , m_eType(eDataType)
{
    m_iDim = (t_int32)m_vShape.size();
    switch (eDataType)
    {
    case eInt32:
    {
        m_uiBaseSize = sizeof(t_int32);
        break;
    }
    case eInt64:
    {
        m_uiBaseSize = sizeof(t_int64);
        break;
    }
    case eUint32:
    {
        m_uiBaseSize = sizeof(t_uint32);
        break;
    }
    case eUint64:
    {
        m_uiBaseSize = sizeof(t_uint64);
        break;
    }
    case eDouble:
    {
        m_uiBaseSize = sizeof(double);
        break;
    }
    case eByte:
    {
        m_uiBaseSize = 1;
        break;
    }
    }
    t_uint64 uiFactor = 1;
    for (auto itDim : m_vShape)
    {
        uiFactor *= itDim;
    }
    m_uiElementSize = m_uiBaseSize * uiFactor;
}

void C_channel::getInterfaceAt(t_int32 iIndex, C_VariableDataInterface &aInterface)
{
    if (iIndex >= 0 && iIndex < (t_int32)m_uiCount)
    {
        aInterface.setData(m_pData + iIndex * m_uiElementSize, m_uiElementSize);
    }
    if (iIndex < 0 && std::abs(iIndex) <= (t_int32)m_uiCount )
    {
        aInterface.setData(m_pData + ((t_int32)m_uiCount + iIndex) * m_uiElementSize, m_uiElementSize);
    }
}

C_VariableDataInterface::C_VariableDataInterface(std::vector<t_int32> vShape, C_channel::EChanelData eDataType)
    : m_vShape(vShape)
    , m_eType(eDataType)
{
    m_iDim = (t_int32)m_vShape.size();
    switch (eDataType)
    {
    case C_channel::eInt32:
    {
        m_uiBaseSize = sizeof(t_int32);
        break;
    }
    case C_channel::eInt64:
    {
        m_uiBaseSize = sizeof(t_int64);
        break;
    }
    case C_channel::eUint32:
    {
        m_uiBaseSize = sizeof(t_uint32);
        break;
    }
    case C_channel::eUint64:
    {
        m_uiBaseSize = sizeof(t_uint64);
        break;
    }
    case C_channel::eDouble:
    {
        m_uiBaseSize = sizeof(double);
        break;
    }
    case C_channel::eByte:
    {
        m_uiBaseSize = 1;
        break;
    }
    }
    t_uint64 uiFactor = 1;
    for (auto itDim : m_vShape)
    {
        uiFactor *= itDim;
    }
    m_uiElementSize = m_uiBaseSize * uiFactor;
}

C_VariableDataInterface::~C_VariableDataInterface()
{

}

void C_VariableDataInterface::setData(void *pData, t_uint64 uiSize)
{
    if (uiSize == m_uiBaseSize)
    {
        m_pData = pData;
    }
}
