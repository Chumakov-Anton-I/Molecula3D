#ifndef CAMERA_H
#define CAMERA_H

/*
 * The Camera class
 *
 * A 3D camera
 */

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    explicit Camera();

    void setPosition(const QVector3D &position);
    void setTarget(const QVector3D &target);
    void setPositionAndTarget(const QVector3D &position, const QVector3D &target);

    void rotate(const QVector2D &diff);
    void shift(const QVector2D &diff);
    void zoom(float distance);

    const QMatrix4x4& matrix() const { return m_matrix; }
    const QVector3D& position() const { return m_position; }
    const QVector3D& lightPosition() const { return m_lightPosition; }

private:
    void lookAt();
    void recalculate();

    QVector3D m_position;
    QVector3D m_target;
    QVector3D m_front;
    QVector3D m_up;
    QVector3D m_left;
    float m_distance;
    QVector3D m_lightPosition;

    QMatrix4x4 m_matrix;    // view matrix
    QMatrix4x4 m_rotation;
};

#endif // CAMERA_H
