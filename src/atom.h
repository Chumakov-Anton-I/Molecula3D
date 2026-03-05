#ifndef ATOM_H
#define ATOM_H

#include "sphere.h"
#include "defines.h"

class Atom : public Sphere
{
public:
    Atom(const Element &param, const QVector3D &position, Atom *parent = nullptr);
    ~Atom();

private:
    QString m_elementName;
    QString m_id;
    Element m_params;
    Atom *m_parent;
    QList<Atom *> m_children;
};

#endif // ATOM_H
