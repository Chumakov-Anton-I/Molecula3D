#include "mainwindow.h"
#include "canvas3d.h"
#include "scene.h"
#include "dialogaddatom.h"
#include "atom.h"

#include <QSplitter>
#include <QKeyEvent>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTreeView>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Molecula 3D"));

    // right bar - scene
    m_view = new Canvas3D;
    m_scene = new Scene;
    m_view->setScene(m_scene);
    // left bar - tree view // TODO: make special widget
    m_structure = new QTreeView;
    m_structure->setMinimumWidth(96);
    m_structure->setMaximumWidth(256);
    makeActions();
    makeToolBars();
    makeMenus();

    auto *splitted = new QSplitter(Qt::Horizontal);
    setCentralWidget(splitted);
    splitted->addWidget(m_structure);
    splitted->addWidget(m_view);

    splitted->setChildrenCollapsible(false);
    splitted->setStretchFactor(0, 0);
    splitted->setStretchFactor(1, 1);

    // connect model <--> view
    connect(m_scene, &Scene::queryRendering, m_view, QOverload<>::of(&Canvas3D::update));
    connect(m_view, &Canvas3D::querySelection, m_scene, &Scene::selectWithRay);
    QTimer::singleShot(500, this, &MainWindow::loadPTable); // load the Periodic System after 0.5 sec
}

MainWindow::~MainWindow()
{
    delete m_scene;
}

void MainWindow::slotNewFile()
{

}

void MainWindow::slotOpenFile()
{

}

void MainWindow::slotSaveFile()
{

}

void MainWindow::slotSaveFileAs()
{

}

void MainWindow::slotAdd()
{
    auto *dlg = new DialogAddAtom(this);
    if (dlg->exec() == QDialog::Accepted) {
        int key = dlg->number();
        m_scene->addItem(new Atom(m_Table.value(key), dlg->position()));
    }
    delete dlg;
}

void MainWindow::loadPTable()
{
    QFile ptableFile(":/data/elements.json");
    if (!ptableFile.open(QIODevice::Text | QIODevice::ReadOnly)) {
        qDebug() << "Can not open file 'data/elements.json'";
        return;
    }
    QByteArray fdata = ptableFile.readAll();
    ptableFile.close();

    QJsonParseError parse_errors;
    auto json = QJsonDocument::fromJson(fdata, &parse_errors);
    if (parse_errors.error != QJsonParseError::NoError) {
        qDebug() << "Parse error:\n" << parse_errors.errorString();
        return;
    }
    m_Table.reserve(118);   // Nowadays there are just 118 chemical elements
    auto obj = json.object();
    auto elems = obj["elements"].toArray();
    for (auto it = elems.constBegin(); it != elems.constEnd(); ++it) {
        auto item = it->toObject();
        auto valence = item["valence"].toArray();
        QList<int> valences;
        for (auto vi = valence.constBegin(); vi != valence.constEnd(); ++vi)
            valences.append(vi->toInt(0));
        int key = item["number"].toInt();
        Element e;
        e.number = key;
        e.name   = item["name"].toString("Unknown");
        e.sign   = item["sign"].toString("--");
        e.group  = item["group"].toInt(0);
        e.period = item["period"].toInt(0);
        e.block  = item["block"].toString("--");
        e.type   = item["class"].toString("--");
        e.radius = item["radius"].toDouble(10.0);
        e.electronegativity = item["enegativity"].toDouble(0.0);
        e.valences = valences;
        m_Table.insert(key, e);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        m_scene->clearSelection();
        break;
    default:
        break;
    }
}

void MainWindow::makeActions()
{
    m_actNewFile = new QAction(tr("New file..."), this);
    connect(m_actNewFile, &QAction::triggered, this, &MainWindow::slotNewFile);

    m_actOpenFile = new QAction(tr("Open"), this);
    m_actOpenFile->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(m_actOpenFile, &QAction::triggered, this, &MainWindow::slotOpenFile);

    m_actSaveFile = new QAction(tr("Save"), this);
    m_actSaveFile->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    connect(m_actSaveFile, &QAction::triggered, this, &MainWindow::slotSaveFile);

    m_actSaveFileAs = new QAction(tr("Save as..."), this);
    m_actSaveFileAs->setShortcut(QKeySequence(Qt::Key_F12));
    connect(m_actSaveFileAs, &QAction::triggered, this, &MainWindow::slotSaveFileAs);

    m_actAddItem = new QAction(tr("Add item"), this);
    m_actAddItem->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(m_actAddItem, &QAction::triggered, this, &MainWindow::slotAdd);

    m_actRemoveItem = new QAction(tr("Remove"), this);
    m_actRemoveItem->setShortcut(QKeySequence(Qt::Key_Delete));
    connect(m_actRemoveItem, &QAction::triggered, m_scene, &Scene::deleteSelected);

    m_actRemoveItemBranch = new QAction(tr("Remove branch"), this);
    m_actRemoveItemBranch->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_Delete));
    //connect()

    m_actResetView = new QAction(tr("Reset view"), this);
    //connect(m_actResetView, &QAction::triggered, m_scene, &Scene::resetView);
}

void MainWindow::makeToolBars()
{
    m_mainToolbar = new QToolBar(tr("Main"));
    m_mainToolbar->addAction(m_actAddItem);
    m_mainToolbar->addAction(m_actRemoveItem);
    addToolBar(Qt::TopToolBarArea, m_mainToolbar);
    m_mainToolbar->setMovable(false);
}

void MainWindow::makeMenus()
{
    // Menu 'File'
    auto *menuFile = menuBar()->addMenu(tr("File"));
    menuFile->addAction(m_actNewFile);
    menuFile->addAction(m_actOpenFile);
    menuFile->addAction(m_actSaveFile);
    menuFile->addAction(m_actSaveFileAs);
    menuFile->addSeparator();
    menuFile->addAction(tr("Quit"), QKeySequence(Qt::CTRL | Qt::Key_Q), qApp, &qApp->exit);

    // Menu 'Edit'
    auto *menuEdit = menuBar()->addMenu(tr("Edit"));
    menuEdit->addAction(m_actAddItem);
    menuEdit->addSeparator();
    menuEdit->addAction(m_actRemoveItem);
    menuEdit->addAction(m_actRemoveItemBranch);

    // Menu 'View'
    auto *menuView = menuBar()->addMenu(tr("View"));
    menuView->addAction(m_actResetView);

    // Menu 'Help'
    auto *menuHelp = menuBar()->addMenu(tr("Help"));
    menuHelp->addAction(tr("About"));
    menuHelp->addAction(tr("About Qt"));
}
