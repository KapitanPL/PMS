#include "gnr_dummycammera.h"
#include "string.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <string>
#include <QDebug>

extern "C" PLUGIN_API t_int32 ExitFunc()
{
  return 0;
}

static S_ObjectInfo sDummyCameraInfo;

void createInfo()
{
    InitObjectInfo(&sDummyCameraInfo);
    strcpy(static_cast<char*>(sDummyCameraInfo.name), "dummy Camera");
    sDummyCameraInfo.eDevice = eCamera;
    sDummyCameraInfo.eDimensionsIn = eUndef;
    sDummyCameraInfo.eDimensionsOut = e2D;
    sDummyCameraInfo.iID = 0;

    /*sDummyCameraInfo.uuid[0] = 0x45;
    sDummyCameraInfo.uuid[1] = 0x1C;
    sDummyCameraInfo.uuid[2] = 0x1B;
    sDummyCameraInfo.uuid[3] = 0x5A;
    sDummyCameraInfo.uuid[4] = 0x42;
    sDummyCameraInfo.uuid[5] = 0xCC;
    sDummyCameraInfo.uuid[6] = 0x82;
    sDummyCameraInfo.uuid[7] = 0x11;
    sDummyCameraInfo.uuid[8] = 0xE8;
    sDummyCameraInfo.uuid[9] = 0x9A;
    sDummyCameraInfo.uuid[10] = 0x7D;
    sDummyCameraInfo.uuid[11] = 0x8C;
    sDummyCameraInfo.uuid[12] = 0xC2;
    sDummyCameraInfo.uuid[13] = 0x6B;
    sDummyCameraInfo.uuid[14] = 0xE5;
    sDummyCameraInfo.uuid[15] = 0xB9;*/

     //"70 6c bc c9 91 2d 4f 94 bc 1b 0c a9 f3 c9 d3 71"
    std::string sUUID = "451C1B5A42CC8211E89A7D8CC26BE5B9";
    if (sUUID.size()>32)
        sUUID.resize(32);
    for (unsigned int i = 0; i < 16; i++)
    {
        std::string byteString  = sUUID.substr(2*i, 2);
        sDummyCameraInfo.uuid[i] = strtol(byteString.c_str(), nullptr, 16);
    }
}

extern "C" PLUGIN_API PF_ExitFunc PF_initPlugin(const PF_PlatformServices * params)
{
  PF_RegisterParams rp;
  rp.version.major = 1;
  rp.version.minor = 0;
  rp.programmingLanguage = PF_ProgrammingLanguage_CPP;

  platform::setServices(params);
  platform::callLog("LogEntry from module");

  createInfo();

  platform &platform = platform::getPlatform();
  rp.createFunc = gnr_dummyCammera::create;
  rp.callFunc =  gnr_dummyCammera::call;
  rp.destroyFunc = gnr_dummyCammera::destroy;
  platform.services.registerObject(ps("DUMMY_CAMERA"), &rp);

  return ExitFunc;
}

platform::platform()
{
}

platform::~platform()
{

}

platform &platform::getPlatform()
{
    static platform plat;
    return plat;
}

void platform::setServices(const PF_PlatformServices *pServices)
{
    platform &platform = getPlatform();
    platform.services = *pServices;
}

t_int32 platform::invokeService(const t_byte *sCommand, void *data)
{
    platform &platform = getPlatform();
    if (platform.services.invokeService)
    {
        return platform.services.invokeService(sCommand, data);
    }
    else
        return 0;
}

t_int32 platform::callLog(const char sMessage[256])
{
    platform &platform = getPlatform();
    if (platform.services.invokeService)
        return platform.services.invokeService(ps("LOG"), (void*)(&sMessage[0]));
    return -1;

}

void *gnr_dummyCammera::create(PF_ObjectParams */*pParams*/)
{
    static gnr_dummyCammera camera;
    return &camera;
}

t_int32 gnr_dummyCammera::destroy(void *camera)
{
    gnr_dummyCammera * pCamera = static_cast<gnr_dummyCammera * >(camera);
    if (pCamera)
    {
        delete pCamera;
        camera = nullptr;
        return RES_OK;
    }
    return RES_ERR_FAIL;
}

t_int32 gnr_dummyCammera::call(void *camera, const t_byte *command, t_int32 */*piParam*/, double */*pdParam*/, void *pParam)
{
    t_int32 iRes = RES_OK;
    gnr_dummyCammera * pCamera = static_cast<gnr_dummyCammera * >(camera);
    const char * sCommand = static_cast<const char *>(command);
    /*if (pCamera && command)
    {
        iRes = RES_ERR_NOTIMPL;
    }*/
    if (strcmp(sCommand, "DEVICE_DESCRIPTION") == 0 && pParam)
    {
        S_ObjectInfo * pInfo = static_cast<S_ObjectInfo*>(pParam);
        *pInfo = sDummyCameraInfo;
    }
    if (pCamera && strcmp(sCommand, "GET_GUI_INFO") == 0 && pParam)
    {
        S_GuiInfo *pInfo = static_cast<S_GuiInfo*>(pParam);
        *pInfo = pCamera->sGui;
    }
    return iRes;
}

gnr_dummyCammera::gnr_dummyCammera()
{    
    vControls.resize(vControls.size()+1);
    roiLeft.Init(vControls.back(),this, "Left", "Left border", "px", true, true, eEdit);
    vControls.resize(vControls.size()+1);
    roiTop.Init(vControls.back(),this, "Top", "Top border", "px", true, true, eEdit);
    vControls.resize(vControls.size()+1);
    roiRight.Init(vControls.back(),this, "Right", "Right border", "px", true, true, eEdit);
    vControls.resize(vControls.size()+1);
    roiBottom.Init(vControls.back(),this, "Bottom", "Bottom border", "px", true, true, eEdit);

    vControls.resize(vControls.size()+1);
    vControls.back().eType = eSplitter;

    vControls.resize(vControls.size()+1);
    SourceFileEdit.Init(vControls.back(),this, "Source File", "Source file path", "", true, false, eEdit);
    vControls.resize(vControls.size()+1);
    SourceFileButton.Init(vControls.back(),this, "Choose file", "Choose file", "", true, false, ePushButton);
    vControls.resize(vControls.size()+1);
    SourceFileResetPos.Init(vControls.back(),this, "Reset file position", "Choose file", "", true, false, ePushButton);

    sGui.iCount = static_cast<t_int32>(vControls.size());
    sGui.pControls = vControls.data();
}



void gnr_dummyCammera::getMin(S_GuiControl *pControl, void *min)
{
    if (pControl && min)
    {
        if (pControl == roiLeft.m_pControl)
        {

        }
        else if (pControl == roiRight.m_pControl)
        {

        }
        else if (pControl == roiTop.m_pControl)
        {

        }
        else if (pControl == roiBottom.m_pControl)
        {

        }
    }
}

void gnr_dummyCammera::getMax(S_GuiControl *pControl, void *max)
{
    if (pControl && max)
    {
        if (pControl == roiLeft.m_pControl)
        {

        }
        else if (pControl == roiRight.m_pControl)
        {

        }
        else if (pControl == roiTop.m_pControl)
        {

        }
        else if (pControl == roiBottom.m_pControl)
        {

        }
    }
}

void gnr_dummyCammera::getVal(S_GuiControl *pControl, void *val)
{
    if (pControl && val)
    {
        if (pControl == roiLeft.m_pControl)
        {

        }
        else if (pControl == roiRight.m_pControl)
        {

        }
        else if (pControl == roiTop.m_pControl)
        {

        }
        else if (pControl == roiBottom.m_pControl)
        {

        }
        else if (pControl == SourceFileEdit.m_pControl)
        {

        }
        else if (pControl == SourceFileEdit.m_pControl)
        {

        }
        else if (pControl == SourceFileResetPos.m_pControl)
        {

        }
    }
}

bool gnr_dummyCammera::validate(S_GuiControl *pControl, void *val)
{
    bool bRet = true;
    if (pControl && val)
    {
        if (pControl == roiLeft.m_pControl)
        {

        }
        else if (pControl == roiRight.m_pControl)
        {

        }
        else if (pControl == roiTop.m_pControl)
        {

        }
        else if (pControl == roiBottom.m_pControl)
        {

        }
        else if (pControl == SourceFileEdit.m_pControl)
        {

        }
        else if (pControl == SourceFileEdit.m_pControl)
        {

        }
        else if (pControl == SourceFileResetPos.m_pControl)
        {

        }
    }
    return bRet;
}

void gnr_dummyCammera::linkControl(S_GuiControl *pControl)
{
    if (pControl)
    {
        /*strcpy(pControl->sName,sName);
        strcpy(pControl->sDescription,sDescription);
        strcpy(pControl->sUnit, sUnit);
        pControl->bDisplayName = bDisplayName;
        pControl->bDisplayUnit = bDisplayUnit;
        pControl->eType = eType;
        pControl->getMax = [pControl, this](void * max){getMax(pControl,max);};
        pControl->getMin = [pControl, this](void * min){getMin(pControl,min);};
        pControl->getVal = [pControl, this](void * val){getMax(pControl,val);};
        pControl->validate = [pControl, this](void * val)->bool{validate(pControl,val);};*/
    }
}

C_GuiControl::C_GuiControl()
{
}

void C_GuiControl::Init(S_GuiControl &aControl, gnr_dummyCammera *pCamera, const t_byte *sName, const t_byte *sDescription, const t_byte *sUnit, bool bDisplayName, bool bDisplayUnit, eGuiControl eType)
{
    m_pControl = &aControl;
    m_pControl->eType = eType;
    strcpy(m_pControl->sName, sName);
    strcpy(m_pControl->sDescription, sDescription);
    strcpy(m_pControl->sUnit, sUnit);
    m_pControl->bDisplayName = bDisplayName;
    m_pControl->bDisplayUnit = bDisplayUnit;
    m_pCamera = pCamera;
}

void C_GuiControl::getMin(void *min)
{
    if (m_pCamera)
        m_pCamera->getMin(m_pControl, min);
}

void C_GuiControl::getMax(void *max)
{
    if (m_pCamera)
        m_pCamera->getMax(m_pControl, max);
}

void C_GuiControl::getVal(void *val)
{
    if (m_pCamera)
        m_pCamera->getVal(m_pControl, val);
}

bool C_GuiControl::validate(void *val)
{
    if (m_pCamera)
        return m_pCamera->validate(m_pControl, val);
    else
        return true;
}
