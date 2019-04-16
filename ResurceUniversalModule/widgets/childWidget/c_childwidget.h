#ifndef C_CHILDWIDGET_H
#define C_CHILDWIDGET_H

#include <QWidget>
#include <QString>

class C_childWidget : public QWidget
{
    Q_OBJECT
public:
    enum eChildType{
        eMDIChild   = 0,
        eDocked     = 1,
        eSeparate   = 2,
        eUnknown
    };

    enum eCloseBehaviour{
        eAskAlways          = 0,
        eAskAskBasedOnState = 1,
        eHide               = 2
    };

    C_childWidget(QWidget * parent);
    virtual ~C_childWidget();
    void setMDIMode();
    void setDockedMode();
    void setSeparateMode();
    eChildType getMode();

    void callCloseEvent(QCloseEvent * event);
    void setName(QString sName);
    QString getName();

    virtual void saveThis() = 0; //called while closing
protected:
    void moveEvent(QMoveEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
signals:
    void moved(QPoint point);
    void hidding();
public:
    QString          m_sHumanReadableName;
protected:
    eCloseBehaviour  m_eClose;
private:
   eChildType       m_eType;
   bool             m_bModified;
   QString          m_sName;
   QPoint           m_ptLastMDIAreaPosition;
};

#endif // C_CHILDWIDGET_H
