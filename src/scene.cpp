#include "scene.h"
#include "figure.h"
#include "sphere.h"

#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

struct Vertex
{
    QVector3D p;
    QVector3D n;
};

Scene::Scene(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(600, 300);
    setMouseTracking(true);

    m_FOV = 45.0f;
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

    m_VBOsphere = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_EBOsphere = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    //m_VBOcone   = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //m_EBOcone   = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
}

Scene::~Scene()
{
    makeCurrent();
    for (auto it = m_storage.begin(); it != m_storage.end(); ++it)
        delete *it;
    m_storage.clear();

    m_VBOsphere->destroy();
    m_EBOsphere->destroy();
    delete m_VBOsphere;
    delete m_EBOsphere;

    doneCurrent();
    qDebug() << "SCENE: removed";
}

void Scene::drawSphere(const QMatrix4x4 &matrix, const QVector3D &color)
{
    m_program.bind();
    m_VBOsphere->bind();
    m_EBOsphere->bind();

    m_program.setUniformValue("projMatr", m_projMatr);
    m_program.setUniformValue("viewMatr", m_viewMatr);
    m_program.setUniformValue("modelMatr", matrix);

    m_program.setUniformValue("light_pos", m_camera.position());
    m_program.setUniformValue("obj_color", color);

    m_program.enableAttributeArray("vert_pos");
    m_program.setAttributeBuffer("vert_pos", GL_FLOAT, 0, 3, sizeof(Vertex));

    m_program.enableAttributeArray("vert_norm");
    m_program.setAttributeBuffer("vert_norm", GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));

    m_program.setUniformValue("light_color", QVector3D(1.0f, 1.0f, 1.0f));
    m_program.setUniformValue("alpha", 1.0f);

    glDrawElements(GL_TRIANGLES, m_sphereIndices, GL_UNSIGNED_INT, nullptr);

    m_program.disableAttributeArray("vert_pos");
    m_program.disableAttributeArray("vert_norm");

    m_VBOsphere->release();
    m_EBOsphere->release();
    m_program.release();
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

    initSphere();
}

void Scene::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    float aspect = float(width) / float(height ? height : 1);
    m_projMatr.setToIdentity();
    m_projMatr.perspective(m_FOV, aspect, 0.1f, 100.0f);
}

void Scene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto item = m_storage.begin(); item != m_storage.end(); ++item)
        item.value()->draw(this);
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

void Scene::addAtom(const QVector3D &position, double radius)
{
    clearSelection();
    m_storage.insert(getID(), new Sphere(radius, position));
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

void Scene::initSphere()
{
    int sectors = 36;
    int stacks = 36;
    GLfloat x, y, z, xy, u, v;
    GLfloat du = M_PI / GLfloat(stacks);
    GLfloat dv = M_PI * 2.0f / GLfloat(sectors);

    int cnt_v = (stacks + 1)*(sectors + 1);
    m_sphereIndices = 6*sectors*(stacks - 1);

    Vertex *data = new Vertex[cnt_v];
    int idx = 0;
    for (int i = 0; i <= stacks; ++i) {
        u = M_PI / 2.0f - du * i;
        xy = std::cos(u);
        z  = std::sin(u);
        for (int j = 0; j <= sectors; ++j) {
            v = dv * j;
            x = xy * std::cos(v);
            y = xy * std::sin(v);
            data[idx++] = { QVector3D(x, y, z), QVector3D(x, y, z) };
        }
    }

    GLuint *indices = new GLuint[m_sphereIndices];
    GLuint k1, k2;
    idx = 0;
    for (int i = 0; i < stacks; ++i) {
        k1 = (sectors + 1) * i;
        k2 = k1 + stacks + 1;
        for (int j = 0; j < sectors; ++j) {
            indices[idx++] = k1;
            indices[idx++] = k2;
            if (i == 0)
                indices[idx++] = k2 + 1;
            else {
                indices[idx++] = k1 + 1;
                if (i != (stacks - 1)) {
                    indices[idx++] = k1 + 1;
                    indices[idx++] = k2;
                    indices[idx++] = k2 + 1;
                }
            }
            k1++;
            k2++;
        }
    }

    m_VBOsphere->create();
    m_VBOsphere->bind();
    m_VBOsphere->allocate(data, cnt_v*sizeof(Vertex));
    m_VBOsphere->setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_EBOsphere->create();
    m_EBOsphere->bind();
    m_EBOsphere->allocate(indices, m_sphereIndices*sizeof(GLuint));
    m_EBOsphere->setUsagePattern(QOpenGLBuffer::StaticDraw);

    delete[] data;
    delete[] indices;
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
