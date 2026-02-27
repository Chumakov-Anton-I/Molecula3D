#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class Scene;
class QPushButton;
class QToolBar;
class QTreeView;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    
signals:
    
public slots:
    void slotAdd();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void makeActions();

    Scene *m_scene;
    QTreeView *m_structure;

    QToolBar *m_mainToolbar;

    QAction *m_actAddItem;
    QAction *m_actRemoveItem;
    QAction *m_actResetView;
    //QPushButton *m_btnAdd;
};

#endif // MAINWINDOW_H
