#include "scene.h"
#include "figure.h"
#include "sphere.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

Scene::Scene(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(600, 300);
    setMouseTracking(true);

    m_lastID = 0ul;

    // enable antialiasing
    QSurfaceFormat format;
    format.setBlueBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    setFormat(format);

    m_camera.setPositionAndTarget(QVector3D(0.0f, 4.0f, 10.0f), QVector3D());
}

Scene::~Scene()
{
    makeCurrent();
    for (auto it = m_storage.begin(); it != m_storage.end(); ++it)
        delete *it;
    m_storage.clear();
    doneCurrent();
    qDebug() << "SCENE: removed";
}

void Scene::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // init shaders
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/vshader.glsl");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl");
    if (!m_program.link()) {
        qWarning() << "Shader program linking failed:" << m_program.log();
        return;
    }
    if (!m_program.bind()) {
        qWarning() << "Shader program binding failed:" << m_program.log();
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_viewMatr = m_camera.matrix();
}

void Scene::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    float aspect = float(width) / float(height ? height : 1);
    m_projMatr.setToIdentity();
    m_projMatr.perspective(45.0f, aspect, 0.1f, 100.0f);
}

void Scene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program.bind();
    m_program.setUniformValue("projMatr", m_projMatr);
    m_program.setUniformValue("viewMatr", m_viewMatr);
    m_program.setUniformValue("light_pos", m_camera.position());
    for (auto item = m_storage.begin(); item != m_storage.end(); ++item)
        (*item)->draw(&m_program);
    m_program.release();
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        selectObject(event->position());
        break;
    case Qt::RightButton:
        m_modeRotateView = true;
        break;
    case Qt::MiddleButton:
        m_modePanView = true;
        break;
    default:
        break;
    }
}

void Scene::mouseMoveEvent(QMouseEvent *event)
{
    if (m_modeRotateView) {
        auto delta = QVector2D(event->position() - m_mousePosition);
        rotateScene(delta);
    }
    else if (m_modePanView) {
        auto delta = QVector2D(event->position() - m_mousePosition);
        panScene(delta);
    }
    m_mousePosition = event->position();
}

void Scene::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        m_modeRotateView = false;
    else if (event->button() == Qt::MiddleButton)
        m_modePanView = false;
}

void Scene::wheelEvent(QWheelEvent *event)
{
    float d = event->angleDelta().y() / 120.f;
    zoomScene(d);
    m_viewMatr = m_camera.matrix();
    update();
}

void Scene::addAtom(const QVector3D &position)
{
    clearSelection();
    m_storage.insert(getID(), new Sphere(2.0f, position));
}

void Scene::clearSelection()
{
    if (m_selected.isEmpty())
        return;
    for (auto item = m_selected.begin(); item != m_selected.end(); ++item) {
        auto obj = m_storage.value(*item);
            obj->select(false);
    }
    m_selected.clear();
    update();
}

void Scene::deleteSelected()
{
    if (m_selected.isEmpty())
        return;
    for (auto it = m_selected.begin(); it != m_selected.end(); ++it) {
        auto obj = m_storage.value(*it);
        delete obj;
        m_storage.remove(*it);
    }
    m_selected.clear();
    update();
}

void Scene::rotateScene(const QVector2D &delta)
{
    m_camera.rotate(delta);
    m_viewMatr = m_camera.matrix();
    update();
}

void Scene::panScene(const QVector2D &delta)
{
    m_camera.shift(delta);
    m_viewMatr = m_camera.matrix();
    update();
}

void Scene::zoomScene(float distance)
{
    m_camera.zoom(distance);
    m_viewMatr = m_camera.matrix();
    update();
}

void Scene::selectObject(const QPointF &p)
{
    clearSelection();
    auto ray = getRay(p);
    double d = 1000.0;  // enough
    unsigned long s_key = 0ul;    // current selected object
    bool select = false;
    for (auto item = m_storage.begin(); item != m_storage.end(); ++item) {
        double t_dist = 0.0;
        if ((*item)->rayIntersect(m_camera.position(), ray, t_dist)) {
            if (t_dist < d) {
                d = t_dist;
                s_key = item.key();
            }
            select = true;
        }
    }
    if (select) {
        m_selected.append(s_key);
        m_storage[s_key]->select(true);
        update();
    }
}

QVector3D Scene::getRay(const QPointF &p)
{
    float x = (2.0f * p.x()) / width() - 1.0f;
    float y = 1.0f - (2.0f * p.y()) / height();
    QVector4D ray_clip(x, y, -1.0f, 0.0f);
    QVector4D ray_eye = m_projMatr.inverted()*ray_clip;
    ray_eye.setZ(-1.0f);
    ray_eye.setW( 0.0f);
    QVector3D ray_w(m_viewMatr.inverted()*ray_eye);
    return ray_w.normalized();
}

unsigned long Scene::getID()
{
    return ++m_lastID;
}
