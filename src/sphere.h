#ifndef SPHERE_H
#define SPHERE_H

#include "figure.h"

class Sphere : public Figure
{
public:
    explicit Sphere(float radius, const QVector3D &position);
    ~Sphere();

    void setPosition(const QVector3D &pos);
    void select(bool status = true) { m_selected = status; }
    void draw(Scene *scene);

    bool rayIntersect(const QVector3D &rayOrigin, const QVector3D &ray, double &dist);

private:
    void updateMarix();

    QMatrix4x4 m_modelMatrix;
    GLfloat m_radius;
    QVector3D m_position;

    QVector3D m_color;
    QVector3D m_selectColor;
};

#endif // SPHERE_H
