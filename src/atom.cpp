#include "atom.h"

Atom::Atom(const Element &param, const QVector3D &position, Atom *parent)
    : Sphere(position), m_params(param), m_parent{parent}
{
    setRadius(m_params.radius/10.0);
    qDebug() << "================\nAtom created:\n"
             << QString("Element: %1\n").arg(m_params.name)
             << QString(" Radius: %1\n").arg(m_params.radius);
}

Atom::~Atom()
{
    qDebug() << "Atom: deleted";
}
