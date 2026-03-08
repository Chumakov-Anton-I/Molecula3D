#include "elementtile.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

ElementTile::ElementTile(QWidget *parent)
    : QFrame{parent}
{
    setMinimumWidth(380);
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setLineWidth(1);

    auto *topLayout = new QVBoxLayout;
    setLayout(topLayout);

    // top line: number - [gap] - mass
    auto *topLine = new QHBoxLayout;
    topLine->setContentsMargins(1, 1, 1, 1);
    topLayout->addLayout(topLine);
    m_number = new QLabel("0");
    m_number->setFont(QFont("Open Sans", 22));
    topLine->addWidget(m_number, 0, Qt::AlignLeft);
    topLine->addStretch(1);
    m_mass  = new QLabel("0.0");
    m_mass->setFont(QFont("Open Sans", 20));
    topLine->addWidget(m_mass, 0, Qt::AlignRight);

    // symbol
    m_symbol = new QLabel;
    m_symbol->setFont(QFont("Open Sans SemiBold", 48));
    topLayout->addWidget(m_symbol, 1, Qt::AlignCenter);

    // name
    m_name = new QLabel(tr("name"));
    m_name->setFont(QFont("Open Sans", 22));
    topLayout->addWidget(m_name, 1, Qt::AlignCenter);

    // class
    m_class = new QLabel(tr("Family"));
    m_class->setFont(QFont("Open Sans", 16));
    topLayout->addWidget(m_class, 1, Qt::AlignCenter);

    m_colors.insert("nonmetal",       "#A0FFA0");
    m_colors.insert("noble",          "#C0FFFF");
    m_colors.insert("alkali",         "#FF6666");
    m_colors.insert("alkaline_earth", "#FFDEAD");
    m_colors.insert("metalloid",      "#CCCC99");
    m_colors.insert("halogen",        "#FFFF99");
    m_colors.insert("post_transit",   "#CCCCCC");
    m_colors.insert("transit",        "#FFC0C0");
    m_colors.insert("lanthanide",     "#FFBFFF");
    m_colors.insert("actinide",       "#EF99CC");
}

void ElementTile::setElement(const Element &e)
{
    m_number->setText(QString::number(e.number));
    //m_mass->setText(QString::number(e.mass));
    m_symbol->setText(e.sign);
    m_name->setText(e.name);
    m_class->setText(e.type);
    setStyleSheet(QString("background-color:%1;").arg(m_colors.value(e.type)));
}

