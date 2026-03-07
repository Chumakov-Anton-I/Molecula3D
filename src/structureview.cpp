#include "structureview.h"

#include <QTreeView>
#include <QVBoxLayout>

StructureView::StructureView(QWidget *parent)
    : QWidget{parent}
{
    setMinimumWidth(256);

    auto *topLayout = new QVBoxLayout;
    setLayout(topLayout);

    m_treeView = new QTreeView;
    topLayout->addWidget(m_treeView);
}
