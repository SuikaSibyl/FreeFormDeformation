#ifndef MESHCREATER_H
#define MESHCREATER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

class MeshCreator:protected QOpenGLFunctions
{
public:
    MeshCreator();
    void CreateSphere();
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
private:
};

#endif // MESHCREATER_H
