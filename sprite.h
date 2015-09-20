#ifndef SPRITE_H
#define SPRITE_H

#include <QMatrix4x4>
#include <QVector2D>

class Sprite
{
public:
    Sprite();

    void setOrigin(QVector2D origin);
    void setRotation(float angle, QVector2D axises);
    void setScale(QVector2D scale);

    QVector2D getOrigin() const;
    float getRotationAngle() const;
    QVector2D getScale() const;
    QMatrix4x4 getModelMatrix() const;

private:
    QVector2D   m_Origin;
    QVector2D   m_Scale;
    float       m_RotationAngle;
    QVector2D   m_RotationAxises;
};

#endif // SPRITE_H
