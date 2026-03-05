#include "mainwindow.h"
#include "canvas3d.h"
#include "scene.h"
#include "dialogaddatom.h"
#include "atom.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QKeyEvent>
#include <QToolBar>
#include <QAction>
#include <QTreeView>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Molecula 3D"));

    auto *topLayout = new QVBoxLayout;
    topLayout->setContentsMargins(1, 1, 1, 1);
    setLayout(topLayout);

    // right bar - scene
    m_view = new Canvas3D;
    m_scene = new Scene;
    m_view->setScene(m_scene);
    // left bar - tree view
    m_structure = new QTreeView;
    m_structure->setMinimumWidth(96);
    makeActions();
    m_mainToolbar = new QToolBar(tr("Main"));
    m_mainToolbar->addAction(m_actAddItem);
    m_mainToolbar->addAction(m_actRemoveItem);

    topLayout->addWidget(m_mainToolbar);
    auto *splitted = new QSplitter(Qt::Horizontal);
    topLayout->addWidget(splitted);
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
    case Qt::Key_Delete:
        m_scene->deleteSelected();
        break;
    case Qt::Key_Insert:    // TODO: move to shortcut of action
        slotAdd();
        break;
    default:
        break;
    }
}

void MainWindow::makeActions()
{
    m_actAddItem = new QAction(tr("Add item"), this);
    connect(m_actAddItem, &QAction::triggered, this, &MainWindow::slotAdd);

    m_actRemoveItem = new QAction(tr("Remove"), this);
    connect(m_actRemoveItem, &QAction::triggered, m_scene, &Scene::deleteSelected);

    //m_actResetView = new QAction(tr("Reset view"), this);
    //connect(m_actResetView, &QAction::triggered, m_scene, &Scene::resetView);
}
