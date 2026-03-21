#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QVector3D>

struct Vertex
{
    Vertex() = default;
    Vertex(const QVector3D& p, const QVector3D& n, const QVector2D& uv)
        : position(p), normal(n), uv(uv) {}

    QVector3D position;
    QVector3D normal;
    QVector2D uv;
};

#endif // VERTEX_H
