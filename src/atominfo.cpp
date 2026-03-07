#include "atominfo.h"
#include "atom.h"
#include "elementtile.h"

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>

AtomInfo::AtomInfo(QWidget *parent)
    : QDialog{parent}
{
    auto *topLayout = new QHBoxLayout;
    setLayout(topLayout);
    auto *form = new QFormLayout;
    topLayout->addLayout(form);
    m_name = new QLineEdit;
    m_name->setReadOnly(true);
    m_id = new QLineEdit;
    m_id->setReadOnly(true);

    form->addRow(tr("Name"), m_name);
    form->addRow(tr("Atom ID"), m_id);

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
}

void AtomInfo::closeEvent(QCloseEvent *event)
{
    setVisible(false);
    event->ignore();    // don't close dialog - just hide
}
