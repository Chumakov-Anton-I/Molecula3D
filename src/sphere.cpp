#include "sphere.h"

#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QVector4D>
#include <QOpenGLBuffer>

Sphere::Sphere(float radius, const QVector3D &position)
    : m_radius(radius), m_position(position)
{
    initializeOpenGLFunctions();

    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    generateSphere(36, 36);

    m_modelMatrix.translate(m_position);

    m_color = QVector3D(1.0f, 0.5f, 0.3f);
    m_selectColor = QVector3D(0.0f, 1.0f, 0.0f);
}

Sphere::~Sphere()
{
    m_vbo->destroy();
    m_ebo->destroy();
    delete m_vbo;
    delete m_ebo;
    qDebug() << "SPHERE: removed";
}

void Sphere::setPosition(const QVector3D &pos)
{
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(pos);
}

void Sphere::draw(QOpenGLShaderProgram *program)
{
    m_vbo->bind();
    m_ebo->bind();
    program->setUniformValue("modelMatr", m_modelMatrix);

    program->enableAttributeArray("vert_pos");
    program->setAttributeBuffer("vert_pos", GL_FLOAT, 0, 3, sizeof(Vertex));

    program->enableAttributeArray("vert_norm");
    program->setAttributeBuffer("vert_norm", GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));

    if (m_selected)
        program->setUniformValue("obj_color", m_selectColor);
    else
        program->setUniformValue("obj_color", m_color);
    program->setUniformValue("light_color", QVector3D(1.0f, 1.0f, 1.0f));
    program->setUniformValue("alpha", 1.0f);  // TODO: introduce a variable for alpha-channel

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, nullptr);

    program->disableAttributeArray("vert_pos");
    program->disableAttributeArray("vert_norm");

    m_vbo->release();
    m_ebo->release();
}

bool Sphere::rayIntersect(const QVector3D &rayOrigin, const QVector3D &ray, double &dist)
{
    QVector3D distance = rayOrigin - m_position;
    double b = QVector3D::dotProduct(ray, distance);
    double c = QVector3D::dotProduct(distance, distance) - m_radius * m_radius;
    float bb_c = b * b - c;
    if (bb_c < 0.0)
        return false;
    if (bb_c > 0.0) {
        double t_a = -b + std::sqrt(bb_c);
        double t_b = -b - std::sqrt(bb_c);
        dist = t_b;

        if (t_a < 0.0) {
            if (t_b < 0.0) return false;
        }
        else if (t_b < 0.0)
            dist = t_b;
        return true;
    }

    if (bb_c == 0.0) {
        double t = -b + std::sqrt(bb_c);
        if (t < 0.0)
            return false;
        dist = t;
        return true;
    }
    return false;
}

void Sphere::generateSphere(int sectors, int stacks)
{
    GLfloat x, y, z, xy, u, v;
    GLfloat du = M_PI / GLfloat(stacks);
    GLfloat dv = M_PI * 2.0f / GLfloat(sectors);

    int cnt_v = (stacks + 1)*(sectors + 1);
    m_indices = 6*sectors*(stacks - 1);

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
            data[idx++] = { m_radius * QVector3D(x, y, z), QVector3D(x, y, z) };
        }
    }

    GLuint *indices = new GLuint[m_indices];
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

    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(data, cnt_v*sizeof(Vertex));
    m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_ebo->create();
    m_ebo->bind();
    m_ebo->allocate(indices, m_indices*sizeof(GLuint));
    m_ebo->setUsagePattern(QOpenGLBuffer::StaticDraw);

    delete[] data;
    delete[] indices;
}
