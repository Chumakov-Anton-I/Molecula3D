#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>

#include "defines.h"

class Canvas3D;
class Scene;
class QToolBar;
class QTreeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotAdd();

private slots:
    void loadPTable();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void makeActions();
    void makeToolBars();
    void makeMenus();

    QHash<short, Element> m_Table;

    Canvas3D *m_view;   // view
    Scene    *m_scene;  // model
    QTreeView *m_structure;

    QToolBar *m_mainToolbar;

    QAction *m_actNewFile;
    QAction *m_actOpenFile;
    QAction *m_actSaveFile;
    QAction *m_actAddItem;
    QAction *m_actRemoveItem;
    QAction *m_actRemoveItemBranch;
    QAction *m_actResetView;
};

#endif // MAINWINDOW_H
