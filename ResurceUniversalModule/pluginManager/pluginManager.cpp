#include "pluginManager.h"

#include "../mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include "DynamicLibrary.h"
#include <QDebug>

const t_byte * ps(const char a[])
{
    return reinterpret_cast<const t_byte *>(&a[0]);
}

//static C_pluginManager manager;

static bool isValid(const t_byte * objectType, const PF_RegisterParams * params)
{
  if (!objectType || !(*objectType))
     return false;
  if (!params ||!params->createFunc || !params->destroyFunc)
    return false;

  return true;
}

C_pluginManager::C_pluginManager()
    : QObject(nullptr)
    , m_parent(nullptr)
{
    m_platformService.version.major = 1;
    m_platformService.version.minor = 0;
    m_platformService.invokeService = callService; // can be populated later
    m_platformService.registerObject = registerObject;
}

void C_pluginManager::Init()
{
    //TODO fill m_platformService.invokeService
    QString sErrMsg = QString("");
    for (auto sPluginPath : g_globals.m_listPluginPath)
    {
        if (!QDir().exists(sPluginPath))
        {
            sErrMsg.append(QString("%1\n").arg(sPluginPath));
            continue;
        }
        //okay, path exist, check if we have something to load.
        QDir directory(sPluginPath);
#ifdef _WIN32
        QStringList listPlugins = directory.entryList(QStringList() << "*.dll" << "*.DLL", QDir::Files);
#else
        QStringList listPlugins = directory.entryList(QStringList() << "*.so", QDir::Files);
#endif
        foreach (QString sFileName, listPlugins)
        {
            loadLibrary(sPluginPath + QDir::separator() + sFileName);
        }
    }

    if (!sErrMsg.isEmpty())
    {
        QMessageBox::warning(nullptr
                             , tr("Plugin path: warning")
                             , tr("Bad plugin directory, path(s) does not exist: \n %1").arg(sErrMsg)
                             , QMessageBox::Ok);
    }

    //all plugins loaded

}

void C_pluginManager::setParent(QObject *parent)
{
    m_parent = parent;
    QObject::setParent(parent);
}

t_int32 C_pluginManager::registerObject(const t_byte *nodeType, const PF_RegisterParams *params)
{
    //this call is necessary because of need of initialization of static variables!
    C_pluginManager * pm = &C_pluginManager::getInstance();
    if (isValid(nodeType, params) && pm)
    {
        if (params->version.major == pm->m_platformService.version.major
                && params->version.minor <= pm->m_platformService.version.minor)
        {
            QString key((const char *)nodeType);
            if (key == QString("*"))
                pm->m_vRegs.push_back(*params);
            if (pm->m_regMap.find(key) == pm->m_regMap.end())
                pm->m_regMap[key] = *params;
            else
                return -1;
            S_ObjectInfo aInfo;
            qDebug() << callTest(5,8);
            //InitObjectInfo(&aInfo);
            params->callFunc(0, ps("DEVICE_DESCRIPTION"), 0, 0,static_cast<void *>(&aInfo));

            S_pluginDevice aDevice;
            aDevice.sName = QString((const char*)aInfo.name);
            aDevice.aInfo = aInfo;
            aDevice.aParams = * params;
            switch (aInfo.eDevice)
            {
            case eCamera:
            {
                pm->m_vCameras.push_back(aDevice);
                break;
            }
            case eSpectro:
            {
                pm->m_vSpectros.push_back(aDevice);
                break;
            }
            case eSensor:
            {
                pm->m_vSensors.push_back(aDevice);
                break;
            }
            default:
            {
                pm->m_vOthers.push_back(aDevice);
                break;
            }
            }
        }
    }
    return 0;
}

C_pluginManager & C_pluginManager::getInstance()
{
    static C_pluginManager S;
    return S;
}

t_int32 C_pluginManager::callService(const t_byte *command, void *dataParam)
{
    C_pluginManager * self = &getInstance();
    if (!self)
        return -1;
    QString sCommand = QString::fromLatin1((const char*)command);
    if (sCommand == "LOG")
    {
        MainWindow* parent = qobject_cast<MainWindow*>(self->m_parent);
        QString sMessage = QString::fromLatin1((char *)dataParam);
        if (parent)
            parent->LogThis(sMessage);
    }
    if (sCommand == "ERR")
    {
        MainWindow* parent = qobject_cast<MainWindow*>(self->m_parent);
        QString sError = QString(*(char *)dataParam);
        if (parent)
            parent->LogThis(QString("ERR ").append(sError));
    }
    if (sCommand == "PROGRES")
    {
        MainWindow* parent = qobject_cast<MainWindow*>(self->m_parent);
        double dProgress = *(double*)dataParam;
        QString sMessage = QString("Progress %1").arg(dProgress);
        if (parent)
            parent->LogThis(sMessage);
    }
    return 0;
}

C_pluginManager::~C_pluginManager()
{
    debfor (auto &itExit : m_vExitFuncs)
        itExit();
}

void C_pluginManager::loadLibrary(QString sPath)
{
    MainWindow* parent = qobject_cast<MainWindow*>(m_parent);
    if (parent)
        parent->LogThis(QString("Attempting to load library: ").append(sPath));
    std::string sErrString;
    bool bSuccess = false;
    T_sharedLibraryPointer pLib = T_sharedLibraryPointer(DynamicLibrary::load(sPath.toStdString(), sErrString));
    if (pLib)
    {
        PF_InitFunc fInitFunc = (PF_InitFunc)(pLib->getSymbol("PF_initPlugin"));
        if (fInitFunc)
        {
            PF_ExitFunc fExitFunc = fInitFunc(&m_platformService);
            if (fExitFunc)
            {
                m_vExitFuncs.push_back(fExitFunc);
                m_pluginsMap[sPath] = pLib;
                bSuccess = true;
            }
        }
    }
    if (bSuccess)
    {
        if (parent)
            parent->LogThis(QString("Library loaded: ").append(sPath));
    }
    else
    {
        if (parent)
        {
            QString sMessage = QString("FAIL! Library not loaded: %1 \n Reason: %2").arg(sPath).arg(sErrString.data());
            parent->LogThis(sMessage);
        }
    }
}
