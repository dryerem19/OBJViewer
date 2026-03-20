#include "orbitcamera.h"


OrbitCamera::OrbitCamera(const QVector3D& center, const QVector3D& up, float r, float mr, float az, float pa, QObject *parent)
    : QObject{parent}
    , m_center(center)
    , m_upVector(up)
    , m_radius(r)
    , m_minRadius(mr)
    , m_azimuthAngle(az)
    , m_polarAngle(pa)
{
    update();
}

void OrbitCamera::rotateAzimuth(float radians)
{
    m_azimuthAngle += radians;

    const auto fullCircle = 2.0f * M_PI;
    m_azimuthAngle = fmodf(m_azimuthAngle, fullCircle);

    if (m_azimuthAngle < 0.0f) {
        m_azimuthAngle = fullCircle + m_azimuthAngle;
    }

    update();
}

void OrbitCamera::rotatePolar(float radians)
{
    m_polarAngle += radians;

    const auto polarCap = M_PI / 2.0f - 0.0001f;
    if (m_polarAngle > polarCap) {
        m_polarAngle = polarCap;
    }

    if (m_polarAngle < -polarCap) {
        m_polarAngle = -polarCap;
    }

    update();
}

void OrbitCamera::zoom(float distance)
{
    m_radius += distance;

    if (m_radius < m_minRadius) {
        m_radius = m_minRadius;
    }

    update();
}

void OrbitCamera::moveHorizontal(float distance)
{
    const auto eye      = getEye();
    const auto view     = getNormalizedViewVector();

    QVector3D strafe    = QVector3D::crossProduct(m_viewVector, m_upVector);
    strafe              = strafe.normalized();

    m_center            += strafe * distance;
    update();
}

void OrbitCamera::moveVertical(float distance)
{
    m_center += m_upVector * distance;
    update();
}

const QMatrix4x4 &OrbitCamera::getViewMatrix() const
{
    return m_viewMatrix;
}

const QVector3D &OrbitCamera::getEye() const
{
    return m_eyeVector;
}

const QVector3D &OrbitCamera::getViewPoint() const
{
    return m_center;
}

const QVector3D &OrbitCamera::getUpVector() const
{
    return m_upVector;
}

const QVector3D &OrbitCamera::getNormalizedViewVector() const
{
    return m_viewVector;
}

float OrbitCamera::getAzimuthAngle() const
{
    return m_azimuthAngle;
}

float OrbitCamera::getPolarAngle() const
{
    return m_polarAngle;
}

float OrbitCamera::getRadius() const
{
    return m_radius;
}

void OrbitCamera::update()
{
    // Eye
    float sineAzimuth   = sin(m_azimuthAngle);
    float cosineAzimuth = cos(m_azimuthAngle);
    float sinePolar     = sin(m_polarAngle);
    float cosinePolar   = cos(m_polarAngle);

    float x = m_center.x() + m_radius * cosinePolar * cosineAzimuth;
    float y = m_center.y() + m_radius * sinePolar;
    float z = m_center.z() + m_radius * cosinePolar * sineAzimuth;

    m_eyeVector.setX(x);
    m_eyeVector.setY(y);
    m_eyeVector.setZ(z);

    // View
    auto viewVector = m_center - m_eyeVector;
    m_viewVector = viewVector.normalized();

    // Matrix
    QMatrix4x4 m;
    m.setToIdentity();
    m.lookAt(m_eyeVector, m_eyeVector + m_viewVector, m_upVector);
    m_viewMatrix = m;
}
