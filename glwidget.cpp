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

    QMatrix4x4 viewMatrix;
    // Камера дальше от куба - лучше видно
    viewMatrix.lookAt(QVector3D(5.0f, 3.0f, 8.0f),  // позиция камеры дальше
                     QVector3D(0.0f, 0.0f, 0.0f),
                     QVector3D(0.0f, 1.0f, 0.0f));

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    QMatrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;


    m_program->setUniformValue("projectionMatrix", projectionMatrix);
    m_program->setUniformValue("modelMatrix", modelMatrix);
    m_program->setUniformValue("viewMatrix", viewMatrix);

    mesh->draw();
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    update();
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
