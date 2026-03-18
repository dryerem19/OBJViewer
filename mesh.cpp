#include "mesh.h"

#include <QFile>
#include <QTextStream>

Mesh::Mesh()
    : m_vbo(QOpenGLBuffer::VertexBuffer)
    , m_ebo(QOpenGLBuffer::IndexBuffer)
{
    auto ctx = QOpenGLContext::currentContext();
    m_func = ctx->functions();;
}

void Mesh::draw()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    // glDrawArrays(GL_TRIANGLES, 0, 24);
    m_func->glDrawElements(GL_TRIANGLES, m_ebo.size(), GL_UNSIGNED_INT, 0);
}

bool Mesh::load(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }


    QVector<QVector3D> vCoords;
    QVector<QVector3D> nCoords;
    QVector<QVector2D> tCoords;

    QVector<Vertex>         m_vertices;
    QVector<quint32>   m_indices;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString input = in.readLine();
        if (input.isEmpty() || input[0] == '#') {
            continue;
        }

        QStringList tokens = input.split(" ");
        if (tokens.empty()) {
            continue;
        }

        std::string key = tokens[0].toStdString();
        switch (key[0])
        {
            case 'v':
            {
                switch (key[1])
                {
                    case 't':
                        tCoords.append(QVector2D(tokens[1].toFloat(), tokens[2].toFloat()));
                        break;
                    case 'n':
                        nCoords.append(QVector3D(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()));
                        break;
                    default:
                        vCoords.append(QVector3D(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()));
                        break;
                }
                break;
            }
            case 'f':
                for (int i = 1; i <= 3; i++)
                {
                    QStringList f = tokens[i].split("/");

                    int vindex = f[0].toLong();
                    int tindex = f[1].toLong();
                    int nindex = f[2].toLong();

                    vindex = vindex < 0 ? vCoords.size() + vindex : vindex - 1;
                    tindex = tindex < 0 ? tCoords.size() + tindex : tindex - 1;
                    nindex = nindex < 0 ? nCoords.size() + nindex : nindex - 1;

                    m_vertices.append(Vertex(vCoords[vindex], tCoords[tindex], nCoords[nindex]));
                    m_indices.append(m_indices.size());
                }
            default:
                break;
        }
    }

    if (m_vertices.empty()) {
        return false;
    }

    // Заполняем буфер
    if (m_vbo.isCreated()) m_vbo.destroy();
    Q_ASSERT(m_vbo.create());
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(m_vertices.constData(), m_vertices.size() * sizeof(Vertex));

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
    m_ebo.allocate(m_indices.constData(), m_indices.size() * sizeof(quint32));

    m_vao.release();
    m_ebo.release();
    m_vbo.release();

    return true;
}
