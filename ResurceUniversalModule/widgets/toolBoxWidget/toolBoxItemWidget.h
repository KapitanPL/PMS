#ifndef TOOLBOXITEMWIDGET_H
#define TOOLBOXITEMWIDGET_H

#include <QWidget>
#include <QString>
#include <QIcon>
#include <QScrollArea>

class QToolBoxItemWidget : public QWidget
{
    Q_OBJECT
public:
    QToolBoxItemWidget(QString sName, QIcon &icon, QWidget *pwidget, QWidget * parent = nullptr);
    virtual ~QToolBoxItemWidget();
    virtual QToolBoxItemWidget * addItem(QString sName, QIcon &icon, QWidget *pWidget);
    virtual int count();
    virtual QWidget * getItemAt(int iPos);
    virtual int level() const;
private:
    virtual void pushClicked(bool);
    int     m_iLevel = 0;
};

class C_QScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    C_QScrollArea(QWidget * parent = nullptr) : QScrollArea(parent){}
    virtual QSize sizeHint() const override;
};
#endif //C_CHILDWIDGET_H
