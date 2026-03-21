#ifndef MESH_H
#define MESH_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

#include "vertex.h"
#include "material.h"

class Mesh : public QObject
{
    Q_OBJECT
public:
    Mesh(QObject *parent = nullptr);

public slots:
    void draw       ();
    void create     (const QVector<Vertex>& vertices, const QVector<quint32>& indices);
    void setMaterial(Material* material);

private:
    QOpenGLVertexArrayObject    m_vao;
    QOpenGLBuffer               m_vbo;
    QOpenGLBuffer               m_ebo;
    QOpenGLFunctions*           m_func;
    Material*                   m_material;
};

#endif // MESH_H
