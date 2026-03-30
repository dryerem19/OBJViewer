#include "material.h"


Material::Material(QObject *parent)
    : QObject{parent}
{

}

void Material::setName(const QString &name)
{
    m_name = name;
}

void Material::setShinnes(float ns)
{
    m_shinnes = ns;
}

void Material::setDiffuseMap(const QString &filename)
{
    m_diffuseMap = new QOpenGLTexture(QImage(filename).mirrored());
}

void Material::setDiffuseColor(const QVector3D &kd)
{
    m_diffuseColor = kd;
}

void Material::setAmbienceColor(const QVector3D &ka)
{
    m_ambienceColor = ka;
}

void Material::setSpecularColor(const QVector3D &ks)
{
    m_specularColor = ks;
}

const QString &Material::getName() const
{
    return m_name;
}

const float &Material::getShiness() const
{
    return m_shinnes;
}

QOpenGLTexture *Material::getDiffuseMap() const
{
    return m_diffuseMap;
}

const QVector3D &Material::getDiffuseColor() const
{
    return m_diffuseColor;
}

const QVector3D &Material::getAmbienceColor() const
{
    return m_ambienceColor;
}

const QVector3D &Material::getSpecularColor() const
{
    return m_specularColor;
}
