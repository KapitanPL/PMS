#ifndef GNR_DUMMYCAMMERA_H
#define GNR_DUMMYCAMMERA_H
#include "gnr_dummycammera_global.h"
#include <mutex>
#include "../common_includes/commonIncludes.h"
#include "../PF_interface/PF_structs.h"
#include "../PF_interface/PF_pluginInterfaceV01.h" //here the PLUGIN_API is declared

class gnr_dummyCammera;

extern "C" PLUGIN_API t_int32 ExitFunc();

class platform
{
public:
    static platform &getPlatform();
    static void setServices(const PF_PlatformServices * pServices);
    static t_int32 invokeService(const t_byte * sCommand, void * data);
    static t_int32 callLog(const char sMessage[256]);
    PF_PlatformServices services;
private:
    platform();
    ~platform();
};

/*class C_GuiControl
{
public:
    C_GuiControl();
    virtual ~C_GuiControl();
    virtual void Init(gnr_dummyCammera *pCamera, t_byte *sName, t_byte *sDescription, t_byte *sUnit, bool bDisplayName, bool bDisplayUnit, eGuiControl eType);
    S_GuiControl m_control;
};

class gnr_dummyCammera
{

public:
    static void * create(PF_ObjectParams * pParams);
    static t_int32 destroy(void * camera);
    static t_int32 call(void * camera, const t_byte * command, t_int32 * piParam, double * pdParam, void * pParam);
    //disable existance of copy
    gnr_dummyCammera(gnr_dummyCammera const&) = delete;
    void operator=(gnr_dummyCammera const&) = delete;
protected:
    void linkControl(S_GuiControl *pControl);
private:
    gnr_dummyCammera();

    void getMin(S_GuiControl * pControl, void *min);
    void getMax(S_GuiControl * pControl, void *max);
    void getVal(S_GuiControl * pControl, void *val);
    bool validate(S_GuiControl * pControl, void *val);


    S_GuiControl controls[7];
    S_GuiInfo sGui;

    std::mutex m;
    //gui controls statics


    C_GuiControl    roiLeft;
    C_GuiControl    roiRight;
    C_GuiControl    roiTop;
    C_GuiControl    roiBottom;
    C_GuiControl    SourceFileEdit;
    C_GuiControl    SourceFileButton;
    C_GuiControl    SourceFileResetPos;
};*/

#endif // GNR_DUMMYCAMMERA_H
