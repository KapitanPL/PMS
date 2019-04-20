#include "PF_structs.h"

#include "string.h"

void InitDataTransfer(S_DataTransfer * data)
{
    if (data)
    {
    strcpy((char*)data->sName,"");
    strcpy((char*)data->sDescription,"");
    data->bUsed = 0;
    data->dataIn = 0;
    data->dataInSize = 0;
    data->dataOut = 0;
    data->dataOutSize = 0;
    }
    return;
}

void FreeDataTransfer(S_DataTransfer * pData)
{
    if (pData)
    {}
    return;
}

void InitObjectInfo(S_ObjectInfo *info)
{
    if (info)
    {
    strcpy((char*)info->name,"");
    info->iID = -1;
    info->eDimensionsIn = eUndef;
    info->eDimensionsOut = eUndef;
    info->eDevice = eUndefined;
    info->data = 0;
    for (int i = 0; i<16; i++)
    {
        info->uuid[i] = 0;
    }
    }
    return;
}

void FreeObjectInfo(S_ObjectInfo * pInfo)
{
    if (pInfo)
    {}
    return;
}

void InitGuiControl(S_GuiControl * control)
{
    if (control)
    {
    control->getMin = 0;
    control->getMax = 0;
    control->getVal = 0;
    control->validate = 0;
    strcpy((char*)control->sName,"");
    strcpy((char*)control->sDescription,"");
    strcpy((char*)control->sUnit,"");
    control->bDisplayName = TRUE;
    control->bDisplayUnit = FALSE;
    }
    return;
}

void FreeGuiControl(S_GuiControl * pControl)
{
    if (pControl)
    {}
    return;
}

void InitGui(S_GuiInfo * info)
{
    if (info)
    {
    info->iCount = 0;
    info->pControls = 0;
    info->bUpdate = FALSE;
    }
    return;
}

void FreeGui(S_GuiInfo * pInfo)
{
    if (pInfo)
    {}
    return;
}
