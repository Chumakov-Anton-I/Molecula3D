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

#endif // DEFINES_H
