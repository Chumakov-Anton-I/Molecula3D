#ifndef SCENEITEM_H
#define SCENEITEM_H

/*
 * The SceneItem class
 * An abstract base class of all scene object
 */

// TODO: probably, this class has to subclass QAbstractItem

#include <QVector3D>
#include <QMatrix4x4>

class Canvas3D;

class SceneItem
{
public:
    enum ItemType {
        ItemUndefined,
        ItemAtom
    };
    SceneItem(const QVector3D &position) : m_position{position} {}
    virtual ~SceneItem() {}

    virtual int type() const { return ItemUndefined; }

    virtual void draw(Canvas3D *canvas) = 0;

    virtual bool rayIntersect(const QVector3D &rayOrigin, const QVector3D &ray, double &dist) = 0;
    virtual void select(bool on) = 0;

protected:
    QVector3D m_position;
    QMatrix4x4 m_matrix;
};

#endif // SCENEITEM_H
