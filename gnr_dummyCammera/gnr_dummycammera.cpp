#include "gnr_dummycammera.h"
#include "string.h"

extern "C" PLUGIN_API t_int32 ExitFunc()
{
  return 0;
}

static S_ObjectInfo sDummyCameraInfo;

void createInfo()
{
    int b = callTest(5,6);
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

  platform &platform = platform::getPlatform();
  rp.createFunc = nullptr; // gnr_dummyCammera::create;
  rp.callFunc = nullptr; // gnr_dummyCammera::call;
  rp.destroyFunc = nullptr; //gnr_dummyCammera::destroy;
  platform.services.registerObject((const t_byte*)"DUMMY_CAMERA", &rp);

  createInfo();

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

