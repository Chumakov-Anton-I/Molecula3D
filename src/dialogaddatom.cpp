#include "dialogaddatom.h"

#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>

DialogAddAtom::DialogAddAtom(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle(tr("Add new item"));
    auto *form = new QFormLayout;
    setLayout(form);

    m_X = new QDoubleSpinBox;
    m_Y = new QDoubleSpinBox;
    m_Z = new QDoubleSpinBox;
    m_X->setRange(-1000.0, 1000.0);
    m_Y->setRange(-1000.0, 1000.0);
    m_Z->setRange(-1000.0, 1000.0);

    form->addRow(" X ", m_X);
    form->addRow(" Y ", m_Y);
    form->addRow(" Z ", m_Z);

    auto *bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    form->addRow(bbox);
    connect(bbox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bbox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setFixedSize(220, 140);
}

QVector3D DialogAddAtom::position() const
{
    return QVector3D(m_X->value(), m_Y->value(), m_Z->value());
}
