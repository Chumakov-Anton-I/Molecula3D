#include "dialogaddatom.h"

#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>

DialogAddAtom::DialogAddAtom(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle(tr("Add new item"));

    auto *topLayout = new QVBoxLayout;
    setLayout(topLayout);

    auto *groupPos = new QGroupBox(tr("Position"));
    topLayout->addWidget(groupPos);
    auto *form = new QFormLayout;
    groupPos->setLayout(form);
    m_X = new QDoubleSpinBox;
    m_Y = new QDoubleSpinBox;
    m_Z = new QDoubleSpinBox;
    m_X->setRange(-1000.0, 1000.0);
    m_Y->setRange(-1000.0, 1000.0);
    m_Z->setRange(-1000.0, 1000.0);

    form->addRow(" X ", m_X);
    form->addRow(" Y ", m_Y);
    form->addRow(" Z ", m_Z);

    auto *groupRadius = new QGroupBox(tr("Radius"));
    topLayout->addWidget(groupRadius);
    auto *radLayout = new QVBoxLayout;
    groupRadius->setLayout(radLayout);
    m_R = new QDoubleSpinBox;
    m_R->setRange(0.1, 100.0);
    m_R->setValue(2.0);
    radLayout->addWidget(m_R);

    topLayout->addStretch(1);

    auto *bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    topLayout->addWidget(bbox);

    connect(bbox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bbox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setFixedSize(260, 240);
}

QVector3D DialogAddAtom::position() const
{
    return QVector3D(m_X->value(), m_Y->value(), m_Z->value());
}

double DialogAddAtom::radius() const
{
    return m_R->value();
}
