#include "scenewidget.h"

SceneWidget::SceneWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    startTimer(16);
}

SceneWidget::~SceneWidget()
{
    cleanup();
}

Scene *SceneWidget::getScene()
{
    return &m_scene;
}

void SceneWidget::initializeGL()
{
    initializeOpenGLFunctions();

#ifdef QT_DEBUG
    if (m_logger.initialize())
    {
        m_logger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
        m_logger.enableMessages();

        connect(&m_logger, &QOpenGLDebugLogger::messageLogged, [](QOpenGLDebugMessage message) {
            qDebug() << message;
        });
    }
#endif

    glEnable(GL_DEPTH_TEST);

    initShader();

    m_camera = std::make_unique<OrbitCamera>();
    m_scene.loadObjFromFile("cube.obj");

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &SceneWidget::cleanup);
}

void SceneWidget::resizeGL(int w, int h)
{
    glClearColor(0.25, 0.45, 0.65, 1.0);
}

void SceneWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, width() / (float)height(), 0.1f, 100.0f);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    m_program->setUniformValue("projectionMatrix", projectionMatrix);
    m_program->setUniformValue("modelMatrix", modelMatrix);
    m_program->setUniformValue("viewMatrix", m_camera->getViewMatrix());

    m_scene.draw(m_program);
}

void SceneWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    update();
}

void SceneWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !m_rotating && !m_moving)
    {
        m_rotating      = true;
        m_lastMousePos  = event->pos();
    }
    else if (event->button() == Qt::MiddleButton && !m_rotating && !m_moving)
    {
        m_moving        = true;
        m_lastMousePos  = event->pos();
    }
}

void SceneWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_rotating)
    {
        int deltaX = event->pos().x() - m_lastMousePos.x();
        int deltaY = event->pos().y() - m_lastMousePos.y();

        m_camera->rotateAzimuth(deltaX * m_rotateSpeed);
        m_camera->rotatePolar(deltaY * m_rotateSpeed);
        m_lastMousePos = event->pos();
    }
    else if (m_moving)
    {
        int deltaX = event->pos().x() - m_lastMousePos.x();
        int deltaY = event->pos().y() - m_lastMousePos.y();

        m_camera->moveHorizontal(-deltaX * m_sensitivity);
        m_camera->moveVertical(deltaY * m_sensitivity);
        m_lastMousePos = event->pos();
    }
}

void SceneWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_rotating = false;
    }
    else if (event->button() == Qt::MiddleButton)
    {
        m_moving = false;
    }
}

void SceneWidget::wheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() / 120.0f;
    m_camera->zoom(delta * m_zoomSpeed);
}

void SceneWidget::initShader()
{
    m_program = new QOpenGLShaderProgram(this);
    Q_ASSERT(m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/diffuse.vert"));
    Q_ASSERT(m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/diffuse.frag"));
    Q_ASSERT(m_program->link());
}

void SceneWidget::cleanup()
{
    makeCurrent();

    doneCurrent();
    disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &SceneWidget::cleanup);
}
