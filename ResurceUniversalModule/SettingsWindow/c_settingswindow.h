#ifndef C_SETTINGSWINDOW_H
#define C_SETTINGSWINDOW_H

#include "./../widgets/childWidget/c_childwidget.h"
#include "ui_settings.h"
#include <QSharedPointer>
#include <./globals.h>
#include <QLineEdit>

class C_settingsWindow : public C_childWidget
{
    Q_OBJECT
public:
    C_settingsWindow(QWidget * parent = nullptr, QString sQML = QString(""), QSharedPointer<s_globals> globals = QSharedPointer<s_globals>());
    ~C_settingsWindow();

    virtual void saveThis();

    QJsonObject saveToJson();
    void        loadFromJson(QJsonObject variant);

signals:
    void pluginPathChanged();

public slots:
    virtual QString selectPluginDir(bool);

private:
    void addPluginPath(QString path, bool bAdd, QLineEdit *pEdit = nullptr);
    void addPluginPathGroup(QString sPath);


    Ui_Form * m_puiMain;
    QSharedPointer<s_globals> m_pGlobals;
};

#endif // C_LOGWINDOW_H
