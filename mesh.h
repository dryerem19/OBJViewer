#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

struct Vertex
{
    Vertex() = default;
    Vertex(const QVector3D& p, const QVector2D& t, const QVector3D& n = QVector3D(0, 0, 0))
        : position(p), uv(t), normal(n) {}

    QVector3D position;
    QVector3D normal;
    QVector2D uv;
};

class Mesh
{
public:
    Mesh();

public slots:
    void draw();
    bool load(const QString& filename);

private:
    QOpenGLVertexArrayObject    m_vao;
    QOpenGLBuffer               m_vbo;
    QOpenGLBuffer               m_ebo;
    QOpenGLFunctions*           m_func;
};

#endif // MESH_H
