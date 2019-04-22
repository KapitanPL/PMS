#include "c_logwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QDir>
#include <QDataStream>
#include <QDateTime>
#include <QCheckBox>
#include <QJsonObject>
#include <QFileDialog>

C_logWindow::C_logWindow(QWidget *parent)
    : C_childWidget(parent)
    , m_pEdit(nullptr)
    , m_pFile(Q_NULLPTR)
    , m_pSaveThread(Q_NULLPTR)
{
    this->setMDIMode();
    this->m_eClose = eHide;
    QVBoxLayout * layoutMainV = new QVBoxLayout;
    this->setLayout(layoutMainV);
    QHBoxLayout * layoutDir = new QHBoxLayout;

    m_checkEnable = new QCheckBox(tr("Enable logging"));
    m_checkEnable->setObjectName("checkEnable");
    connect(m_checkEnable, &QCheckBox::toggled, this, &C_logWindow::checkEnableChanged);
    m_checkEnable->setChecked(true);
    layoutMainV->addWidget(m_checkEnable);

    QPushButton * buttonDir = new QPushButton("");
    buttonDir->setIcon(QIcon(":/directory.svg"));
    buttonDir->setObjectName("buttonDirectory");
    connect(buttonDir, &QPushButton::clicked, this, &C_logWindow::selectLogDir);
    layoutDir->addWidget(buttonDir);

    QLabel * labelDir = new QLabel();
    labelDir->setObjectName("labelDir");
    layoutDir->addWidget(labelDir);
    layoutDir->addStretch(1);

    layoutMainV->addLayout(layoutDir);

    m_pEdit = new QTextEdit();
    layoutMainV->addWidget(m_pEdit);
    this->m_sHumanReadableName = tr("Log");

    m_logPath = QDir::currentPath() + QDir::separator() + QString("logs");
    m_logPath = QDir::cleanPath(m_logPath);

    setLogPath(m_logPath);

    startLogger();

    resize(sizeHint());
}

C_logWindow::~C_logWindow()
{
    saveThis();
    if (m_pFile && m_pFile->isOpen())
        m_pFile->close();
    qDebug() << "Destroyed";
}

void C_logWindow::logThis(const QString &sMessage)
{
    if (m_checkEnable && m_checkEnable->isChecked())
    {
        QTime now = QTime::currentTime();
        QString logMsg = now.toString("hh:mm:ss.zzz") + ": " +sMessage/*+"\n"*/;
        m_pEdit->append(logMsg);
        m_mainQueue.enqueue(logMsg);
        if (m_storeQueue.empty())
        {
            if (m_pSaveThread && m_pSaveThread->joinable())
            {
                m_pSaveThread->join(); //this should be really fast as the store queue is empty, this should return imediatly
                m_pSaveThread = nullptr;
            }
            swapQueues(); //set the second queue to be saved
            m_pSaveThread = new std::thread([this]{saveQueue();});
        }
    }
}

void C_logWindow::saveThis()
{
    if (m_pSaveThread && m_pSaveThread->joinable())
    {
        m_pSaveThread->join(); //this should be really fast as the store queue is empty, this should return imediatly
        m_pSaveThread = nullptr;
    }
    swapQueues(); //set the second queue to be saved
    m_pSaveThread = new std::thread([this]{saveQueue();});
    m_pSaveThread->join();
    return;
}

void C_logWindow::saveQueue()
{
    if (m_pFile && m_pFile->isOpen())
    {
        QTextStream out(m_pFile);
        std::lock_guard<std::mutex> swapGuard(m_SwapMutex);
        while (!m_storeQueue.empty())
        {
            out << m_storeQueue.dequeue();
        }
        out.flush();
    }
}

void C_logWindow::swapQueues()
{
    std::lock_guard<std::mutex> swapGuard(m_SwapMutex);
    m_storeQueue.swap(m_mainQueue);
}

void C_logWindow::selectLogDir(bool)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Log Directory"),m_logPath, QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if (dir != m_logPath)
    {
        setLogPath(dir);
    }
}

void C_logWindow::startLogger()
{
    if (!QDir().exists(m_logPath))
    {
        QDir().mkdir(m_logPath);
    }
    QDateTime now = QDateTime::currentDateTime();
    QDate date = now.date();
    QTime time = now.time();
    QString name = QString("%1-%2-%3-(%4-%5)").arg(date.year()).arg(date.month()).arg(date.day()).arg(time.hour()).arg(time.minute());
    m_pFile = new QFile(QString("%1/%2.log").arg(m_logPath ).arg(name));
    m_pFile->open(QIODevice::Append | QIODevice::Text);

    logThis(QString("Program started started"));
}

void C_logWindow::checkEnableChanged(bool)
{
    QCheckBox * pCheck = qobject_cast<QCheckBox*>(sender());
    if (pCheck)
    {
        QPushButton * pDirButton = findChild<QPushButton*>("buttonDirectory");
        if (pDirButton)
            pDirButton->setVisible(pCheck->isChecked());
        QLabel * pDirLabel = findChild<QLabel*>("labelDir");
        if (pDirLabel)
            pDirLabel->setVisible(pCheck->isChecked());
    }
}

void C_logWindow::setLogPath(QString path)
{
    if (path != m_logPath)
    {
        //save what is running
        if (m_pSaveThread && m_pSaveThread->joinable())
        {
            m_pSaveThread->join(); //wait for completing
        }

        //move file
        m_pFile->rename(QDir::cleanPath( path + QDir::separator() + *m_pFile->fileName().split(QDir::separator()).rbegin() ));

        m_pFile->open(QIODevice::Append | QIODevice::Text);

        //set new
        m_logPath = path;
    }
    QLabel * labelDir = findChild<QLabel*>("labelDir");
    if (m_logPath.contains(QDir::currentPath()))
    {
        labelDir->setText(*m_logPath.split(QDir::separator()).rbegin());
    }
    else
    {
        labelDir->setText(path);
    }
}

QJsonObject C_logWindow::saveToJson()
{
    QJsonObject variant;
    variant["Name"] = m_sHumanReadableName;
    variant["dirPath"] = m_logPath;
    QCheckBox * check = findChild<QCheckBox*>("checkEnable");
    if (check)
        variant["bEnableLogging"] = check->isChecked();
    return variant;
}

void C_logWindow::loadFromJson(QJsonObject variant)
{
    QString keyWord = "Name";
    if (variant.contains(keyWord) && variant[keyWord].isString())
        m_sHumanReadableName = variant[keyWord].toString();
    keyWord = "dirPath";
    if (variant.contains(keyWord) && variant[keyWord].isString())
    {
        setLogPath(variant[keyWord].toString());
    }
    keyWord = "bEnableLogging";
    QCheckBox * check = findChild<QCheckBox*>("checkEnable");
    if (variant.contains(keyWord) && variant[keyWord].isBool() && check)
    {
        check->setChecked(variant[keyWord].toBool());
    }
}

