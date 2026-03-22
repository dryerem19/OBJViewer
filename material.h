#ifndef MATERIAL_H
#define MATERIAL_H

#include <QImage>
#include <QObject>
#include <QString>
#include <QVector3D>
#include <QOpenGLTexture>

class Material : public QObject
{
    Q_OBJECT
public:
    explicit Material(QObject *parent = nullptr);

public slots:
    void                setName         (const QString& name);
    void                setShinnes      (float ns);
    void                setDiffuseMap   (const QString& filename);
    void                setDiffuseColor (const QVector3D& kd);
    void                setAmbienceColor(const QVector3D& ka);
    void                setSpecularColor(const QVector3D& ks);

    const QString&      getName         () const;
    const float&        getShiness      () const;
    QOpenGLTexture*     getDiffuseMap   () const;
    const QVector3D&    getDiffuseColor () const;
    const QVector3D&    getAmbienceColor() const;
    const QVector3D&    getSpecularColor() const;
    const bool          isHasDiffuseMap () const;

private:
    QString             m_name;
    float               m_shinnes;
    QVector3D           m_diffuseColor;
    QVector3D           m_ambienceColor;
    QVector3D           m_specularColor;
    QOpenGLTexture*     m_diffuseMap{nullptr};
};

#endif // MATERIAL_H
