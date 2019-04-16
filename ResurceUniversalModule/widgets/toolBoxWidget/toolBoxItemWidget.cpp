#include "toolBoxItemWidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

QToolBoxItemWidget::QToolBoxItemWidget(QString sName, QIcon &icon, QWidget * pwidget, QWidget *parent)
    :QWidget(parent)
{
    QVBoxLayout * pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0,0,0,0);
    pLayout->setSpacing(2);
    QPushButton * pPush = new QPushButton();
    pPush->setObjectName("push");
    pPush->setCheckable(true);
    pPush->setChecked(true);
    connect(pPush, &QPushButton::clicked, this, &QToolBoxItemWidget::pushClicked);
    pLayout->addWidget(pPush, 0);
    QScrollArea * pArea = new QScrollArea();
    pArea->setObjectName("scroll");
    pLayout->addWidget(pArea);
    pLayout->addStretch(1);
    QVBoxLayout * pAreaLayout = new QVBoxLayout(pArea);
    pAreaLayout->setSpacing(1);
    pAreaLayout->setContentsMargins(0,0,0,0);
    pAreaLayout->setObjectName("area");
    pPush->setText(sName);
    pPush->setIcon(icon);
    if (pwidget)
        pAreaLayout->addWidget(pwidget);
}

QToolBoxItemWidget::~QToolBoxItemWidget()
{

}

QToolBoxItemWidget *QToolBoxItemWidget::addItem(QString sName, QIcon &icon, QWidget * pWidget)
{
    QVBoxLayout * pLayout = this->findChild<QVBoxLayout*>("area");
    if (pLayout)
    {
        QToolBoxItemWidget * pNew = new QToolBoxItemWidget(sName, icon, pWidget, this);
        pLayout->addWidget(pNew);
        return pNew;
    }
    return nullptr;
}

int QToolBoxItemWidget::count()
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

QWidget *QToolBoxItemWidget::getItemAt(int iPos)
{
    QVBoxLayout * pLayout = this->findChild<QVBoxLayout*>("area");
    if (iPos >-1 && iPos < count() && pLayout)
    {
        return qobject_cast<QWidget*>(pLayout->itemAt(iPos)->widget());
    }
    else
        return nullptr;
}

void QToolBoxItemWidget::pushClicked(bool)
{
    QPushButton * pPush = this->findChild<QPushButton*>("push");
    QScrollArea * pScroll = this->findChild<QScrollArea*>("scroll");
    if (pPush && pScroll)
    {
        pScroll->setVisible(pPush->isChecked());
    }
}
