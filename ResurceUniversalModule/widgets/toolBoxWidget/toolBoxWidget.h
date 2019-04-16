#ifndef TOOLBOXWIDGET_H
#define TOOLBOXWIDGET_H

#include "toolBoxItemWidget.h"

class QToolBoxWidget : public QWidget
{
    Q_OBJECT
public:
    QToolBoxWidget(QWidget * parent = nullptr);
    virtual ~QToolBoxWidget();
    virtual QToolBoxItemWidget * addItem(QString sName, QIcon &icon, QWidget *pWidget);
    virtual int count();
    virtual QWidget *getItemAt(int iPos);
private:
    virtual void expandAll(bool);
    virtual void colapseAll(bool);
};

#endif //C_CHILDWIDGET_H
