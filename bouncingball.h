#ifndef BOUNCINGBALL_H
#define BOUNCINGBALL_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QElapsedTimer>
#include <QMatrix4x4>

#include "sprite.h"

class BouncingBall : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    BouncingBall();
    ~BouncingBall();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

public slots:
    void updateGLScene();

private:
    QElapsedTimer   m_ElapsedTimer;

    QOpenGLContext              *m_pContext;
    QOpenGLVertexArrayObject    m_BallVAO;
    QOpenGLBuffer               m_BallVB;
    QOpenGLShader               *m_pTexMapVS;
    QOpenGLShader               *m_pTexMapFS;
    QOpenGLShaderProgram        *m_pTexMapSP;
    QOpenGLTexture              *m_pBallTexture;

    QMatrix4x4      m_mtxView;
    QMatrix4x4      m_mtxViewProj;
    QMatrix4x4      m_mtxWorldViewProj;

    Sprite  m_Ball;
    int     m_XDirection;
    int     m_YDirection;

    void buildbuffers();
    void manageShaders();

    inline void moveBall(float velocity, int timeElapsed);
};

#endif // BOUNCINGBALL_H
