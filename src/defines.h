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

    /*Element(const Element &e)
    {
        number = e.number;
        name   = e.name;
        sign   = e.sign;
        group  = e.group;
        period = e.period;
        block  = e.block;
        type   = e.type;
        radius = e.radius;
        electronegativity = e.electronegativity;
        valences = e.valences;
    }

    Element(int _numb, QString _name, QString _sign, int _gr, int _per, QString _block, QString _type, double _r, double _eneg, QList<int> _v)
        : number(_numb), name(_name), sign(_sign), group(_gr), period(_per), block(_block), type(_type), radius(_r), electronegativity(_eneg), valences(_v)
    {}*/
};

#endif // DEFINES_H
