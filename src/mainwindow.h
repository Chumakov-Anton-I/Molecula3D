#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class Canvas3D;
class Scene;
class QPushButton;
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

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void makeActions();

    Canvas3D *m_view;   // view
    Scene    *m_scene;  // model
    QTreeView *m_structure;

    QToolBar *m_mainToolbar;

    QAction *m_actAddItem;
    QAction *m_actRemoveItem;
    QAction *m_actResetView;
};

#endif // MAINWINDOW_H
