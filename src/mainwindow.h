#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHash>

#include "defines.h"

class Canvas3D;
class Scene;
class QToolBar;
class QTreeView;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
signals:
    
public slots:
    void slotAdd();

private slots:
    void loadPTable();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void makeActions();

    QHash<short, Element> m_Table;

    Canvas3D *m_view;   // view
    Scene    *m_scene;  // model
    QTreeView *m_structure;

    QToolBar *m_mainToolbar;

    QAction *m_actAddItem;
    QAction *m_actRemoveItem;
    QAction *m_actResetView;
};

#endif // MAINWINDOW_H
