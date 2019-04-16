#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QStringList>

template<typename T>
void save_delete(T *& t)
{
    if (t != nullptr)
    {
        delete t;
        t = nullptr;
    }
}

struct s_globals
{
    QString m_sConfigPath;
    QStringList m_listPluginPath;
    s_globals()
        : m_sConfigPath("./Configuration")
        , m_listPluginPath({"./Plugins"})
    {}
    virtual ~s_globals(){}
};

static s_globals g_globals;
#endif//GLOBALS_H
