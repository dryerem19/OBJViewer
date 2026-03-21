#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include <QObject>
#include <QVector>
#include <QString>

class Material;
class MaterialLibrary : public QObject
{
    Q_OBJECT
public:
    explicit MaterialLibrary(QObject *parent = nullptr);
    ~MaterialLibrary();

public slots:
    void        clear               ();
    bool        addMaterial         (Material* material);
    Material*   getMaterial         (quint32 index)             const;
    Material*   getMaterial         (const QString& name)       const;
    quint32     getCountMaterials   ()                          const;
    bool        loadMaterialFromFile(const QString& filename);

private:
    QVector<Material*> m_materials;
};

#endif // MATERIALLIBRARY_H
