#ifndef SCENE_H
#define SCENE_H

/*
 * The Scene class
 * The Model of the Model--View--Controller architecture
 */

#include <QList>
#include <QMap>
#include <QVector3D>
#include <QObject>

class SceneItem;
class Canvas3D;

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene();
    ~Scene();

    void addItem(SceneItem *item);
    SceneItem* getItemByIndex(unsigned long index);
    const QList<unsigned long>& selectedItems() const { return m_selected; }

    void render(Canvas3D *canvas);

public slots:
    void clearSelection();
    void deleteSelected();
    void selectWithRay(const QVector3D &ray, const QVector3D &origin);

signals:
    void sceneChanged();
    void queryRendering();

private:
    unsigned long getID();
    unsigned long m_lastID;
    QMap<unsigned long, SceneItem *> m_storage;
    QList<unsigned long> m_selected;
};

#endif // SCENE_H
