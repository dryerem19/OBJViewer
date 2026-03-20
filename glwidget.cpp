#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    startTimer(16);
}

GLWidget::~GLWidget()
{
    cleanup();
}

void GLWidget::initializeGL()
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

    mesh = new Mesh;
    mesh->load("cube.obj");

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
}

void GLWidget::resizeGL(int w, int h)
{
    glClearColor(0.25, 0.45, 0.65, 1.0);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, width() / (float)height(), 0.1f, 100.0f);

    // QMatrix4x4 viewMatrix;
    // // Камера дальше от куба - лучше видно
    // viewMatrix.lookAt(QVector3D(5.0f, 3.0f, 8.0f),  // позиция камеры дальше
    //                  QVector3D(0.0f, 0.0f, 0.0f),
    //                  QVector3D(0.0f, 1.0f, 0.0f));

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    m_program->setUniformValue("projectionMatrix", projectionMatrix);
    m_program->setUniformValue("modelMatrix", modelMatrix);
    m_program->setUniformValue("viewMatrix", m_camera->getViewMatrix());

    mesh->draw();
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
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

void GLWidget::mouseMoveEvent(QMouseEvent *event)
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

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
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

void GLWidget::wheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() / 120.0f;
    m_camera->zoom(delta * m_zoomSpeed);
}

void GLWidget::initShader()
{
    m_program = new QOpenGLShaderProgram(this);
    Q_ASSERT(m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/diffuse.vert"));
    Q_ASSERT(m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/diffuse.frag"));
    Q_ASSERT(m_program->link());
}

void GLWidget::cleanup()
{
    makeCurrent();

    doneCurrent();
    disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
}
