#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H

#include <QtMath>
#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>

class OrbitCamera : public QObject
{
    Q_OBJECT
public:
    explicit OrbitCamera(const QVector3D& center = QVector3D(0.0f, 0.0f, 0.0f),
                         const QVector3D& up = QVector3D(0.0, 1.0f, 0.0f),
                         float r = 15, float mr = 3, float az = M_PI * 0.5f, float pa = 0.0f, QObject *parent = nullptr);

public slots:
    void rotateAzimuth  (float radians);
    void rotatePolar    (float radians);
    void zoom           (float distance);

    void moveHorizontal (float distance);
    void moveVertical   (float distance);

    const QMatrix4x4&   getViewMatrix           () const;
    const QVector3D&    getEye                  () const;
    const QVector3D&    getViewPoint            () const;
    const QVector3D&    getUpVector             () const;
    const QVector3D&    getNormalizedViewVector () const;

    float getAzimuthAngle   () const;
    float getPolarAngle     () const;
    float getRadius         () const;

private:
    float       m_radius;
    float       m_minRadius;
    float       m_polarAngle;
    float       m_azimuthAngle;

    QVector3D   m_center;
    QVector3D   m_upVector;
    QVector3D   m_eyeVector;
    QVector3D   m_viewVector;
    QMatrix4x4  m_viewMatrix;

    void update();
};

#endif // ORBITCAMERA_H
