#ifndef C_LOGWINDOW_H
#define C_LOGWINDOW_H

#include "./../widgets/childWidget/c_childwidget.h"
#include <QFile>
#include <QString>
#include <mutex>
#include <thread>
#include <QDataStream>
#include <QQueue>
#include <QTextEdit>

class C_logWindow : public C_childWidget
{
    Q_OBJECT
public:
    C_logWindow(QWidget * parent = nullptr);
    ~C_logWindow();

    virtual void logThis(const QString &sMessage);
    virtual void saveThis();

    QJsonObject saveToJson();
    void        loadFromJson(QJsonObject variant);

protected:
    virtual void saveQueue();
    virtual void swapQueues();
    virtual void selectLogDir(bool);

    QTextEdit * m_pEdit;

private:
    void startLogger();
    void checkEnableChanged(bool);
    void setLogPath(QString path);

    QFile * m_pFile;
    QString m_logPath;
    std::thread * m_pSaveThread;
    QQueue<QString> m_mainQueue;
    QQueue<QString> m_storeQueue;
    std::mutex      m_SwapMutex;
};

#endif // C_LOGWINDOW_H
