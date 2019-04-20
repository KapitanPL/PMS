#include "gnr_dummycammera.h"
#include "string.h"

extern "C" PLUGIN_API t_int32 ExitFunc()
{
  return 0;
}

static S_ObjectInfo sDummyCameraInfo;

void createInfo()
{
    InitObjectInfo(&sDummyCameraInfo);
    strcpy((char*)sDummyCameraInfo.name, "dummy Camera");
    sDummyCameraInfo.eDevice = eCamera;
    sDummyCameraInfo.eDimensionsIn = eUndef;
    sDummyCameraInfo.eDimensionsOut = e2D;
    sDummyCameraInfo.iID = 0;
    sDummyCameraInfo.uuid[0] = 0x45;
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
    sDummyCameraInfo.uuid[15] = 0xB9;
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
  platform.services.registerObject((const t_byte*)"DUMMY_CAMERA", &rp);

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
        return platform.services.invokeService((const t_byte*)&"LOG", (void*)sMessage);
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
    const char * sCommand = (const char *)command;
    if (pCamera && command)
    {
        iRes = RES_ERR_NOTIMPL;
    }
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
    sGui.iCount = sizeof(controls)/sizeof(S_GuiControl);
    sGui.pControls = &controls[0];
}



void gnr_dummyCammera::getMin(S_GuiControl *pControl, void *min)
{
    if (pControl && min)
    {
        if (pControl == &roiLeft.m_control)
        {

        }
        else if (pControl == &roiRight.m_control)
        {

        }
        else if (pControl == &roiTop.m_control)
        {

        }
        else if (pControl == &roiBottom.m_control)
        {

        }
    }
}

void gnr_dummyCammera::getMax(S_GuiControl *pControl, void *max)
{
    if (pControl && max)
    {
        if (pControl == &roiLeft.m_control)
        {

        }
        else if (pControl == &roiRight.m_control)
        {

        }
        else if (pControl == &roiTop.m_control)
        {

        }
        else if (pControl == &roiBottom.m_control)
        {

        }
    }
}

void gnr_dummyCammera::getVal(S_GuiControl *pControl, void *val)
{
    if (pControl && val)
    {
        if (pControl == &roiLeft.m_control)
        {

        }
        else if (pControl == &roiRight.m_control)
        {

        }
        else if (pControl == &roiTop.m_control)
        {

        }
        else if (pControl == &roiBottom.m_control)
        {

        }
        else if (pControl == &SourceFileEdit.m_control)
        {

        }
        else if (pControl == &SourceFileEdit.m_control)
        {

        }
        else if (pControl == &SourceFileResetPos.m_control)
        {

        }
    }
}

bool gnr_dummyCammera::validate(S_GuiControl *pControl, void *val)
{
    bool bRet = true;
    if (pControl && val)
    {
        if (pControl == &roiLeft.m_control)
        {

        }
        else if (pControl == &roiRight.m_control)
        {

        }
        else if (pControl == &roiTop.m_control)
        {

        }
        else if (pControl == &roiBottom.m_control)
        {

        }
        else if (pControl == &SourceFileEdit.m_control)
        {

        }
        else if (pControl == &SourceFileEdit.m_control)
        {

        }
        else if (pControl == &SourceFileResetPos.m_control)
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
   m_control.eType = eUnknown;
   m_control.sName[0] = 0;
   m_control.sUnit[0] = 0;
   m_control.sDescription[0] = 0;
   m_control.getMax = nullptr;
   m_control.getMin = nullptr;
   m_control.getVal = nullptr;
   m_control.validate = nullptr;
   m_control.bDisplayName = true;
   m_control.bDisplayUnit = false;
}

void C_GuiControl::Init(gnr_dummyCammera *pCamera, t_byte *sName, t_byte *sDescription, t_byte *sUnit, bool bDisplayName, bool bDisplayUnit, eGuiControl eType)
{
    m_control.eType = eType;
    strcpy(m_control.sName, sName);
    strcpy(m_control.sDescription, sDescription);
    strcpy(m_control.sUnit, sUnit);
    m_control.bDisplayName = bDisplayName;
    m_control.bDisplayUnit = bDisplayUnit;
    m_pCamera = pCamera;
}

void C_GuiControl::getMin(void *min)
{
    if (m_pCamera)
        m_pCamera->getMin(&m_control, min);
}

void C_GuiControl::getMax(void *max)
{
    if (m_pCamera)
        m_pCamera->getMax(&m_control, max);
}

void C_GuiControl::getVal(void *val)
{
    if (m_pCamera)
        m_pCamera->getVal(&m_control, val);
}

bool C_GuiControl::validate(void *val)
{
    if (m_pCamera)
        return m_pCamera->validate(&m_control, val);
    else
        return true;
}
