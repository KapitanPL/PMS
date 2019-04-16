#include "mdichild.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QDebug>
#include <ctime>

MDIChild::MDIChild(QWidget * parent)
    : QMdiSubWindow(parent)
    , m_pWidget(nullptr)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void MDIChild::setWidget(C_childWidget* contentWidget)
{
    m_pWidget = contentWidget;
    QMdiSubWindow::setWidget(contentWidget);
    if (m_pWidget)
    {
        this->setWindowTitle(m_pWidget->m_sHumanReadableName);
        this->resize(m_pWidget->size());
    }
}

void MDIChild::closeEvent(QCloseEvent * event)
{
    if (m_pWidget)
    {
        m_pWidget->callCloseEvent(event);
        if (event->isAccepted())
        {
            setWidget(nullptr);
        }
    }
    else
        event->accept();
}

void MDIChild::paintEvent(QPaintEvent *event)
{
    QMdiSubWindow::paintEvent(event);
}

void MDIChild::moveEvent(QMoveEvent *event)
{
    emit(m_pWidget->moved(mapToParent(event->pos())));
    QMdiSubWindow::moveEvent(event);
}
