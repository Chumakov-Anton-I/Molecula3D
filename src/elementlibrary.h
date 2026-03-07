#ifndef ELEMENTLIBRARY_H
#define ELEMENTLIBRARY_H

#include <QDialog>
#include <QHash>

#include "defines.h"

class QListWidget;
class QListWidgetItem;
class QDoubleSpinBox;
class QSpinBox;
class Atom;
class ElementTile;

class ElementLibrary : public QDialog
{
    Q_OBJECT
public:
    explicit ElementLibrary(QWidget *parent = nullptr);

public slots:
    void init();
    void accept();

private slots:
    void selectElement(QListWidgetItem *item);

signals:
    void createAtom(Atom *newAtom);

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QHash<int, Element> m_database;
    QHash<int, QString> m_colorbase;
    QDoubleSpinBox *m_X;
    QDoubleSpinBox *m_Y;
    QDoubleSpinBox *m_Z;
    QSpinBox *m_number;
    QListWidget *m_PTable;
    ElementTile *m_eTile;
};

#endif // ELEMENTLIBRARY_H
