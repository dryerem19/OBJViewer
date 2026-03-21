#include "materiallibrary.h"
#include "material.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

MaterialLibrary::MaterialLibrary(QObject *parent)
    : QObject{parent}
{

}

MaterialLibrary::~MaterialLibrary()
{
    clear();
}

void MaterialLibrary::clear()
{
    for (int i = 0; i < m_materials.size(); i++) {
        delete m_materials[i];
    }

    m_materials.clear();
}

bool MaterialLibrary::addMaterial(Material *material)
{
    if (!material) {
        return false;
    }

    for (int i = 0; i < m_materials.size(); i++)
    {
        if (m_materials[i] == material) {
            return false;
        }
    }

    m_materials.append(material);
    return true;
}

Material *MaterialLibrary::getMaterial(quint32 index) const
{
    if (index < (quint32)m_materials.size()) {
        return m_materials[index];
    }

    return nullptr;
}

Material *MaterialLibrary::getMaterial(const QString &name) const
{
    for (int i = 0; i < m_materials.size(); i++)
    {
        if (m_materials[i]->getName() == name) {
            return m_materials[i];
        }
    }

    return nullptr;
}

quint32 MaterialLibrary::getCountMaterials() const
{
    return m_materials.size();
}

bool MaterialLibrary::loadMaterialFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    clear();
    Material* material = nullptr;

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

        if (begin == "newmtl")
        {
            QString name;
            ts >> name;

            addMaterial(material);
            material = new Material;
            material->setName(name);
        }
        else if (begin == "Ns")
        {
            float ns;
            ts >> ns;
            material->setShinnes(ns);
        }
        else if (begin == "Ka")
        {
            QVector3D ka;
            for (int i = 0; i < 3; i++) {
                ts >> ((float*)&ka)[i];
            }
            material->setAmbienceColor(ka);
        }
        else if (begin == "Kd")
        {
            QVector3D kd;
            for (int i = 0; i < 3; i++) {
                ts >> ((float*)&kd)[i];
            }
            material->setDiffuseColor(kd);
        }
        else if (begin == "Ks")
        {
            QVector3D ks;
            for (int i = 0; i < 3; i++) {
                ts >> ((float*)&ks)[i];
            }
            material->setSpecularColor(ks);
        }
        else if (begin == "map_Kd")
        {
            QString mapName;
            ts >> mapName;

            QFileInfo   matFileInfo(filename);
            QString     fullMapPath = QString("%1/%2").arg(matFileInfo.absolutePath()).arg(mapName);
            material->setDiffuseMap(fullMapPath);
        }
    }

    addMaterial(material);
    return m_materials.size() > 0;
}
