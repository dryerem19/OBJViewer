#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>

#include <QMouseEvent>
#include <QWheelEvent>

#include "scene.h"
#include "orbitcamera.h"

class SceneWidget final : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    SceneWidget(QWidget *parent);
    ~SceneWidget();

public slots:
    Scene* getScene();

protected:
    void initializeGL   ()                      final;
    void resizeGL       (int w, int h)          final;
    void paintGL        ()                      final;
    void timerEvent     (QTimerEvent *event)    final;

    void mousePressEvent    (QMouseEvent *event);
    void mouseMoveEvent     (QMouseEvent *event);
    void mouseReleaseEvent  (QMouseEvent *event);
    void wheelEvent         (QWheelEvent *event);

private:
    void initShader     ();
    void cleanup        ();

    bool    m_moving        = false;
    bool    m_rotating      = false;
    float   m_zoomSpeed     = 0.1f;
    float   m_rotateSpeed   = 0.1f;
    float   m_sensitivity   = 0.005f;
    QPoint  m_lastMousePos  = QPoint(0, 0);

    Scene m_scene;
    QOpenGLShaderProgram*           m_program = nullptr;
    QOpenGLDebugLogger              m_logger;
    std::unique_ptr<OrbitCamera>    m_camera;
};

#endif // SCENEWIDGET_H
