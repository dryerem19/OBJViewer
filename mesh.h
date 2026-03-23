#ifndef MESH_H
#define MESH_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "vertex.h"
#include "material.h"

class Mesh : public QObject
{
    Q_OBJECT
public:
    Mesh(QObject *parent = nullptr);

public slots:
    void draw       (QOpenGLShaderProgram* shaderProgram);
    void create     (const QVector<Vertex>& vertices, const QVector<quint32>& indices);
    void setName    (const QString& name);
    void setMaterial(Material* material);

    const QString& getName() const;

private:
    QOpenGLVertexArrayObject    m_vao;
    QOpenGLBuffer               m_vbo;
    QOpenGLBuffer               m_ebo;
    QOpenGLFunctions*           m_func;
    QString                     m_name;
    Material*                   m_material;
};

#endif // MESH_H
