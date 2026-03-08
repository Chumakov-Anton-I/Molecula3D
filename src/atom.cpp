#include "atom.h"
#include "canvas3d.h"

Atom::Atom(const Element &param, const QVector3D &position, Atom *parent)
    : SceneItem(position), m_params(param), m_parent{parent}
{
    m_sphere.setPosition(position);
    m_sphere.setRadius(m_params.radius/10.0);

    m_matrix.translate(m_position);
}

Atom::~Atom()
{
    qDebug() << QString("Atom '%1' deleted").arg(m_params.name);
}

void Atom::setColor(const QVector3D &color)
{
    m_sphere.setColor(color);
}

bool Atom::rayIntersect(const QVector3D &rayOrigin, const QVector3D &ray, double &dist)
{
    //if (!m_selected)
    return m_sphere.rayIntersect(rayOrigin, ray, dist);
}

void Atom::select(bool on)
{
    m_selected = on;
    m_sphere.select(on);
}

void Atom::draw(Canvas3D *canvas)
{
    m_sphere.draw(canvas);
    if (m_selected) {
        double len = 1.5 * m_params.radius/10.0;
        canvas->drawDot(m_matrix.map(QVector3D(len, 0.0f, 0.0f)), QVector3D(1.0f, 0.0f, 0.0f));
        canvas->drawDot(m_matrix.map(QVector3D(0.0f, len, 0.0f)), QVector3D(0.0f, 1.0f, 0.0f));
        canvas->drawDot(m_matrix.map(QVector3D(0.0f, 0.0f, len)), QVector3D(0.0f, 0.0f, 1.0f));
    }
}
