#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include <QTreeWidget>
#include <QObject>

class Model;
class Scene;
class ObjectList final : public QTreeWidget
{
    Q_OBJECT
public:
    ObjectList(QWidget *parent);

public slots:
    void setScene(Scene* scene);

private slots:
    void onAddModel(Model* model);

private:
    Scene* m_scene{nullptr};
};

#endif // OBJECTLIST_H
