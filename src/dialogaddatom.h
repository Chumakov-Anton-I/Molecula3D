#ifndef DIALOGADDATOM_H
#define DIALOGADDATOM_H

#include <QDialog>
#include <QVector3D>

class QDoubleSpinBox;

class DialogAddAtom : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAddAtom(QWidget *parent = nullptr);

    QVector3D position() const;

private:
    QDoubleSpinBox *m_X;
    QDoubleSpinBox *m_Y;
    QDoubleSpinBox *m_Z;
};

#endif // DIALOGADDATOM_H
