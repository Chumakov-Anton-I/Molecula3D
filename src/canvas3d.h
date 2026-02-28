#ifndef CANVAS3D_H
#define CANVAS3D_H

/*
 * The Canvas3D class
 *
 * A 3D viewport which implements the View component of the MVC-architecture
 */

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QList>
#include <QMap>

#include "camera.h"

class QOpenGLBuffer;
class Figure;
class Scene;

class Canvas3D : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Canvas3D(QWidget *parent = nullptr);
    ~Canvas3D();

    void setScene(Scene *scene);

    QSize sizeHint() const override { return QSize(640, 360); }

    void drawSphere(const QMatrix4x4 &matrix, const QVector3D &color);

signals:
    void querySelection(const QVector3D &ray, const QVector3D &origin);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void initSphere();

    void rotateScene(const QVector2D &delta);
    void panScene(const QVector2D &delta);
    void zoomScene(float distance);

    QVector3D getRay(const QPointF &p);

    Scene *m_scene;

    QPointF m_mousePosition;
    bool m_modeRotateView = false;
    bool m_modePanView = false;

    QOpenGLShaderProgram m_program;
    QMatrix4x4 m_viewMatr;
    QMatrix4x4 m_projMatr;

    float m_FOV;
    Camera m_camera;

    QOpenGLBuffer *m_VBOsphere;
    QOpenGLBuffer *m_EBOsphere;
    QOpenGLBuffer *m_VBOcone;
    QOpenGLBuffer *m_EBOcone;
    QOpenGLBuffer *m_VBOcylinder;
    QOpenGLBuffer *m_EBOcylinder;

    GLuint m_sphereIndices;
};

#endif // CANVAS3D_H
