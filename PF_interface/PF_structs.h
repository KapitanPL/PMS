#ifndef PF_STRUCT_H
#define PF_STRUCT_H

#include "PF_defines.h"
#define MAX_STR 255 /* max string length of string fields */

#define TRUE 1
#define FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

int callTest(int a, int b);

typedef enum 
{
    eUndef      = 0x00,
    e0D         = 0x01,
    e1D         = 0x02,
    e2D         = 0x04
} eDimension;

typedef enum
{
    eUndefined  = 0x000000,
    eSensor     = 0x000001, //provider of 0D data
    eCamera     = 0X000002, //provider of 2D data
    eSpectro    = 0x000004, //provider of 1D dta
    eProcessor  = 0x000008, //takes input data, process and outputs
    eConsumer   = 0x000010, //takes data on input, does not output, reacts
    eDeviceMax
} eDeviceType;

typedef struct S_ObjectInfo
{
    t_byte      name[MAX_STR]; // null terminated identificator
    t_int32     iID;
    t_byte      uuid[16];
    eDimension  eDimensionsIn;
    eDimension  eDimensionsOut;
    eDeviceType eDevice;
    
    void *      data;
    
} S_ObjectInfo;

void InitObjectInfo(S_ObjectInfo * pInfo);
void FreeObjectInfo(S_ObjectInfo * pInfo);

typedef struct S_DataTransfer
{
    t_byte      sName[MAX_STR]; // null terminated identificator
    t_byte      sDescription[MAX_STR]; // null terminated identificator

    t_int32        bUsed;
    
    t_byte *    dataIn;
    t_uint64    dataInSize;
    t_byte *    dataOut;
    t_uint64    dataOutSize;
} S_DataTransfer;
void InitDataTransfer(S_DataTransfer * pData);
void FreeDataTransfer(S_DataTransfer * pData);

typedef enum _eGuiControl
{
    ePushButton     = 0,
    eSwitchButton   = 1,
    eEdit           = 2,
    eSlider         = 3,
    eCombo          = 4,
    eSpinBox        = 5
} eGuiControl;

typedef struct _S_GuiControl
{
    void (*getMin)(void * min); //gets the minimum value
    void (*getMax)(void * max); //gets the maximum value
    void (*getVal)(void * val); //gets the current value
    int (*validate)(void * val); //used to set the value if in ranges. Otherwise the GUI provider should undo

    t_byte      sName[MAX_STR]; // null terminated identificator
    t_byte      sDescription[MAX_STR]; // null terminated identificator
    t_byte      sUnit[MAX_STR]; // null terminated identificator
    int        bDisplayName;
    int        bDisplayUnit;
    eGuiControl eType;
} S_GuiControl;

void InitGuiControl(S_GuiControl * pControl);
void FreeGuiControl(S_GuiControl * pControl);

typedef struct S_GuiInfo
{
    t_int32 iCount;
    S_GuiControl *pControls;
    int    bUpdate;
} S_GuiInfo;

void InitGui(S_GuiInfo *pInfo);
void FreeGui(S_GuiInfo *pInfo);

#ifdef  __cplusplus //extern C
}
#endif

#endif //PF_STRUCT_H
