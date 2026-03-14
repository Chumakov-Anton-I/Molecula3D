#ifndef ATOM_H
#define ATOM_H

#include "sphere.h"
#include "defines.h"
#include "sceneitem.h"

class Atom : public SceneItem
{
public:
    Atom(const Element &param, const QVector3D &position, Atom *parent = nullptr);
    ~Atom();

    int type() const { return ItemAtom; }

    const Element& elementParams() const { return m_params; }

    QString atomID() const { return m_id; }

    void setColor(const QVector3D &color);

    QVector3D position() const { return m_position; }

    bool rayIntersect(const QVector3D &rayOrigin, const QVector3D &ray, double &dist);
    void select(bool on = true);

    void draw(Canvas3D *canvas);

    int valence() const { return m_valence; }
    void setValence(int valence);

private:
    Sphere m_sphere;
    int m_valence;

    QString m_elementName;
    QString m_id;
    Element m_params;
    Atom *m_parent;
    QList<Atom *> m_children;

    bool m_selected = false;
};

#endif // ATOM_H
