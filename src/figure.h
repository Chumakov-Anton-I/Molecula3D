#ifndef FIGURE_H
#define FIGURE_H

/*
 * The Figure class
 *
 * An abstract base class of any graphics items of the scene
 */

#include <QMatrix4x4>

class Canvas3D;

class Figure
{
public:
    enum FigureType {
        FigureUndefined,
        FigureSphere,
        FigureCylinder,
        FigureCone
    };
    Figure() {}
    virtual ~Figure() {}

    virtual void setPosition(const QVector3D &pos) = 0;

    virtual void draw(Canvas3D *canvas) = 0;

    virtual bool rayIntersect(const QVector3D &rayOrigin, const QVector3D &ray, double &dist) { return false; }

    void select(bool on = true) { m_selected = on; }

    bool isSelected() const { return m_selected; }

    virtual int type() const { return FigureUndefined; }    // RTTI

    //unsigned int id() const { return m_id; }

protected:
    bool m_selected = false;
    bool m_visible;

    //unsigned int m_id;
};

#endif // FIGURE_H
