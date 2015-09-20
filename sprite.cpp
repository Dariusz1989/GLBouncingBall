#include "sprite.h"

#include <QTransform>

Sprite::Sprite()
{
    this->m_Origin = QVector2D(0.0f, 0.0f);
    this->m_Scale = QVector2D(1.0f, 1.0f);
    this->m_RotationAngle = 0.0f;
    this->m_RotationAxises = QVector2D(0.0f, 0.0f);
}

void Sprite::setOrigin(QVector2D origin)
{
    this->m_Origin = origin;
}

void Sprite::setRotation(float angle, QVector2D axises)
{
    this->m_RotationAngle = angle;
    this->m_RotationAxises = axises;
}

void Sprite::setScale(QVector2D scale)
{
    this->m_Scale = scale;
}

QVector2D Sprite::getOrigin() const
{
    return this->m_Origin;
}

float Sprite::getRotationAngle() const
{
    return this->m_RotationAngle;
}

QVector2D Sprite::getScale() const
{
    return this->m_Scale;
}

QMatrix4x4 Sprite::getModelMatrix() const
{
    QMatrix4x4 mtxTranslation;
    mtxTranslation.setToIdentity();
    mtxTranslation.translate(this->m_Origin.x(), this->m_Origin.y(), 0.0f);

    QMatrix4x4 mtxRotation;
    mtxRotation.setToIdentity();
    mtxRotation.rotate(this->m_RotationAngle,
                       this->m_RotationAxises.x(), this->m_RotationAxises.y(), 0.0f);

    QMatrix4x4 mtxScale;
    mtxScale.setToIdentity();
    mtxScale.scale(this->m_Scale.x(), this->m_Scale.y(), 1.0f);

    return (mtxTranslation /* mtxScale * mtxRotation*/);
}

