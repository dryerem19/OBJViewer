#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QString>
#include <QOpenGLShaderProgram>

#include "model.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

signals:
    void onAddModel(Model* model);

public slots:
    void draw               (QOpenGLShaderProgram* shaderProgram);
    void loadObjFromFile    (const QString& filename);
    void addModel           (Model* model);

private:
    QVector<Model*> m_models;
};

#endif // SCENE_H
