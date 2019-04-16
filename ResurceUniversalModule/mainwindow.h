#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainWindow.h"
#include "MDIChild/mdichild.h"
#include "LogWindow/c_logwindow.h"
#include "SettingsWindow/c_settingswindow.h"
#include <globals.h>
#include <QSharedPointer>
#include "pluginManager/pluginManager.h"
#include "../common_includes/commonIncludes.h"
#include "DocumentView/DocumentView.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void LogThis(QString &sMessage);
    ~MainWindow();
    MDIChild * createMDIChild();
    C_DocumentView * CreateNewDocument(QString sName);
private:
    virtual void initGlobals();
    virtual void setupMainMenu();
    virtual void setupLogger();
    virtual void setupSettings();
    virtual void initPluginManager();
    virtual void saveToJson();
    virtual void loadFromJson();

    virtual QWidget * widgetFromQML(QString source, QWidget * parent = nullptr);

private slots:
    virtual void about();
    virtual void newDocument();
    virtual void displayChild(C_childWidget * widget, QAction * pAction);
    virtual void displayWindow(C_childWidget * pWidget);
    virtual void displayLogger();
    virtual void displaySettings();

    virtual void ChildMoved(QPoint pos);
    virtual void ChildHidding();

protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow * m_puiMain;
    C_logWindow* m_pLogWindow;
    C_settingsWindow *m_pSettingsWindow;
    QSharedPointer<s_globals> m_pGlobals;
    C_pluginManager * m_pPluginManager = nullptr;
};

#endif // MAINWINDOW_H
