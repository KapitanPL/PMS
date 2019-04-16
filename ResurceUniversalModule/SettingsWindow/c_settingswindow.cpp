#include "c_settingswindow.h"
#include <QJsonObject>
#include <QFileDialog>
#include <QDebug>
#include <QVBoxLayout>
#include <QQuickView>
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>

C_settingsWindow::C_settingsWindow(QWidget *parent, QString /*sQML*/, QSharedPointer<s_globals> globals)
    :C_childWidget(parent)
    , m_pGlobals(globals)
{
    this->setMDIMode();
    this->m_eClose = eHide;
    this->m_sHumanReadableName = tr("Settings");
    //QVBoxLayout * layoutMainV = new QVBoxLayout;
    //this->setLayout(layoutMainV);
    /*QQuickView *view = new QQuickView();
    QWidget *container = QWidget::createWindowContainer(view, parent ? parent : this);
    view->setSource(QUrl::fromLocalFile(sQML));*/
    m_puiMain = new Ui_Form;
    m_puiMain->setupUi(this);
    //layoutMainV->addWidget(container);
    //m_puiMain->EditPlugins->setText(m_pGlobals ? m_pGlobals->m_sPluginPath : "./Plugs");
    if (m_pGlobals && !m_pGlobals->m_listPluginPath.isEmpty())
    {
        for (auto sPath : m_pGlobals->m_listPluginPath)
        {
            addPluginPathGroup(sPath);
        }
    }

    connect(m_puiMain->pushAddPluginPath, &QToolButton::clicked, this, [this](){addPluginPathGroup("");});
    /*connect(m_puiMain->pushPluginsPath, &QPushButton::clicked, this, &C_settingsWindow::selectPluginDir);
    connect(m_puiMain->EditPlugins, &QLineEdit::editingFinished, this,
            [this](){setPluginPath(m_puiMain->EditPlugins->text());}
    );*/
}

void C_settingsWindow::saveThis()
{
}

QJsonObject C_settingsWindow::saveToJson()
{
    QJsonObject variant;
    variant["Name"] = m_sHumanReadableName;
    return variant;
}

void C_settingsWindow::loadFromJson(QJsonObject variant)
{
    QString keyWord = "Name";
    if (variant.contains(keyWord) && variant[keyWord].isString())
        m_sHumanReadableName = variant[keyWord].toString();
    /*keyWord = "dirPath";
    if (variant.contains(keyWord) && variant[keyWord].isString())
    {
        setLogPath(variant[keyWord].toString());
    }
    keyWord = "bEnableLogging";
    QCheckBox * check = findChild<QCheckBox*>("checkEnable");
    if (variant.contains(keyWord) && variant[keyWord].isBool() && check)
    {
        check->setChecked(variant[keyWord].toBool());
    }*/
}

QString C_settingsWindow::selectPluginDir(bool)
{
    return QFileDialog::getExistingDirectory(this, tr("Plugins Directory"),"./", QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
}

void C_settingsWindow::addPluginPath(QString path, bool bAdd, QLineEdit * pEdit)
{
    if (bAdd)
    {
        if (path.contains(QDir::currentPath()))
        {
            pEdit->setText(*path.split(QDir::separator()).rbegin());
        }
        if (!QDir().exists(path) && path.isEmpty())
        {
            QPalette palette = pEdit->palette();
            palette.setBrush(QPalette::Base, QBrush(Qt::red));
            pEdit->setPalette(palette);
        }
        else
        {
            QPalette palette = pEdit->palette();
            palette.setBrush(QPalette::Base, QBrush(Qt::white));
            pEdit->setPalette(palette);
        }
        if (std::find(m_pGlobals->m_listPluginPath.begin(), m_pGlobals->m_listPluginPath.end(), path) == m_pGlobals->m_listPluginPath.end())
        {
            m_pGlobals->m_listPluginPath.append(path);
        }
    }
    else
    {
        auto itPath = std::find(m_pGlobals->m_listPluginPath.begin(), m_pGlobals->m_listPluginPath.end(), path);
        if (itPath != m_pGlobals->m_listPluginPath.end())
        {
            m_pGlobals->m_listPluginPath.erase(itPath);
        }
    }
    emit (pluginPathChanged());
}

void C_settingsWindow::addPluginPathGroup(QString sPath)
{
    QHBoxLayout * itemLayout = new QHBoxLayout();
    QLineEdit * editPluginPath = new QLineEdit(sPath);
    if (!QDir().exists(sPath) && !sPath.isEmpty())
    {
        QPalette palette = editPluginPath->palette();
        palette.setBrush(QPalette::Base, QBrush(Qt::red));
        editPluginPath->setPalette(palette);
    }
    itemLayout->addWidget(editPluginPath, 1);
    QToolButton * buttonDir = new QToolButton();
    buttonDir->setIcon(QIcon(":/directory.svg"));
    itemLayout->addWidget(buttonDir);
    QToolButton * buttonRemove = new QToolButton();
    buttonRemove->setIcon(QIcon(":/remove.svg"));
    itemLayout->addWidget(buttonRemove);
    m_puiMain->layoutPluginEdits->addLayout(itemLayout);

    connect(buttonDir, &QToolButton::clicked, this, [this, editPluginPath](){editPluginPath->setText(selectPluginDir(true)); emit(editPluginPath->editingFinished());});
    connect(editPluginPath, &QLineEdit::editingFinished, this, [this, editPluginPath](){addPluginPath(editPluginPath->text(), true, editPluginPath);});
    connect(buttonRemove, &QToolButton::clicked, this,
    [this, itemLayout, editPluginPath, buttonDir, buttonRemove]()
    {
        addPluginPath(editPluginPath->text(), true, editPluginPath);
        delete editPluginPath;
        delete buttonDir;
        delete buttonRemove;
        m_puiMain->layoutPluginEdits->removeItem(itemLayout);
    });
}

C_settingsWindow::~C_settingsWindow()
{
    qDebug() << "Destroyed";
}
