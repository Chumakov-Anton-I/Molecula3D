#include "camera.h"

Camera::Camera() {}

void Camera::setPosition(const QVector3D &position)
{
    m_position = position;
    lookAt();
}

void Camera::setTarget(const QVector3D &target)
{
    m_target = target;
    lookAt();
}

void Camera::setPositionAndTarget(const QVector3D &position, const QVector3D &target)
{
    m_position = position;
    m_target = target;
    lookAt();
}

void Camera::rotate(const QVector2D &diff)
{
    QVector3D axis = diff.y() * m_left + diff.x() * m_up;   // NB! X and Y are flipped
    m_rotation.rotate(0.2f * diff.length(), axis);
    recalculate();
}

void Camera::shift(const QVector2D &diff)
{
    QVector3D delta = diff.x() * m_left - diff.y() *m_up;
    m_target -= 0.2f*delta.normalized();
    recalculate();
}

void Camera::zoom(float distance)
{
    if ((m_distance + distance) < 0.1f)
        return;
    m_distance += distance;
    recalculate();
}

void Camera::lookAt()
{
    m_front = m_position - m_target;
    m_distance = m_front.length();
    m_front.normalize();
    m_left = QVector3D::normal(QVector3D(0.0f, 1.0f, 0.0f), m_front);
    m_up   = QVector3D::normal(m_front, m_left);
    m_rotation.setToIdentity();
    m_rotation.setRow(0, QVector4D(m_left,  0.f));
    m_rotation.setRow(1, QVector4D(m_up,    0.f));
    m_rotation.setRow(2, QVector4D(m_front, 0.f));
    QMatrix4x4 trans;
    trans.translate(-m_position);
    m_matrix = m_rotation * trans;
    m_lightPosition = m_position - 50.0f*m_left + 45.0f*m_up;
}

void Camera::recalculate()
{
    m_left =  QVector3D(m_rotation.row(0));
    m_up =    QVector3D(m_rotation.row(1));
    m_front = QVector3D(m_rotation.row(2));
    m_position = m_target + m_distance * m_front;
    QMatrix4x4 trans;
    trans.translate(-m_position);
    m_matrix = m_rotation * trans;
    m_lightPosition = m_position - 50.0f*m_left + 45.0f*m_up;
}
