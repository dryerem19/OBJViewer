#include "mesh.h"

Mesh::Mesh(QObject *parent)
    : QObject(parent)
    , m_vbo(QOpenGLBuffer::VertexBuffer)
    , m_ebo(QOpenGLBuffer::IndexBuffer)
{
    auto ctx    = QOpenGLContext::currentContext();
    m_func      = ctx->functions();
}

void Mesh::draw(QOpenGLShaderProgram* shaderProgram)
{
    auto diffuseMap = m_material->getDiffuseMap();
    if (diffuseMap)
    {
        diffuseMap->bind();
        shaderProgram->setUniformValue("u_useDiffuseMap", true);
    }

    shaderProgram->setUniformValue("u_material.shiness",        m_material->getShiness());
    shaderProgram->setUniformValue("u_material.diffuseColor",   m_material->getDiffuseColor());
    shaderProgram->setUniformValue("u_material.ambienceColor",  m_material->getAmbienceColor());
    shaderProgram->setUniformValue("u_material.specularColor",  m_material->getSpecularColor());

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_func->glDrawElements(GL_TRIANGLES, m_ebo.size(), GL_UNSIGNED_INT, 0);

    if (diffuseMap)
    {
        diffuseMap->release();
    }
}

void Mesh::create(const QVector<Vertex> &vertices, const QVector<quint32> &indices)
{
    Q_ASSERT(vertices.size() > 0);
    Q_ASSERT(indices.size() > 0);

    // Заполняем буфер
    if (m_vbo.isCreated()) m_vbo.destroy();
    Q_ASSERT(m_vbo.create() == true);
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(vertices.constData(), vertices.size() * sizeof(Vertex));

    // Создаём объект массива вершин
    if (m_vao.isCreated()) m_vao.destroy();
    Q_ASSERT(m_vao.create() == true);
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
    Q_ASSERT(m_ebo.create() == true);
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_ebo.bind();
    m_ebo.allocate(indices.constData(), indices.size() * sizeof(quint32));

    // Освобождаем ресурсы
    m_vao.release();
    m_ebo.release();
    m_vbo.release();
}

void Mesh::setName(const QString &name)
{
    m_name = name;
}

void Mesh::setMaterial(Material *material)
{
    if (material)
    {
        m_material = material;
    }
}

const QString &Mesh::getName() const
{
    return m_name;
}
