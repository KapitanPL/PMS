#ifndef C_DOCUMENTVIEW_H
#define C_DOCUMENTVIEW_H

#include "./../widgets/childWidget/c_childwidget.h"

class C_DocumentView : public C_childWidget
{
    Q_OBJECT
public:
    C_DocumentView(QWidget * parent = Q_NULLPTR);
    ~C_DocumentView();
    virtual void saveThis();
};

#endif //C_DOCUMENTVIEW_H
