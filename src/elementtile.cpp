#include "elementtile.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

ElementTile::ElementTile(QWidget *parent)
    : QFrame{parent}
{
    /* Open Sans Family
        "Open Sans"
        "Open Sans Light"
        "Open Sans SemiBold"
        "Open Sans ExtraBold"
        "Open Sans Condensed Light"
        "Open Sans Condensed"
        "Open Sans Condensed SemiBold"
        "Open Sans Condensed ExtraBold"*/

    setMinimumWidth(256);
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setLineWidth(1);

    auto *topLayout = new QVBoxLayout;
    setLayout(topLayout);

    // top line: number - [gap] - mass
    auto *topLine = new QHBoxLayout;
    topLine->setContentsMargins(1, 1, 1, 1);
    topLayout->addLayout(topLine);
    m_number = new QLabel("0");
    m_number->setFont(QFont("Open Sans", 20));
    topLine->addWidget(m_number, 0, Qt::AlignLeft);
    topLine->addStretch(1);
    m_mass  = new QLabel("0.0");
    m_mass->setFont(QFont("Open Sans", 18));
    topLine->addWidget(m_mass, 0, Qt::AlignRight);

    // symbol
    m_symbol = new QLabel;
    m_symbol->setFont(QFont("Open Sans SemiBold", 36));
    topLayout->addWidget(m_symbol, 1, Qt::AlignCenter);

    // name
    m_name = new QLabel(tr("name"));
    m_name->setFont(QFont("Open Sans", 20));
    topLayout->addWidget(m_name, 1, Qt::AlignCenter);

    // class
    m_class = new QLabel(tr("Family"));
    m_class->setFont(QFont("Open Sans", 16));
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
