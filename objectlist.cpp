#include "objectlist.h"
#include "scene.h"

ObjectList::ObjectList(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(1);
}

void ObjectList::setScene(Scene *scene)
{
    if (!scene) {
        return;
    }

    m_scene = scene;
    connect(m_scene, &Scene::onAddModel, this, &ObjectList::onAddModel);
}

void ObjectList::onAddModel(Model *model)
{
    QTreeWidgetItem* modelItem = new QTreeWidgetItem(this);
    modelItem->setText(0, model->getName());

    for (int i = 0; i < model->getCountMeshes(); i++)
    {
        auto mesh       = model->getMesh(i);
        auto meshItem   = new QTreeWidgetItem;
        meshItem->setText(0, mesh->getName());
        modelItem->addChild(meshItem);
    }

    addTopLevelItem(modelItem);
}
