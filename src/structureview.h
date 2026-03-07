#ifndef STRUCTUREVIEW_H
#define STRUCTUREVIEW_H

#include <QWidget>

class QTreeView;

class StructureView : public QWidget
{
    Q_OBJECT
public:
    explicit StructureView(QWidget *parent = nullptr);

signals:

private:
    QTreeView *m_treeView;
};

#endif // STRUCTUREVIEW_H
