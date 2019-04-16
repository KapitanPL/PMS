#include "toolBoxWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>

QToolBoxWidget::QToolBoxWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout * pLayout = new QVBoxLayout(this);
    pLayout->setSpacing(1);
    pLayout->setContentsMargins(0,0,0,0);
    QPushButton * pPushExpand = new QPushButton();
    pPushExpand->setObjectName("pushExpand");
    pPushExpand->setIcon(QIcon(":/expand_all.svg"));
    QPushButton * pPushColapse = new QPushButton();
    pPushColapse->setObjectName("pushColapse");
    pPushColapse->setIcon(QIcon(":/colapse_all.svg"));
    QHBoxLayout * pButtonLayout = new QHBoxLayout();
    pButtonLayout->addWidget(pPushExpand);
    pButtonLayout->addWidget(pPushColapse);
    pButtonLayout->addStretch(1);
    pLayout->addLayout(pButtonLayout);

    connect(pPushExpand, &QPushButton::clicked, this, &QToolBoxWidget::expandAll);
    connect(pPushColapse, &QPushButton::clicked, this, &QToolBoxWidget::colapseAll);
    QScrollArea * pArea = new QScrollArea();
    pArea->setObjectName("scroll");
    pLayout->addWidget(pArea);
    //pLayout->addStretch(1);
    QVBoxLayout * pAreaLayout = new QVBoxLayout(pArea);
    pAreaLayout->setSpacing(1);
    pAreaLayout->setContentsMargins(0,0,0,0);
    pAreaLayout->setObjectName("area");
}

QToolBoxWidget::~QToolBoxWidget()
{

}

QToolBoxItemWidget *QToolBoxWidget::addItem(QString sName, QIcon &icon, QWidget * pWidget)
{
    QVBoxLayout * pLayout = this->findChild<QVBoxLayout*>("area");
    if (pLayout)
    {
        QToolBoxItemWidget * pNew = new QToolBoxItemWidget(sName, icon, pWidget, this);
        pLayout->addWidget(pNew);
        return pNew;
    }
    else
        return nullptr;
}

int QToolBoxWidget::count()
{
    QVBoxLayout * pLayout = this->findChild<QVBoxLayout*>("area");
    int iCount = 0;
    if (pLayout)
    {
        for (int i =0; i < pLayout->count(); i++)
        {
            if (qobject_cast<QWidget*>(pLayout->itemAt(i)->widget()))
                iCount++;
        }
    }
    return iCount;
}

QWidget *QToolBoxWidget::getItemAt(int iPos)
{
    QVBoxLayout * pLayout = this->findChild<QVBoxLayout*>("area");
    if (iPos >-1 && iPos < count() && pLayout)
    {
        return qobject_cast<QWidget*>(pLayout->itemAt(iPos)->widget());
    }
    else
        return nullptr;
}

void QToolBoxWidget::expandAll(bool)
{
    QVBoxLayout * pLayout = this->findChild<QVBoxLayout*>("area");
    if (pLayout)
    {
        for (int i =0; i < pLayout->count(); i++)
        {
            if (qobject_cast<QToolBoxItemWidget*>(pLayout->itemAt(i)->widget()))
            {

            }
        }
    }
}

void QToolBoxWidget::colapseAll(bool)
{
    QVBoxLayout * pLayout = this->findChild<QVBoxLayout*>("area");
    if (pLayout)
    {
        for (int i =0; i < pLayout->count(); i++)
        {
            if (qobject_cast<QToolBoxItemWidget*>(pLayout->itemAt(i)->widget()))
            {

            }
        }
    }
}
