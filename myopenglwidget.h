#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

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
#include <QMainWindow>

const GLuint lats = 4;
const GLuint lons = 10;
const GLuint division = 4;
const int n = division-1;

class MainWindow;

enum ToolMode
{
    Pen,
    Eraser,
    Anchor
};

enum MouseMode
{
    Press,
    Release
};

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    MainWindow* mainwindow;
    void setMainWindow(MainWindow* mainwindow);

    MyOpenGLWidget(QWidget* parent);
    ~MyOpenGLWidget();

    float cpara[division];
    bool frameworkVisibility = true;
    bool startAnimation = false;

    void visibilityToggle(){frameworkVisibility=!frameworkVisibility;}
    void animationToggle(){startAnimation=!startAnimation;}
    void InitCpara();
    void CreateSphere();
    void CreateCube();
    void RenewSphere();
    // Function: Interaction
    void setToolModePen(){toolMode=Pen;qDebug()<<tr("Set Mode Pen");}
    void setToolModeEraser(){toolMode=Eraser;}
    void setToolModeAnchor(){toolMode=Anchor;}

    bool mouseHitPoint(QVector2D& point, QMouseEvent *event);

    void resetPara(float para);
    void resetX(int x);
    void resetY(int y);
    void resetZ(int z);

private:
    // Camera Controller
    // ----------------------
    float c = 3.1415926 / 180.0f; //弧度和角度转换参数
    int cameraTheta = 90;
    int cameraOldX = -1;
    int cameraOldY = -1;
    float cameraH=0.0f;
    float camera_r = 1.5f;

    QVector<QVector2D> anchors;
    int anchorChosen=-1;
    GLfloat sphereVertices[3 * (lats+1) * (lons+1)];

    QTimer *timer;
    QElapsedTimer clock;
    int m_nTimeValue = 0;
    int m_chosenPoint = -1;

    // Field: Interaction
    ToolMode toolMode = Pen;
    MouseMode mouseMode = Release;

    // Field: OpenGL
    QOpenGLBuffer cubeVBO;
    QOpenGLBuffer cubeEBO;
    QOpenGLBuffer sphereVBO;
    QOpenGLBuffer sphereEBO;
    QOpenGLVertexArrayObject cubeVAO;
    QOpenGLVertexArrayObject sphereVAO;
    QOpenGLShaderProgram *colorProgram;
    QOpenGLShaderProgram *singleColorProgram;

    // Function: to ease codes
    void setArrowCursor();
    void setCrossCursor();

    QMatrix4x4 anchorProj;
    QMatrix4x4 anchorView;
    QMatrix4x4 anchorModel;

    float currentTime;

    GLfloat cubeVertices[division*division*division*6];
    bool verticeXDir[division*division*division*6];
    bool verticeYDir[division*division*division*6];
    bool verticeZDir[division*division*division*6];
    GLuint cubeIndices[ ((division*(division-1)*2)*division + division*division*(division-1))*2];

protected:
    // Function: OpenGL
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Widget Enter / Leave event
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    // Mouse Move Event
    void mouseMoveEvent(QMouseEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseLeftPressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MYOPENGLWIDGET_H
