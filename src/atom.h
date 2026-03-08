#ifndef ATOM_H
#define ATOM_H

#include "sphere.h"
#include "defines.h"
#include "sceneitem.h"

class Atom : public SceneItem   //public Sphere
{
public:
    Atom(const Element &param, const QVector3D &position, Atom *parent = nullptr);
    ~Atom();

    int type() const { return ItemAtom; }

    const Element& elementParams() const { return m_params; }

    QString atomID() const { return m_id; }

    void setColor(const QVector3D &color);

    bool rayIntersect(const QVector3D &rayOrigin, const QVector3D &ray, double &dist);
    void select(bool on = true);

    void draw(Canvas3D *canvas);

private:
    Sphere m_sphere;

    QString m_elementName;
    QString m_id;
    Element m_params;
    Atom *m_parent;
    QList<Atom *> m_children;
};

#endif // ATOM_H
