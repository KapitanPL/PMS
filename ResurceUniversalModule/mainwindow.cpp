#include "mainwindow.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <Qt>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QQuickView>
#include "./widgets/toolBoxWidget/toolBoxWidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_puiMain(nullptr)
    , m_pLogWindow(nullptr)
    , m_pSettingsWindow(nullptr)
    , m_pGlobals(nullptr)
    , m_pPluginManager(nullptr)
{
    initGlobals();
    m_puiMain = new Ui::MainWindow;
    m_puiMain->setupUi(this);
    setupMainMenu();
    setupLogger(); //starts logging
    setupSettings();
    initPluginManager();

    loadFromJson();

    QMenu * pWindowMenu = findChild<QMenu*>("menuWidnows");
    if (pWindowMenu)
    {
        pWindowMenu->addSeparator();
    }
}

void MainWindow::LogThis(QString &sMessage)
{
    if (m_pLogWindow)
        m_pLogWindow->logThis(sMessage);
}

MainWindow::~MainWindow()
{
    save_delete(m_puiMain);
    save_delete(m_pLogWindow);
    save_delete(m_pSettingsWindow);
    m_pPluginManager->setParent(nullptr);
    m_pPluginManager = nullptr;
    //save_delete(m_pPluginManager); plugin manager is a static! The pointer is only a reference. DO not delete it!!!
}

MDIChild * MainWindow::createMDIChild()
{
    MDIChild *child = new MDIChild(this);
    if (child)
    {
        m_puiMain->mdiArea->addSubWindow(child);
        return child;
    }
    else
        return Q_NULLPTR;
}

C_DocumentView *MainWindow::CreateNewDocument(QString sName)
{
    C_DocumentView * pNew = new C_DocumentView(this);
    pNew->setMDIMode();
    pNew->setVisible(true);
    pNew->setName(sName);
    connect(pNew, &C_childWidget::moved, this, &MainWindow::ChildMoved);
    connect(pNew, &C_childWidget::hidding, this, &MainWindow::ChildHidding);

    QMenu * pWindowMenu = findChild<QMenu*>("menuWidnows");
    if (pWindowMenu)
    {
        QAction *actionLogger = pWindowMenu->addAction(sName, this, [this, pNew](){this->displayWindow(pNew);});
        actionLogger->setCheckable(true);
        actionLogger->setChecked(true);
        actionLogger->setObjectName("action"+sName);
        MDIChild * pChild = createMDIChild();
        if(pChild)
        {
            pNew->setVisible(true);
            pChild->setWidget(pNew);
            pChild->show();
            pNew->move(pNew->pos());
            pChild->resize(pNew->sizeHint());
        }
    }
    return pNew;
}

void MainWindow::initGlobals()
{
    if (!m_pGlobals)
        m_pGlobals = QSharedPointer<s_globals>(new s_globals);
}

void MainWindow::setupMainMenu()
{
    if (m_puiMain && m_puiMain->mainMenu)
    {
        QMenu * pFileMenu = new QMenu(tr("File"), this);
        pFileMenu->setObjectName("menuFile");
        m_puiMain->mainMenu->addMenu(pFileMenu);
        pFileMenu->addAction(tr("New"), this, &MainWindow::newDocument);

        QMenu * pWindowMenu = new QMenu(tr("Windows"), this);
        pWindowMenu->setObjectName("menuWidnows");
        m_puiMain->mainMenu->addMenu(pWindowMenu);

        QMenu * pHelpMenu = new QMenu(tr("Help"));
        m_puiMain->mainMenu->addMenu(pHelpMenu);
        pHelpMenu->addAction(tr("About"),this, &MainWindow::about);
    }
}

void MainWindow::setupLogger()
{
    m_pLogWindow = new C_logWindow(this); //logger started here
    m_pLogWindow->setMDIMode();
    m_pLogWindow->setVisible(false);
    m_pLogWindow->setName(QString("widgetLogger"));
    connect(m_pLogWindow, &C_childWidget::moved, this, &MainWindow::ChildMoved);
    connect(m_pLogWindow, &C_childWidget::hidding, this, &MainWindow::ChildHidding);

    QMenu * pWindowMenu = findChild<QMenu*>("menuWidnows");
    if (pWindowMenu)
    {
        QAction *actionLogger = pWindowMenu->addAction("Logger", this, &MainWindow::displayLogger);
        actionLogger->setCheckable(true);
        actionLogger->setChecked(false);
        actionLogger->setObjectName("actionLogger");
    }
}

void MainWindow::setupSettings()
{
    m_pSettingsWindow = new C_settingsWindow(this, ":/SETTINGS_WINDOW", m_pGlobals);
    m_pSettingsWindow->setMDIMode();
    m_pSettingsWindow->setVisible(false);
    m_pSettingsWindow->setName(QString("widgetSettings"));
    connect(m_pSettingsWindow, &C_childWidget::moved, this, &MainWindow::ChildMoved);
    connect(m_pSettingsWindow, &C_childWidget::hidding, this, &MainWindow::ChildHidding);

    QMenu * pWindowMenu = findChild<QMenu*>("menuWidnows");
    if (pWindowMenu)
    {
        QAction *actionLogger = pWindowMenu->addAction("Settings", this, &MainWindow::displaySettings);
        actionLogger->setCheckable(true);
        actionLogger->setChecked(false);
        actionLogger->setObjectName("actionSettings");
    }
}

void MainWindow::initPluginManager()
{
    //m_pPluginManager = new C_pluginManager(this, m_pGlobals);
    m_pPluginManager = &C_pluginManager::getInstance();
    if (m_puiMain && m_puiMain->splitter)
    {
        QToolBoxWidget * pToolBox = new QToolBoxWidget();
        pToolBox->setObjectName("devices");
        m_puiMain->splitter->addWidget(pToolBox);
    }
    if (m_pPluginManager)
    {
        m_pPluginManager->setParent(this);
        m_pPluginManager->Init();
        QToolBoxWidget * pToolBox = findChild<QToolBoxWidget*>("devices");
        if (m_pPluginManager->m_vCameras.size())
        {
            QIcon iconCameras(":/cameras.svg");
            QToolBoxItemWidget * pCameras = pToolBox->addItem(tr("Cameras"), iconCameras, nullptr);
            if (pCameras)
            {
                QIcon iconCamera(":/camera.svg");
                for (auto camera : m_pPluginManager->m_vCameras)
                {
                    PF_ObjectParams params;
                    params.platformServices = &m_pPluginManager->m_platformService;
                    strcpy(static_cast<char*>(params.objectType), "CAMERA");
                    camera.pDevice = camera.aParams.createFunc(&params);
                    QWidget *pCameraWidget = nullptr;

                    if (camera.pDevice)
                    {
                        S_GuiInfo aGuiInfo;
                        InitGui(&aGuiInfo);
                        camera.aParams.callFunc(camera.pDevice, ps("GET_GUI_INFO"), nullptr, nullptr ,static_cast<void *>(&aGuiInfo));

                        CreateCameraWidget(&pCameraWidget, aGuiInfo);

                        FreeGui(&aGuiInfo);
                    }
                    pCameras->addItem(camera.sName, iconCamera, pCameraWidget);
                }
            }
        }
    }
}

void MainWindow::saveToJson()
{
    if (!QDir().exists(g_globals.m_sConfigPath))
    {
        QDir().mkdir(g_globals.m_sConfigPath);
    }
    QString name = QString("guiData");
    QFile fGuiData(QString("%1/%2.json").arg(g_globals.m_sConfigPath).arg(name));
    if (fGuiData.open(QIODevice::WriteOnly))
    {
        QJsonObject saveObject;
        //save here
        QJsonArray listPluginPath;
        for (auto sPluginPath : g_globals.m_listPluginPath)
        {
            QJsonValue pluginPath(sPluginPath);
            //pluginPath["dirPath"] = sPluginPath;
            listPluginPath.append(pluginPath);
        }
        saveObject["pluginPath"] = listPluginPath;
        if (m_pLogWindow)
        {
            saveObject["logWindow"] = m_pLogWindow->saveToJson();
        }

        QJsonDocument saveDoc(saveObject);
        fGuiData.write(saveDoc.toJson());

        fGuiData.close();
    }
}

void MainWindow::loadFromJson()
{
    QString name = QString("guiData");
    if (QFile::exists(QString("%1/%2.json").arg(g_globals.m_sConfigPath).arg(name)))
    {
        QFile fGuiData(QString("%1/%2.json").arg(g_globals.m_sConfigPath).arg(name));
        if (fGuiData.open(QIODevice::ReadOnly))
        {
             QByteArray saveData = fGuiData.readAll();

             QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

             QJsonObject loadVariant = loadDoc.object();
             if (loadVariant.contains("pluginPath") && loadVariant["pluginPath"].isArray())
             {//m_pGlobals->m_listPluginPath = loadVariant["pluginPath"]
                 QJsonArray listPluginPath = loadVariant["plutginPath"].toArray();
                 for (auto pluginPath : listPluginPath)
                 {
                     if (pluginPath.isString())
                         g_globals.m_listPluginPath.append(pluginPath.toString());
                 }
             }
             if (loadVariant.contains("logWindow") && loadVariant["logWindow"].isObject())
             {
                 m_pLogWindow->loadFromJson(loadVariant["logWindow"].toObject());
             }
        }
    }
}

void MainWindow::CreateCameraWidget(QWidget ** ppWidget, S_GuiInfo &aInfo)
{
    if (aInfo.iCount > 0 && ppWidget)
    {
        *ppWidget = new QWidget();
        QWidget *pWidget = *ppWidget;
        QGridLayout * pMainL = new QGridLayout();
        pMainL->setVerticalSpacing(1);
        pWidget->setLayout(pMainL);
        pMainL->setContentsMargins(0,0,0,0);
        int iNextRow = 0;
        int iColumns = 1;
        for (int i=0; i < aInfo.iCount; i++)
        {
            S_GuiControl *pControl = &aInfo.pControls[i];
            int iCols = 1;
            if (pControl->eType != ePushButton || pControl->eType != eSwitchButton)
            {
                if (pControl->bDisplayName)
                    iCols++;
                if (pControl->bDisplayUnit)
                    iCols++;
            }
            iColumns = std::max(iColumns, iCols);
        }
        QHBoxLayout * pToolbar = nullptr;
        for (int i=0; i < aInfo.iCount; i++)
        {
            S_GuiControl *pControl = &aInfo.pControls[i];
            switch (pControl->eType)
            {
            case eEdit:
            {
                pToolbar = nullptr;
                int iCurCol = 0;
                if (pControl->bDisplayName)
                {
                    pMainL->addWidget(new QLabel(pControl->sName), iNextRow, iCurCol);
                    iCurCol++;
                }
                QLineEdit * pEdit = new QLineEdit();
                pEdit->setFixedHeight(15);
                pMainL->addWidget(pEdit, iNextRow, iCurCol);
                iCurCol++;
                if (pControl->bDisplayUnit)
                {
                    pMainL->addWidget(new QLabel(pControl->sUnit), iNextRow, iCurCol);
                    iCurCol++;
                }
                break;

            }
            case eSplitter:
            {
                pToolbar = nullptr;
                QFrame * line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                pMainL->addWidget(line, iNextRow, 0, 1, iColumns );
                break;
            }
            case ePushButton:
            {
                if (i < aInfo.iCount-1 && aInfo.pControls[i+1].eType == ePushButton) //first of several buttons, put them in a toolbar
                {
                    if (!pToolbar)
                    {
                        pToolbar = new QHBoxLayout();
                        pMainL->addLayout(pToolbar, iNextRow, 0, 1, iColumns);
                    }
                    QPushButton * pPush = new QPushButton(pControl->bDisplayName ? pControl->sName : "");
                    if (pToolbar)
                    {
                        pToolbar->addWidget(pPush);
                    }
                    else
                        pMainL->addWidget(pPush, iNextRow, 0, 1, 1);

                }
                else
                {
                    QPushButton * pPush = new QPushButton(pControl->bDisplayName ? pControl->sName : "");
                    if (pToolbar)
                    {
                        pToolbar->addWidget(pPush);
                        pToolbar->addStretch(1);
                    }
                    else
                        pMainL->addWidget(pPush, iNextRow, 0, 1, 1);
                    pToolbar = nullptr;
                }
                break;
            }
            default:
            {
                pToolbar = nullptr;
                QString sLabel = tr("Here is supposed to be: ");
                sLabel.append(pControl->sName);
                pMainL->addWidget(new QLabel(sLabel), iNextRow, 0, iColumns, 1);

                break;
            }
            }
            if (!pToolbar)
                iNextRow++;
        }

    }
}

QWidget *MainWindow::widgetFromQML(QString source, QWidget *parent)
{
    QQuickView *view = new QQuickView();
    QWidget *container = QWidget::createWindowContainer(view, parent ? parent : this);
    view->setSource(QUrl::fromLocalFile(source));
    return container;
}

void MainWindow::about()
{
    QMessageBox::information(this, tr("About"), tr("Poor Man's software proudly presents."));
}

void MainWindow::newDocument()
{
    CreateNewDocument("New");
}

void MainWindow::displayChild(C_childWidget * widget, QAction * pAction)
{
    if (pAction && pAction->isChecked() && widget)
    {
        switch(widget->getMode())
        {
        case C_childWidget::eMDIChild:
        {
            MDIChild * pLogChild = createMDIChild();
            if(pLogChild)
            {
                widget->setVisible(true);
                pLogChild->setWidget(widget);
                pLogChild->show();
                widget->move(widget->pos());
                //qDebug() << widget->size();
            }
            break;
        }
        case C_childWidget::eSeparate:
        {
            widget->setParent(this);
            Qt::WindowFlags flags = widget->windowFlags();
            flags |= Qt::Window;
            widget->setWindowFlags(flags);
            widget->setSeparateMode();
            widget->setVisible(true);
            widget->show();
            break;
        }
        default:
        {
            qDebug() << "not yet";
            break;
        }
        }
    }
    if (pAction && !pAction->isChecked() && widget)
    {
        switch(widget->getMode())
        {
        case C_childWidget::eMDIChild:
        {
            QList<QMdiSubWindow*> SubWindowList = m_puiMain->mdiArea->subWindowList();
            for (auto &itMDI : SubWindowList)
            {
                MDIChild * pMDI = dynamic_cast<MDIChild*>(itMDI);
                if (pMDI)
                {
                    if (pMDI->widget() == widget)
                    {
                        pMDI->setWidget(nullptr);
                        pMDI->close();
                    }
                }
            }
            break;
        }
        case C_childWidget::eSeparate:
        {
            C_childWidget * pWidget = dynamic_cast<C_childWidget*>(widget);
            if (pWidget)
            {
                pWidget->setParent(Q_NULLPTR);
                Qt::WindowFlags flags = pWidget->windowFlags();
                flags &= ~Qt::Window;
                pWidget->setWindowFlags(flags);
            }
            break;
        }
        default:
        {
            break;
        }
        }

    }
}

void MainWindow::displayWindow(C_childWidget * pWidget)
{
    QAction * pAction = dynamic_cast<QAction*>(sender());
    displayChild(pWidget, pAction);
}

void MainWindow::displayLogger()
{
    //C_logWindow * pLogWindow = new C_logWindow(this);
    //pLogWindow->setMDIMode();
    QAction * pAction = dynamic_cast<QAction*>(sender());
    displayChild(m_pLogWindow, pAction);
}

void MainWindow::displaySettings()
{
    QAction * pAction = dynamic_cast<QAction*>(sender());
    displayChild(m_pSettingsWindow, pAction);
}

void MainWindow::ChildMoved(QPoint /*pos*/)
{
    C_childWidget * pWidget = dynamic_cast<C_childWidget*>(sender());
    if (pWidget && m_puiMain && m_puiMain->mdiArea)
    {
        //qDebug() << "Position: " << pos << ", cursor: " << QCursor::pos();
        QPoint pos = m_puiMain->mdiArea->mapFromGlobal(QCursor::pos());
        QRect rectMDI = m_puiMain->mdiArea->rect();
        QPoint rectMainTopLeft = m_puiMain->mdiArea->mapFromGlobal(this->rect().topLeft());
        QPoint rectMainBottomRight = m_puiMain->mdiArea->mapFromGlobal(this->rect().bottomRight());
        QPoint ptMDI = mapToGlobal(m_puiMain->mdiArea->pos());
        if (pos.x() <= rectMDI.width()
         && pos.y() <= rectMDI.height()
         && pos.x() >= 0
         && pos.y() >= 0)
        {
            //qDebug() << "inside";
            if (pWidget->getMode() != C_childWidget::eMDIChild)
            {
                pWidget->setParent(Q_NULLPTR);
                MDIChild * pChild = this->createMDIChild();
                pChild->setWidget(pWidget); //this should parent the pWidget
                Qt::WindowFlags flags = pWidget->windowFlags();
                flags &= ~Qt::Window;
                pWidget->setWindowFlags(flags);
                pWidget->setMDIMode();
                pChild->move(pWidget->pos()-ptMDI);
                pChild->show();
            }
        }
        else if (pos.x() < rectMainTopLeft.x()
                 || pos.x() > rectMainBottomRight.x()
                 || pos.y() < rectMainTopLeft.y()
                 || pos.y() > rectMainBottomRight.y()
                 )
        {
            //qDebug() << "outside";
            if (pWidget->getMode() != C_childWidget::eSeparate)
            {
                QMdiSubWindow * pSubWind = m_puiMain->mdiArea->activeSubWindow();
                MDIChild * pMDI = dynamic_cast<MDIChild*>(pSubWind);
                if (pMDI)
                {
                    pWidget->setParent(this);
                    Qt::WindowFlags flags = pWidget->windowFlags();
                    flags |= Qt::Window;
                    pWidget->setWindowFlags(flags);
                    pWidget->setSeparateMode();
                    pWidget->move(pMDI->pos()+ptMDI);
                    pWidget->setWindowTitle(pWidget->m_sHumanReadableName);
                    pWidget->show();
                    pMDI->setWidget(Q_NULLPTR);
                    pMDI->close();
                }
            }
        }
    }
}

void MainWindow::ChildHidding()
{
    C_childWidget * pWidget = dynamic_cast<C_childWidget*>(sender());
    QMenu * pWindowMenu = findChild<QMenu*>("menuWidnows");
    if (pWidget && pWindowMenu)
    {
        QString sName = pWidget->getName();
        sName = sName.right(sName.length() - 6); //all widgets start with "widget"
        QAction *pAction = pWindowMenu->findChild<QAction*>("action"+sName);
        if (pAction)
            pAction->setChecked(false);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Close?", "Application will be terminated. Proceed?", QMessageBox::StandardButton::Yes|QMessageBox::StandardButton::No);
    if (reply == QMessageBox::StandardButton::Yes)
    {
        this->saveToJson();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
