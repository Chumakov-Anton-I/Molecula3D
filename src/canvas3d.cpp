#include "canvas3d.h"
#include "figure.h"
#include "scene.h"

#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

struct Vertex
{
    QVector3D p;
    QVector3D n;
};

Canvas3D::Canvas3D(QWidget *parent)
    : QOpenGLWidget{parent}, m_scene{nullptr}
{
    setMinimumSize(600, 300);
    setMouseTracking(true);

    m_FOV = 45.0f;

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
    m_VBOcylinder = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_EBOcylinder = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
}

Canvas3D::~Canvas3D()
{
    makeCurrent();

    m_VBOsphere->destroy();
    m_EBOsphere->destroy();
    delete m_VBOsphere;
    delete m_EBOsphere;

    m_VBOcylinder->destroy();
    m_EBOcylinder->destroy();
    delete m_VBOcylinder;
    delete m_EBOcylinder;

    doneCurrent();
    qDebug() << "Canvas3D: removed";
}

void Canvas3D::setScene(Scene *scene)
{
    m_scene = scene;
}

void Canvas3D::drawSphere(const QMatrix4x4 &matrix, const QVector3D &color)
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

void Canvas3D::drawCone(const QMatrix4x4 &matrix, const QVector3D &color)
{

}

void Canvas3D::drawCylinder(const QMatrix4x4 &matrix, const QVector3D &color)
{
    m_program.bind();
    m_VBOcylinder->bind();
    m_EBOcylinder->bind();

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

    glDrawElements(GL_TRIANGLES, m_cylinderIndices, GL_UNSIGNED_INT, nullptr);

    m_program.disableAttributeArray("vert_pos");
    m_program.disableAttributeArray("vert_norm");

    m_VBOcylinder->release();
    m_EBOcylinder->release();
    m_program.release();
}

void Canvas3D::drawDot(const QVector3D &position, const QVector3D &color)
{
    m_progWframe.bind();
    m_progWframe.setUniformValue("projMatr", m_projMatr);
    m_progWframe.setUniformValue("viewMatr", m_viewMatr);
    m_progWframe.setUniformValue("obj_color", color);

    m_progWframe.setAttributeValue("vert_pos", position);
    glPointSize(4.0f);
    glDrawArrays(GL_POINTS, 0, 1);
    glPointSize(1.0f);
    m_progWframe.release();
}

void Canvas3D::drawLineSegment(const QVector3D &v1, const QVector3D &v2, const QVector3D &color)
{
    m_progWframe.bind();
    m_progWframe.setUniformValue("projMatr", m_projMatr);
    m_progWframe.setUniformValue("viewMatr", m_viewMatr);
    m_progWframe.setUniformValue("obj_color", color);

    const QVector3D verts[] = { v1, v2 };
    m_progWframe.setAttributeArray("vert_pos", GL_FLOAT, verts, 3);
    m_progWframe.enableAttributeArray("ver_pos");
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 1);
    glLineWidth(1.0f);
    m_progWframe.disableAttributeArray("vert_pos");
    m_progWframe.release();
}

void Canvas3D::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // init shaders
    // general shader
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/vshader.glsl");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl");
    if (!m_program.link()) {
        qWarning() << "Shader program linking failed:" << m_program.log();
        return;
    }
    /*if (!m_program.bind()) {
        qWarning() << "Shader program binding failed:" << m_program.log();
        return;
    }*/
    // wireframe shader
    m_progWframe.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/vshader_wframe.glsl");
    m_progWframe.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader_wframe.glsl");
    if (!m_progWframe.link()) {
        qWarning() << "Shader program linking failed:" << m_progWframe.log();
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_viewMatr = m_camera.matrix();

    initSphere();
    initCone();
    initCylinder();
}

void Canvas3D::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    float aspect = float(width) / float(height ? height : 1);
    m_projMatr.setToIdentity();
    m_projMatr.perspective(m_FOV, aspect, 0.1f, 100.0f);
}

void Canvas3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!m_scene)
        return;
    m_scene->render(this);
}

void Canvas3D::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        emit querySelection(getRay(event->position()), m_camera.position());
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

void Canvas3D::mouseMoveEvent(QMouseEvent *event)
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

void Canvas3D::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        m_modeRotateView = false;
    else if (event->button() == Qt::MiddleButton)
        m_modePanView = false;
}

void Canvas3D::wheelEvent(QWheelEvent *event)
{
    float d = event->angleDelta().y() / 120.f;
    zoomScene(d);
    m_viewMatr = m_camera.matrix();
    update();
}

void Canvas3D::initSphere()
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

void Canvas3D::initCone()
{

}

void Canvas3D::initCylinder()
{
    int sectors = 36;
    GLfloat x, y, v;
    GLfloat dv = M_PI * 2.0f / GLfloat(sectors);

    int cnt_v = 4*sectors + 6;
    m_cylinderIndices = 12 * sectors; // cnt_v - 2;
    Vertex *data = new Vertex[cnt_v];
    GLfloat *XX = new GLfloat[sectors + 1];
    GLfloat *YY = new GLfloat[sectors + 1];
    int idx = 0;
    data[idx++] = { QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f) };    // bottom pole - 0st vertex
    for (int i = 0; i <= sectors; ++i) {    // bottom plane
        v = dv * i;
        XX[i] = x = std::cos(v);
        YY[i] = y = std::sin(v);
        data[idx++] = { QVector3D(x, y, 0.0f), QVector3D(0.0f, 0.0f, -1.0f) };
    }
    for (int i = 0; i <= sectors; ++i)  // bottom loop of cylinder
        data[idx++] = { QVector3D(XX[i], YY[i], 0.0f), QVector3D(XX[i], YY[i], 0.0f) };
    for (int i = 0; i <= sectors; ++i)  // top loop of cylinder
        data[idx++] = { QVector3D(XX[i], YY[i], 1.0f), QVector3D(XX[i], YY[i], 0.0f) };
    for (int i = 0; i <= sectors; ++i)  // top plane circle
        data[idx++] = { QVector3D(XX[i], YY[i], 1.0f), QVector3D(0.0f, 0.0f, +1.0f) };
    data[idx++] = { QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f, +1.0f) }; // top pole

    GLuint *indices = new GLuint[m_cylinderIndices];
    idx = 0;
    GLuint k1 = 1, k2;
    // bottom fan
    for (int i = 0; i < sectors; ++i, ++k1) {
        //k1 = i + 1;
        indices[idx++] = 0;
        indices[idx++] = k1 + 1;
        indices[idx++] = k1;
    }
    k1++;
    // cylinder surface - triangle strip
    for (int i = 0; i < sectors; ++i, ++k1) {
        k2 = k1 + sectors + 1;
        indices[idx++] = k1;
        indices[idx++] = k1 + 1;
        indices[idx++] = k2;
        indices[idx++] = k1 + 1;
        indices[idx++] = k2 + 1;
        indices[idx++] = k2;
    }
    k1++;
    // top fan
    for (int i = 0; i < sectors; ++i, ++k1) {
        indices[idx++] = cnt_v - 1; // very last vertex
        indices[idx++] = k1;
        indices[idx++] = k1 + 1;
    }

    m_VBOcylinder->create();
    m_VBOcylinder->bind();
    m_VBOcylinder->allocate(data, cnt_v*sizeof(Vertex));
    m_VBOcylinder->setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_EBOcylinder->create();
    m_EBOcylinder->bind();
    m_EBOcylinder->allocate(indices, m_cylinderIndices*sizeof(GLuint));

    delete[] data;
    delete[] indices;
    delete[] XX;
    delete[] YY;
}

void Canvas3D::rotateScene(const QVector2D &delta)
{
    m_camera.rotate(delta);
    m_viewMatr = m_camera.matrix();
    update();
}

void Canvas3D::panScene(const QVector2D &delta)
{
    m_camera.shift(delta);
    m_viewMatr = m_camera.matrix();
    update();
}

void Canvas3D::zoomScene(float distance)
{
    m_camera.zoom(distance);
    m_viewMatr = m_camera.matrix();
    update();
}

QVector3D Canvas3D::getRay(const QPointF &p)
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
