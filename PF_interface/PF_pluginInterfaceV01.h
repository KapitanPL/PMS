#ifndef PF_PLUGIN_H
#define PF_PLUGIN_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "PF_defines.h"

typedef enum PF_ProgrammingLanguage
{
  PF_ProgrammingLanguage_C,
  PF_ProgrammingLanguage_CPP
} PF_ProgrammingLanguage;
  
struct PF_PlatformServices;

typedef struct PF_ObjectParams_
{
  t_byte * objectType;
  struct PF_PlatformServices * platformServices;
} PF_ObjectParams;
 
typedef struct PF_PluginAPI_Version_
{
  t_int32 major;
  t_int32 minor;
} PF_PluginAPI_Version;
 
typedef void * (*PF_CreateFunc)(PF_ObjectParams *);  
typedef t_int32 (*PF_DestroyFunc)(void *);
typedef t_int32 (*PF_Call)(void *, const t_byte *, t_int32 *, double *, void *);
/*
 PF_Call structure
 void * pointer to objectType
 const t_byte * null terminated char array "ObjectEvent"
 int general purpose simple int argument
 double general purpose simple double argument
 void * general purpose data structure
 */
typedef struct PF_RegisterParam_
{
  PF_PluginAPI_Version  version;
  PF_CreateFunc         createFunc;
  PF_Call               callFunc;
  PF_DestroyFunc        destroyFunc;
  PF_ProgrammingLanguage programmingLanguage;
} PF_RegisterParams;
 
typedef t_int32 (*PF_RegisterFunc)(const t_byte * nodeType, const PF_RegisterParams * params);
/*
 * invokeService now accepts parameters 
 * LOG -> pParams is supposed to be char * and as such converts the null terminated char to message to be logged
 * ERR -> pParams is supposed to be char * and as such converts the null terminated char to message to be logged
 * PROGRES -> pParams is supposed to be double *and as such is onverted and sent to progress bar
 */
typedef t_int32 (*PF_InvokeServiceFunc)(const t_byte * serviceName, void * pParams);
 
typedef struct PF_PlatformServices
{
  PF_PluginAPI_Version version;
  PF_RegisterFunc registerObject; 
  PF_InvokeServiceFunc invokeService; 
} PF_PlatformServices_;

typedef t_int32 (*PF_ExitFunc)();
 
typedef PF_ExitFunc (*PF_InitFunc)(const PF_PlatformServices *);
 
#ifndef PLUGIN_API
  #ifdef WIN32
    #ifdef LIBRARY_EXPORT
        #define PLUGIN_API __declspec(dllexport)
        #define PLUGIN_API_HIDE
    #else
        #define PLUGIN_API __declspec(dllimport)
        #define PLUGIN_API_HIDE
    #endif
  #else
    #ifdef LIBRARY_EXPORT
        #define PLUGIN_API __attribute__((visibility ("default")))
        #define PLUGIN_API_HIDE __attribute__((visibility ("hidden")))
    #else
        #define PLUGIN_API
        #define PLUGIN_API_HIDE
    #endif
  #endif
#endif
 
PLUGIN_API PF_ExitFunc PF_initPlugin(const PF_PlatformServices * params);
 
#ifdef  __cplusplus
}
#endif
#endif /* PF_PLUGIN_H */
