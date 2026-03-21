#include "model.h"
#include "vertex.h"

#include <QFile>
#include <QVector>
#include <QFileInfo>
#include <QTextStream>

Model::Model(QObject *parent)
    : QObject{parent}
{

}

Model::~Model()
{
    for (int i = 0; i < m_meshes.size(); i++) {
        delete m_meshes[i];
    }
}

void Model::draw()
{
    for (int i = 0; i < m_meshes.size(); i++) {
        m_meshes[i]->draw();
    }
}

bool Model::loadObjFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QVector<QVector3D>  vCoords;
    QVector<QVector3D>  nCoords;
    QVector<QVector2D>  tCoords;

    QVector<Vertex>     vertices;
    QVector<quint32>    indices;

    Mesh*   mesh = nullptr;
    QString currentMaterialName;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString input = in.readLine().trimmed();
        if (input.isEmpty() || input[0] == '#') {
            continue;
        }

        QTextStream ts(&input);
        QString     begin;
        ts >> begin;

        if (begin == "mtllib")
        {
            QString materialLibraryName;
            ts >> materialLibraryName;

            QFileInfo   modelFileInfo(filename);
            QString     fullMaterialLibraryPath = QString("%1/%2").arg(modelFileInfo.absolutePath()).arg(materialLibraryName);
            m_mtl.loadMaterialFromFile(fullMaterialLibraryPath);
        }
        else if (begin == "v")
        {
            QVector3D p;
            for (int i = 0; i < 3; ++i) {
                ts >> ((float *)&p)[i];
            }
            vCoords.append(p);
        }
        else if (begin == "vt")
        {
            QVector2D t;
            for (int i = 0; i < 2; ++i) {
                ts >> ((float *)&t)[i];
            }
            tCoords.append(t);
        }
        else if (begin == "vn")
        {
            QVector3D n;
            for (int i = 0; i < 3; ++i) {
                ts >> ((float *)&n)[i];
            }
            nCoords.append(n);
        }
        else if (begin == "f" || begin == "fo")
        {
            while (!ts.atEnd())
            {
                QString face;
                ts >> face;

                QStringList faceTokens  = face.split('/');
                int         vindex      = faceTokens.value(0).toInt();
                int         tindex      = faceTokens.value(1).toInt();
                int         nindex      = faceTokens.value(2).toInt();

                vindex = vindex < 0 ? vCoords.size() + vindex : vindex - 1;
                tindex = tindex < 0 ? tCoords.size() + tindex : tindex - 1;
                nindex = nindex < 0 ? nCoords.size() + nindex : nindex - 1;

                vertices.append(Vertex(vCoords[vindex], nCoords[nindex], tCoords[tindex]));
                indices.append(indices.size());
            }
        }
        else if (begin == "usemtl")
        {
            if (mesh)
            {

                mesh->create(vertices, indices);
                mesh->setMaterial(m_mtl.getMaterial(currentMaterialName));
                addMesh(mesh);
            }

            QString materialName;
            ts >> currentMaterialName;

            mesh = new Mesh;
            vertices.clear();
            indices.clear();
        }
    }

    if (mesh)
    {
        mesh->create(vertices, indices);
        mesh->setMaterial(m_mtl.getMaterial(currentMaterialName));
        addMesh(mesh);
    }

    return m_meshes.size() > 0;
}

bool Model::addMesh(Mesh *mesh)
{
    if (!mesh) {
        return false;
    }

    for (int i = 0; i < m_meshes.size(); i++) {
        if (m_meshes[i] == mesh) {
            return false;
        }
    }

    mesh->setParent(this);
    m_meshes.append(mesh);
    return true;
}

Mesh *Model::getMesh(quint32 index) const
{
    if (index < (quint32)m_meshes.size()) {
        return m_meshes[index];
    }

    return nullptr;
}
