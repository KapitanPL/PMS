#ifndef TOOLBOXITEMWIDGET_H
#define TOOLBOXITEMWIDGET_H

#include <QWidget>
#include <QString>
#include <QIcon>

class QToolBoxItemWidget : public QWidget
{
    Q_OBJECT
public:
    QToolBoxItemWidget(QString sName, QIcon &icon, QWidget *pwidget, QWidget * parent = nullptr);
    virtual ~QToolBoxItemWidget();
    virtual QToolBoxItemWidget * addItem(QString sName, QIcon &icon, QWidget *pWidget);
    virtual int count();
    virtual QWidget * getItemAt(int iPos);
private:
    virtual void pushClicked(bool);
};
#endif //C_CHILDWIDGET_H
