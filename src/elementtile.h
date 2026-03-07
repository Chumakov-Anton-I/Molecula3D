#ifndef ELEMENTTILE_H
#define ELEMENTTILE_H

#include <QFrame>

#include "defines.h"

class QLabel;

class ElementTile : public QFrame
{
    Q_OBJECT
public:
    explicit ElementTile(QWidget *parent = nullptr);

public slots:
    void setElement(const Element &e);

private:
    QLabel *m_number;
    QLabel *m_mass;
    QLabel *m_symbol;
    QLabel *m_name;
    QLabel *m_class;
};

#endif // ELEMENTTILE_H
