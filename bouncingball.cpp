#include "bouncingball.h"

#include <QSurfaceFormat>
#include <QSurface>
#include <QImage>
#include <QVector2D>
#include <QDebug>

using namespace std;

struct Vertex
{
    QVector3D position;
    QVector2D texUV;
};

enum Direction
{
    LEFT = -1,
    RIGHT = 1,
    UP = 1,
    DOWN = -1,
};

BouncingBall::BouncingBall()
    : QOpenGLWindow()
{
    this->setSurfaceType(QSurface::OpenGLSurface);

    QSurfaceFormat surfaceFormat;
    surfaceFormat.setVersion(4, 4);
    surfaceFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);

    this->setWidth(640);
    this->setHeight(480);

    this->m_pContext = new QOpenGLContext(this);
    this->m_pContext->setFormat(surfaceFormat);
    this->m_pContext->create();
    this->m_pContext->makeCurrent(this);

    this->m_mtxView.setToIdentity();
    this->m_mtxViewProj.setToIdentity();
    this->m_mtxWorldViewProj.setToIdentity();

    this->m_XDirection = Direction::RIGHT;
    this->m_YDirection = Direction::UP;

    connect(this, SIGNAL(frameSwapped()), this, SLOT(updateGLScene()));
}

BouncingBall::~BouncingBall()
{

}

void BouncingBall::initializeGL()
{
    this->initializeOpenGLFunctions();

    this->glViewport(0, 0, this->width(), this->height());

    this->m_pBallTexture = new QOpenGLTexture(QImage(":/textures/ballTex.dds").mirrored());
    this->m_pBallTexture->create();

    this->buildbuffers();

    this->manageShaders();

    this->m_Ball.setOrigin(QVector2D(100.0f, 100.0f));

    this->m_mtxView.lookAt(QVector3D(0.0f, 0.0f, 5.0f), QVector3D(0.0f, 0.0f, 0.0f),
                   QVector3D(0.0f, 1.0f, 0.0f));

    QMatrix4x4 mtxProj;
    mtxProj.setToIdentity();
    mtxProj.ortho(0.0f, (float)this->width(), 0.0f, (float)this->height(),
                                              -5.0f, 10.0f);

    this->m_mtxViewProj = mtxProj * this->m_mtxView;

    this->glEnable(GL_BLEND);
    this->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BouncingBall::paintGL()
{
    this->m_ElapsedTimer.start();

    this->glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT);

    this->m_pTexMapSP->bind();

    this->m_BallVAO.bind();
    this->m_pBallTexture->bind(0);
    QMatrix4x4 mtxWorld;
    mtxWorld.setToIdentity();
    mtxWorld = this->m_Ball.getModelMatrix();
    this->m_mtxWorldViewProj = this->m_mtxViewProj * mtxWorld;

    this->glUniformMatrix4fv(0, 1, GL_FALSE, this->m_mtxWorldViewProj.data());

    this->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    this->m_BallVAO.release();
    this->m_pTexMapSP->release();
}

void BouncingBall::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);

    QMatrix4x4 mtxProj;
    mtxProj.setToIdentity();
    mtxProj.ortho(0.0f, (float)w, 0.0f, (float)h, 0.0f, 10.0f);
    this->m_mtxViewProj = mtxProj * this->m_mtxView;
}

void BouncingBall::updateGLScene()
{
    this->moveBall(0.5f, this->m_ElapsedTimer.elapsed());

    QVector2D currentBallPosition = this->m_Ball.getOrigin();

    if((currentBallPosition.x() - 16.0f) <= 0.0f)
    {
        this->m_XDirection = Direction::RIGHT;
    }
    if((currentBallPosition.x() + 16.0f) >= this->width())
    {
        this->m_XDirection = Direction::LEFT;
    }
    if((currentBallPosition.y() - 16.0f) <= 0.0f)
    {
        this->m_YDirection = Direction::UP;
    }
    if((currentBallPosition.y() + 16.0f) >= this->height())
    {
        this->m_YDirection = Direction::DOWN;
    }
    this->update();
}

void BouncingBall::buildbuffers()
{
    float halfBallWidth = this->m_pBallTexture->width() / 2.0f;
    float halfBallHeight = this->m_pBallTexture->height() / 2.0f;

    Vertex ballVertices[] =
    {
        { QVector3D(-halfBallWidth, -halfBallHeight, 0.0f), QVector2D(0.0f, 0.0f) },
        { QVector3D(+halfBallWidth, -halfBallHeight, 0.0f), QVector2D(1.0f, 0.0f) },
        { QVector3D(-halfBallWidth, +halfBallHeight, 0.0f), QVector2D(0.0f, 1.0f) },
        { QVector3D(+halfBallWidth, +halfBallHeight, 0.0f), QVector2D(1.0f, 1.0f) },
    };

    this->m_BallVAO.create();
    this->m_BallVAO.bind();

    this->m_BallVB.create();
    this->m_BallVB.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->m_BallVB.bind();
    this->m_BallVB.allocate(ballVertices, 4 * sizeof(Vertex));

    this->glEnableVertexAttribArray(0);
    this->glEnableVertexAttribArray(1);

    this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                reinterpret_cast<void*>(offsetof(Vertex, position)));
    this->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                reinterpret_cast<void*>(offsetof(Vertex, texUV)));

    this->m_BallVAO.release();
}

void BouncingBall::manageShaders()
{
    this->m_pTexMapVS = new QOpenGLShader(QOpenGLShader::Vertex, this);
    this->m_pTexMapVS->compileSourceFile(":/shaders/TexMap.vsh");
    
    this->m_pTexMapFS = new QOpenGLShader(QOpenGLShader::Fragment, this);
    this->m_pTexMapFS->compileSourceFile(":/shaders/TexMap.fsh");

    this->m_pTexMapSP = new QOpenGLShaderProgram(this);
    this->m_pTexMapSP->create();
    this->m_pTexMapSP->addShader(this->m_pTexMapVS);
    this->m_pTexMapSP->addShader(this->m_pTexMapFS);
    this->m_pTexMapSP->link();
    this->m_pTexMapSP->release();
}

void BouncingBall::moveBall(float velocity, int timeElapsed)
{
    QVector2D currentPosition = this->m_Ball.getOrigin();

    QVector2D newPosition;
    newPosition.setX(currentPosition.x() +
                     (velocity * timeElapsed) * this->m_XDirection);
    newPosition.setY(currentPosition.y() +
                     (velocity * timeElapsed) * this->m_YDirection);

    this->m_Ball.setOrigin(newPosition);
}
