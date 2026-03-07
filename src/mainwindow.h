#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Canvas3D;
class Scene;
class ElementLibrary;
class Atom;
class QToolBar;
class StructureView;
class AtomInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotNewFile();
    void slotOpenFile();
    void slotSaveFile();
    void slotSaveFileAs();
    void slotAdd();
    void slotAddAtom(Atom *atom);

    void slotShowAtomInfo(const QVector3D &ray, const QVector3D &origin);

private slots:

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void makeActions();
    void makeToolBars();
    void makeMenus();

    Canvas3D *m_view;   // view
    Scene    *m_scene;  // model
    ElementLibrary *m_library;
    StructureView *m_structure;
    AtomInfo *m_atomInfo;

    QToolBar *m_mainToolbar;

    QAction *m_actNewFile;
    QAction *m_actOpenFile;
    QAction *m_actSaveFile;
    QAction *m_actSaveFileAs;
    QAction *m_actAddItem;
    QAction *m_actRemoveItem;
    QAction *m_actRemoveItemBranch;
    QAction *m_actResetView;
    QAction *m_actViewAll;
};

#endif // MAINWINDOW_H
