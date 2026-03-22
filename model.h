#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QOpenGLShaderProgram>

#include "mesh.h"
#include "materiallibrary.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    ~Model();

public slots:
    void    draw            (QOpenGLShaderProgram* shaderProgram);
    bool    loadObjFromFile (const QString& filename);
    bool    addMesh         (Mesh* mesh);
    void    setName(const QString& name);

    Mesh*           getMesh         (quint32 index) const;
    const QString&  getName         () const;
    quint32         getCountMeshes  () const;

private:
    MaterialLibrary m_mtl;
    QString         m_name;
    QVector<Mesh*>  m_meshes;
};

#endif // MODEL_H
