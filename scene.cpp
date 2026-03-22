#include "scene.h"


Scene::Scene(QObject *parent)
    : QObject{parent}
{

}

void Scene::draw(QOpenGLShaderProgram *shaderProgram)
{
    for (int i = 0; i < m_models.size(); i++) {
        m_models[i]->draw(shaderProgram);
    }
}

void Scene::loadObjFromFile(const QString &filename)
{
    Model* model = new Model;
    if (model->loadObjFromFile(filename)) {
        addModel(model);
    } else {
        delete model;
    }
}

void Scene::addModel(Model *model)
{
    if (!model) {
        return;
    }

    for (int i = 0; i < m_models.size(); i++) {
        if (m_models[i] == model) {
            return;
        }
    }

    model->setParent(this);
    m_models.append(model);
    emit onAddModel(model);
}
