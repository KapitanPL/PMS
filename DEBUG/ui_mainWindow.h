/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QMdiArea *mdiArea;
    QMenuBar *mainMenu;
    QStatusBar *statusbar;
    QToolBar *mainToolBar;
    QToolBar *secondaryToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(814, 470);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setLineWidth(3);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(3);
        splitter->setChildrenCollapsible(false);
        mdiArea = new QMdiArea(splitter);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));
        splitter->addWidget(mdiArea);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        mainMenu = new QMenuBar(MainWindow);
        mainMenu->setObjectName(QStringLiteral("mainMenu"));
        mainMenu->setGeometry(QRect(0, 0, 814, 30));
        MainWindow->setMenuBar(mainMenu);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        secondaryToolBar = new QToolBar(MainWindow);
        secondaryToolBar->setObjectName(QStringLiteral("secondaryToolBar"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, secondaryToolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        mainToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
        secondaryToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
