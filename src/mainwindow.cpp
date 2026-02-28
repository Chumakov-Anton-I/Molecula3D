#include "mainwindow.h"
#include "canvas3d.h"
#include "dialogaddatom.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QKeyEvent>
#include <QToolBar>
#include <QAction>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Molecula 3D"));

    auto *topLayout = new QVBoxLayout;
    topLayout->setContentsMargins(1, 1, 1, 1);
    setLayout(topLayout);

    // right bar - scene
    m_view = new Canvas3D;
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
}

void MainWindow::slotAdd()
{
    auto *dlg = new DialogAddAtom(this);
    if (dlg->exec() == QDialog::Accepted)
        m_view->addAtom(dlg->position(), dlg->radius());
    delete dlg;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        m_view->clearSelection();
        break;
    case Qt::Key_Delete:
        m_view->deleteSelected();
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
    connect(m_actRemoveItem, &QAction::triggered, m_view, &Canvas3D::deleteSelected);

    //m_actResetView = new QAction(tr("Reset view"), this);
    //connect(m_actResetView, &QAction::triggered, m_scene, &Scene::resetView);
}
