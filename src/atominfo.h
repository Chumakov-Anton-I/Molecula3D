#ifndef ATOMINFO_H
#define ATOMINFO_H

#include <QDialog>

class Atom;
class ElementTile;
class QLineEdit;

class AtomInfo : public QDialog
{
    Q_OBJECT
public:
    explicit AtomInfo(QWidget *parent = nullptr);

    void setAtom(Atom *atom);

protected:
    void closeEvent(QCloseEvent *event);

private:
    ElementTile *m_tile;
    QLineEdit *m_name;
    QLineEdit *m_id;
    QLineEdit *m_valence;
    QLineEdit *m_X;
    QLineEdit *m_Y;
    QLineEdit *m_Z;
};

#endif // ATOMINFO_H
