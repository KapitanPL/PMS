#include "c_childwidget.h"
#include <QDebug>
#include <QMoveEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QIcon>

C_childWidget::C_childWidget(QWidget *parent)
    : QWidget(parent)
    , m_sHumanReadableName(QString(""))
    , m_eClose(eAskAlways)
    , m_eType(eMDIChild)
    , m_bModified(false)
    , m_sName(QString(""))
    , m_ptLastMDIAreaPosition(QPoint(0,0))
{
}

C_childWidget::~C_childWidget()
{
    qDebug() << "Leaving";
}

void C_childWidget::setMDIMode()
{
    m_eType = eMDIChild;
}

void C_childWidget::setDockedMode()
{
    m_eType = eDocked;
}

void C_childWidget::setSeparateMode()
{
    m_eType = eSeparate;
}

C_childWidget::eChildType C_childWidget::getMode()
{
    return m_eType;
}

void C_childWidget::callCloseEvent(QCloseEvent * event)
{
    closeEvent(event);
    if (event->isAccepted())
        this->deleteLater();
}

void C_childWidget::setName(QString sName)
{
    m_sName = sName;
}

QString C_childWidget::getName()
{
    return m_sName;
}

void C_childWidget::moveEvent(QMoveEvent *event)
{
    emit(moved(event->pos()));
    QWidget::moveEvent(event);
}

void C_childWidget::closeEvent(QCloseEvent *event)
{
    bool bAccept = false;
    bool bSave = false;
    switch (m_eClose)
    {
    case eAskAskBasedOnState: //if not unmodified, will ask as in eAskAlways
    {
        if (!m_bModified)
        {
            this->setParent(Q_NULLPTR);
            bAccept = true;
            break;
        }
        break;
    }
    case eAskAlways:
    {
        int ans = QMessageBox::question(this, tr("Save?"), tr("Do you want to save?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ans)
        {
        case QMessageBox::Save:
        {
            bSave = true;
            bAccept = true;
            break;
        }
        case QMessageBox::Discard:
        {
            bSave = false;
            bAccept = true;
            break;
        }
        case QMessageBox::Cancel:
        {
            bSave = false;
            bAccept = false;
            break;
        }
        }
        break;
    }
    case eHide:
    {
        bSave = false;
        bAccept = true;

        if (m_eType == eMDIChild)
            m_ptLastMDIAreaPosition = mapToGlobal(pos());

        this->setParent(Q_NULLPTR);
        this->hide();
        emit(hidding());
        break;
    }
    }
    if (bSave)
        saveThis();
    if (bAccept)
        event->accept();
    else
        event->ignore();
}
