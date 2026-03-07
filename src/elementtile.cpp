#include "elementtile.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

ElementTile::ElementTile(QWidget *parent)
    : QWidget{parent}
{
    setMinimumWidth(256);

    auto *topLayout = new QVBoxLayout;
    setLayout(topLayout);

    // top line: number - [gap] - mass
    auto *topLine = new QHBoxLayout;
    topLine->setContentsMargins(1, 1, 1, 1);
    topLayout->addLayout(topLine);
    m_number = new QLabel("0");
    topLine->addWidget(m_number, 0, Qt::AlignLeft);
    topLine->addStretch(1);
    m_mass  = new QLabel("0.0");
    topLine->addWidget(m_mass, 0, Qt::AlignRight);

    // symbol
    m_symbol = new QLabel;
    topLayout->addWidget(m_symbol, 1, Qt::AlignCenter);

    // name
    m_name = new QLabel(tr("name"));
    topLayout->addWidget(m_name, 1, Qt::AlignCenter);

    // class
    m_class = new QLabel(tr("Family"));
    topLayout->addWidget(m_class, 1, Qt::AlignCenter);
}

void ElementTile::setElement(const Element &e)
{
    m_number->setText(QString::number(e.number));
    //m_mass->setText(QString::number(e.mass));
    m_symbol->setText(e.sign);
    m_name->setText(e.name);
    m_class->setText(e.type);
}
