#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "./globals.h"
#include "../../PF_interface/PF_structs.h"
#include "../../PF_interface/PF_pluginInterfaceV01.h"
#include <QObject>
#include <QSharedPointer>
#include <memory>

inline const t_byte * ps(const char a[])
{
    return reinterpret_cast<const t_byte *>(&a[0]);
}

class DynamicLibrary;

struct PluginInfo
{
    QString m_sDllPath;
    PF_ExitFunc pEXitFunc;
};

class C_pluginManager : public QObject
{
    Q_OBJECT

    struct S_pluginDevice
    {
        QString             sName;
        S_ObjectInfo        aInfo;
        PF_RegisterParams   aParams;
        void                *pDevice = nullptr;
    };

    typedef std::shared_ptr<DynamicLibrary> T_sharedLibraryPointer;
    typedef std::map<QString,  T_sharedLibraryPointer> T_mapDynamicLibrary;
public:
    typedef std::map<QString, PF_RegisterParams> T_registrationMap;

    void Init();
    void setParent(QObject * parent);
    static t_int32 registerObject(const t_byte * nodeType,
                                        const PF_RegisterParams * params);
    static C_pluginManager & getInstance();
    static t_int32 callService(const t_byte * command, void * dataParam);
    ~C_pluginManager();

    std::vector<S_pluginDevice> m_vCameras;
    std::vector<S_pluginDevice> m_vSensors;
    std::vector<S_pluginDevice> m_vSpectros;
    std::vector<S_pluginDevice> m_vOthers;
    PF_PlatformServices m_platformService;

private:
    C_pluginManager();
    void loadLibrary(QString sPath);

signals:
    void isAboutToReset();
    void Reseted();

private:
    QObject * m_parent;
    T_mapDynamicLibrary m_pluginsMap;
    T_registrationMap   m_regMap;
    std::vector<PF_RegisterParams> m_vRegs;
    std::vector<PF_ExitFunc> m_vExitFuncs;

    bool                m_bInInitializePlugin = false;

};


#endif //PLUGINMANAGER_H
