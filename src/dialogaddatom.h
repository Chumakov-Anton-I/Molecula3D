#ifndef DIALOGADDATOM_H
#define DIALOGADDATOM_H

// A temporary dialog which adds an item to the scene

#include <QDialog>
#include <QVector3D>

class QDoubleSpinBox;

class DialogAddAtom : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAddAtom(QWidget *parent = nullptr);

    QVector3D position() const;
    double radius() const;

private:
    QDoubleSpinBox *m_X;
    QDoubleSpinBox *m_Y;
    QDoubleSpinBox *m_Z;
    QDoubleSpinBox *m_R;
};

#endif // DIALOGADDATOM_H
