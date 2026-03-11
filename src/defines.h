#ifndef DEFINES_H
#define DEFINES_H

#include <QList>

struct Element
{
    int number;
    QString name;
    QString sign;
    int group;
    int period;
    QString block;
    QString type;
    //QList<double> mass;
    double radius;
    double electronegativity;
    QList<int> valences;
};

const int LAST_ELEMENT_NUMBER = 118;    // Nowadays (2026) there are just 118 chemical elements

#endif // DEFINES_H
