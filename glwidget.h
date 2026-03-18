#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>

#include "mesh.h"

class GLWidget final : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLWidget(QWidget *parent);
    ~GLWidget();

public slots:
    void addMesh(QScopedPointer<Mesh> mesh);

protected:
    void initializeGL   ()                      final;
    void resizeGL       (int w, int h)          final;
    void paintGL        ()                      final;
    void timerEvent     (QTimerEvent *event)    final;

private:
    void initShader     ();
    void cleanup        ();

    QOpenGLShaderProgram* m_program = nullptr;
    QOpenGLDebugLogger m_logger;
    Mesh* mesh;
};

#endif // GLWIDGET_H
