#include "mesh.h"

Mesh::Mesh(QObject *parent)
    : QObject(parent)
    , m_vbo(QOpenGLBuffer::VertexBuffer)
    , m_ebo(QOpenGLBuffer::IndexBuffer)
{
    auto ctx    = QOpenGLContext::currentContext();
    m_func      = ctx->functions();;
}

void Mesh::draw()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_func->glDrawElements(GL_TRIANGLES, m_ebo.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::create(const QVector<Vertex> &vertices, const QVector<quint32> &indices)
{
    // Заполняем буфер
    if (m_vbo.isCreated()) m_vbo.destroy();
    Q_ASSERT(m_vbo.create());
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(vertices.constData(), vertices.size() * sizeof(Vertex));

    // Создаём объект массива вершин
    if (m_vao.isCreated()) m_vao.destroy();
    Q_ASSERT(m_vao.create());
    m_vao.bind();

    // Активируем массивы вершинных атрибутов
    m_func->glEnableVertexAttribArray(0); // Координаты вершин
    m_func->glEnableVertexAttribArray(1); // Координаты нормали
    m_func->glEnableVertexAttribArray(2); // Координаты UV

    // Вычисляем смещения атрибутов
    const GLsizei stride                  = sizeof(Vertex);
    const GLvoid* positionAttributeOffset = 0;
    const GLvoid* normalAttributeOffset   = reinterpret_cast<GLvoid*>(sizeof(QVector3D));
    const GLvoid* uvAttributeOffset       = reinterpret_cast<GLvoid*>(sizeof(QVector3D) * 2);

    // Закрепляем индексы массива вершинных атрибутов за буфером
    m_func->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, positionAttributeOffset);
    m_func->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, normalAttributeOffset);
    m_func->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, uvAttributeOffset);

    // Заполняем буфер индексов
    if (m_ebo.isCreated()) m_ebo.destroy();;
    Q_ASSERT(m_ebo.create());
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_ebo.bind();
    m_ebo.allocate(indices.constData(), indices.size() * sizeof(quint32));

    // Освобождаем ресурсы
    m_vao.release();
    m_ebo.release();
    m_vbo.release();
}

void Mesh::setMaterial(Material *material)
{
    m_material = material;
}
