#include "toolBoxItemWidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>



QColor levelColor(int iLevel)
{
    QColor ret;
    int iVal = 64*(iLevel % 3);
    iVal = iVal % 255;
    switch (iLevel % 3)
    {
    case 0:
    {
        ret = QColor::fromRgb(255 - iVal, 255, 255);
        break;
    }
    case 1:
    {
        ret = QColor::fromRgb(255, 255 - iVal, 255);
        break;
    }
    case 2:
    {
        ret = QColor::fromRgb(255, 255, 255-iVal);
        break;
    }
    }
    return ret;
}

QToolBoxItemWidget::QToolBoxItemWidget(QString sName, QIcon &icon, QWidget * pwidget, QWidget *parent)
    :QWidget(parent)
{
    QToolBoxItemWidget *pParentTool = qobject_cast<QToolBoxItemWidget*>(parent);
    if (pParentTool)
        m_iLevel = pParentTool->level()+1;
    QVBoxLayout * pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0,0,0,0);
    pLayout->setSpacing(2);
    QHBoxLayout * pButtonLayout = new QHBoxLayout();
    pButtonLayout->setContentsMargins(0,0,0,0);
    pLayout->addLayout(pButtonLayout);
    QPushButton * pPush = new QPushButton();
    pPush->setObjectName("push");
    pPush->setFixedHeight(20);
    pPush->setCheckable(true);
    pPush->setChecked(true);
    connect(pPush, &QPushButton::clicked, this, &QToolBoxItemWidget::pushClicked);
    pButtonLayout->addWidget(pPush, 0);
    pButtonLayout->addStretch(1);

    QHBoxLayout * hLayTree = new QHBoxLayout();
    hLayTree->setSpacing(0);
    hLayTree->setContentsMargins(0,0,0,0);

    QWidget * pColorWidget = new QWidget();
    pColorWidget->setFixedWidth(5);
    QPalette pal = pColorWidget->palette();
    QPalette originalPalette = pal;

    // set black background
    pal.setColor(QPalette::Background, levelColor(m_iLevel));
    pColorWidget->setAutoFillBackground(true);
    pColorWidget->setPalette(pal);

    setAutoFillBackground(true);
    setPalette(pal);

    hLayTree->addWidget(pColorWidget);

    pLayout->addLayout(hLayTree);

    C_QScrollArea * pArea = new C_QScrollArea();
    pArea->setPalette(originalPalette);
    pArea->setAutoFillBackground(true);
    pArea->setObjectName("scroll");
    pArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    hLayTree->addWidget(pArea);
    pLayout->addStretch(1);
    QVBoxLayout * pAreaLayout = new QVBoxLayout(pArea);
    pAreaLayout->setSpacing(1);
    pAreaLayout->setContentsMargins(0,0,0,0);
    pAreaLayout->setObjectName("area");
    pPush->setText(sName);
    pPush->setIcon(icon);
    if (pwidget)
    {
        pAreaLayout->addWidget(pwidget);
    }
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

int QToolBoxItemWidget::level() const
{
    return m_iLevel;
}

void QToolBoxItemWidget::pushClicked(bool)
{
    QPushButton * pPush = this->findChild<QPushButton*>("push");
    C_QScrollArea * pScroll = this->findChild<C_QScrollArea*>("scroll");
    if (pPush && pScroll)
    {
        pScroll->setVisible(pPush->isChecked());
    }
}

QSize C_QScrollArea::sizeHint() const
{
    return layout()->totalSizeHint();
}
