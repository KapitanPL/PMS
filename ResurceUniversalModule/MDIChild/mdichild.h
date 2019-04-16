#ifndef MDICHILD_H
#define MDICHILD_H

#include <QMdiSubWindow>
#include "./../widgets/childWidget/c_childwidget.h"

class MDIChild : public QMdiSubWindow
{
    Q_OBJECT
public:
    MDIChild(QWidget * parent = Q_NULLPTR);
    void setWidget(C_childWidget* contentWidget);

protected:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private:
    C_childWidget* m_pWidget;
};

#endif //MDICHILD_H
