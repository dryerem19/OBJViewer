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

private:
    QString             m_name{"defaultMaterial"};
    float               m_shinnes{10};
    QVector3D           m_diffuseColor{1.0f, 1.0f, 1.0f};
    QVector3D           m_ambienceColor{1.0f, 1.0f, 1.0f};
    QVector3D           m_specularColor{0.2f, 0.2f, 0.2f};
    QOpenGLTexture*     m_diffuseMap{nullptr};
};

#endif // MATERIAL_H
