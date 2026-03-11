#include "atominfo.h"
#include "atom.h"
#include "elementtile.h"

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QGroupBox>

AtomInfo::AtomInfo(QWidget *parent)
    : QDialog{parent}
{
    auto *topLayout = new QHBoxLayout;
    setLayout(topLayout);

    auto *info = new QVBoxLayout;
    topLayout->addLayout(info);

    auto *groupAtomProps = new QGroupBox(tr("About atom"));
    info->addWidget(groupAtomProps);

    auto *form = new QFormLayout;
    groupAtomProps->setLayout(form);

    m_name = new QLineEdit;
    m_name->setReadOnly(true);
    m_id = new QLineEdit;
    m_id->setReadOnly(true);

    form->addRow(tr("Name"), m_name);
    form->addRow(tr("Atom ID"), m_id);

    auto *groupPos = new QGroupBox(tr("Position"));
    info->addWidget(groupPos);

    auto *formPos = new QFormLayout;
    groupPos->setLayout(formPos);

    m_X = new QLineEdit;
    m_Y = new QLineEdit;
    m_Z = new QLineEdit;
    formPos->addRow(tr("X"), m_X);
    formPos->addRow(tr("Y"), m_Y);
    formPos->addRow(tr("Z"), m_Z);

    // tile
    m_tile = new ElementTile;
    topLayout->addWidget(m_tile);
}

void AtomInfo::setAtom(Atom *atom)
{
    Element e = atom->elementParams();
    m_tile->setElement(e);
    m_name->setText(e.name);
    m_id->setText(atom->atomID());
    auto pos = atom->position();
    m_X->setText(QString::number(pos.x(), 'f', 6));
    m_Y->setText(QString::number(pos.y(), 'f', 6));
    m_Z->setText(QString::number(pos.z(), 'f', 6));
}

void AtomInfo::closeEvent(QCloseEvent *event)
{
    setVisible(false);
    event->ignore();    // don't close dialog - just hide
}
