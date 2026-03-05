#include "sphere.h"
#include "canvas3d.h"

#include <QVector3D>

// TODO: replace overloaded ctors to default value arguments
Sphere::Sphere(const QVector3D &position, float radius, const QVector3D &color)
    : m_radius(radius), m_position(position), m_color(color)
{
    updateMarix();
    m_selectColor = QVector3D(0.0f, 1.0f, 0.0f);
}

Sphere::Sphere(const QVector3D &position, float radius)
    : m_radius(radius), m_position(position), m_color(QVector3D(1.0f, 0.5f, 0.3f))
{
    updateMarix();
    m_selectColor = QVector3D(0.0f, 1.0f, 0.0f);
}

Sphere::Sphere(const QVector3D &position)
    : m_radius(1.0f), m_position(position), m_color(QVector3D(1.0f, 0.5f, 0.3f))
{
    updateMarix();
    m_selectColor = QVector3D(0.0f, 1.0f, 0.0f);
}

Sphere::~Sphere()
{
    qDebug() << "SPHERE: removed";
}

void Sphere::setRadius(float radius)
{
    m_radius = radius;
    updateMarix();
}

void Sphere::setPosition(const QVector3D &pos)
{
    m_position = pos;
    updateMarix();
}

void Sphere::setColor(const QVector3D &color)
{
    m_color = color;
}

void Sphere::draw(Canvas3D *canvas)
{
    if (m_selected)
        canvas->drawSphere(m_modelMatrix, m_selectColor);
    else
        canvas->drawSphere(m_modelMatrix, m_color);
}

bool Sphere::rayIntersect(const QVector3D &rayOrigin, const QVector3D &ray, double &dist)
{
    QVector3D distance = rayOrigin - m_position;
    double b = QVector3D::dotProduct(ray, distance);
    double c = QVector3D::dotProduct(distance, distance) - m_radius * m_radius;
    float bb_c = b * b - c;
    if (bb_c < 0.0)
        return false;
    if (bb_c > 0.0) {
        double t_a = -b + std::sqrt(bb_c);
        double t_b = -b - std::sqrt(bb_c);
        dist = t_b;

        if (t_a < 0.0) {
            if (t_b < 0.0) return false;
        }
        else if (t_b < 0.0)
            dist = t_b;
        return true;
    }

    if (bb_c == 0.0) {
        double t = -b + std::sqrt(bb_c);
        if (t < 0.0)
            return false;
        dist = t;
        return true;
    }
    return false;
}

void Sphere::updateMarix()
{
    QMatrix4x4 t;
    t.translate(m_position);
    m_modelMatrix.setToIdentity();
    m_modelMatrix.scale(m_radius);
    m_modelMatrix = t * m_modelMatrix;
}
