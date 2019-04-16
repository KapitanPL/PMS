#include "DocumentView.h"
#include <QVBoxLayout>
#include <QPushButton>

C_DocumentView::C_DocumentView(QWidget *parent)
    : C_childWidget(parent)
{
    this->setMDIMode();
    this->m_eClose = eAskAskBasedOnState;
    QVBoxLayout * layoutMainV = new QVBoxLayout;
    this->setLayout(layoutMainV);
    layoutMainV->addWidget(new QPushButton("Button1"));
    layoutMainV->addWidget(new QPushButton("Button1"));
    layoutMainV->addWidget(new QPushButton("Button1"));
}

C_DocumentView::~C_DocumentView()
{

}

void C_DocumentView::saveThis()
{

}
