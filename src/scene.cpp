#include "scene.h"
#include "sceneitem.h"

unsigned long INVALID_KEY = 0ul;

Scene::Scene()
    : QObject{nullptr}
{
    m_lastID = INVALID_KEY + 1ul;
}

Scene::~Scene()
{
    for (auto it = m_storage.begin(); it != m_storage.end(); ++it)
        delete *it;
    m_storage.clear();
}

void Scene::addItem(SceneItem *item)
{
    clearSelection();
    m_storage.insert(getID(), item);
}

SceneItem *Scene::getItemByIndex(unsigned long index)
{
    return m_storage.value(index, nullptr);
}

void Scene::render(Canvas3D *canvas)
{
    for (auto item = m_storage.begin(); item != m_storage.end(); ++item)
        item.value()->draw(canvas);
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
    emit queryRendering();
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
    emit queryRendering();
}

void Scene::selectWithRay(const QVector3D &ray, const QVector3D &origin)
{
    clearSelection();
    double d = 1000.0;  // enough
    unsigned long s_key = INVALID_KEY;    // current selected object
    bool select = false;
    for (auto item = m_storage.begin(); item != m_storage.end(); ++item) {
        double t_dist = 0.0;
        if ((*item)->rayIntersect(origin, ray, t_dist)) {
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
        emit queryRendering();
    }
}

unsigned long Scene::getID()
{
    return ++m_lastID;
}
