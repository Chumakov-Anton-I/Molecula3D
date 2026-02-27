#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QList>
#include <QMap>

#include "camera.h"

class QGLBuffer;
class Figure;

class Scene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Scene(QWidget *parent = nullptr);
    ~Scene();

    QSize sizeHint() const override { return QSize(640, 360); }

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void sceneChanged();

public slots:
    void addAtom(const QVector3D &position);
    void clearSelection();
    void deleteSelected();

private:
    void rotateScene(const QVector2D &delta);
    void panScene(const QVector2D &delta);
    void zoomScene(float distance);

    void selectObject(const QPointF &p);

    QVector3D getRay(const QPointF &p);
    unsigned long getID();
    unsigned long m_lastID;

    QPointF m_mousePosition;
    bool m_modeRotateView = false;
    bool m_modePanView = false;

    QOpenGLShaderProgram m_program;
    QMatrix4x4 m_viewMatr;
    QMatrix4x4 m_projMatr;

    Camera m_camera;
    QMap<unsigned long, Figure *> m_storage;
    //QList<Figure *> m_selected;
    QList<unsigned long> m_selected;
};

#endif // SCENE_H
