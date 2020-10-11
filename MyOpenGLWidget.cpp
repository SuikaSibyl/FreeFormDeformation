#include "myopenglwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>
#include <myopenglwidget.h>
#include <glut.h>
#include "math.h"
#include "mainwindow.h"

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f
};

GLuint  indices[] = {
    0, 1,   1, 2,   2, 3,   3, 0,
    4, 5,   5, 6,   6, 7,   7, 4,
    0, 4,   1, 5,   2, 6,   3, 7
};

void MyOpenGLWidget::resetPara(float para){
    cubeVBO.bind();
    float parameter[]={para};
    cubeVBO.write(anchorChosen*6*sizeof (float), parameter, sizeof(parameter));
    vertices[anchorChosen*6]=para;
    cubeVBO.release();
}

QVector3D  getPoint(GLfloat u, GLfloat v) {
    GLfloat r = 0.5f;
    GLfloat pi = 3.1416;
    GLfloat z = r * std::cos(pi * u);
    GLfloat x = r * std::sin(pi * u) * std::cos(2 * pi * v);
    GLfloat y = r * std::sin(pi * u) * std::sin(2 * pi * v);
    return QVector3D(x, y, z);
}

void createCube(GLfloat* vertices, GLuint* indices, GLuint division) {
    GLfloat step = 1.0f / (division-1);
    GLuint offset_point = 0;
    GLuint offset_indices = 0;
    for(int i=0; i<division; i++){
        for(int j=0; j<division;j++){
            for(int k=0;k<division;k++){
                QVector3D point(-0.5f+i*step, -0.5f+j*step, -0.5f+k*step);
                memcpy(vertices + offset_point, &point, 3 * sizeof(GLfloat));
                offset_point += 3;
                QVector3D color(i*step, j*step, 1.0f);
                memcpy(vertices + offset_point, &color, 3 * sizeof(GLfloat));
                offset_point += 3;

                if(i<division-1){
                    int point1 = i*division*division+j*division+k;
                    int point2 = (i+1)*division*division+j*division+k;
                    memcpy(indices + offset_indices, &point1, sizeof(GLuint));
                    offset_indices += 1;
                    memcpy(indices + offset_indices, &point2, sizeof(GLuint));
                    offset_indices += 1;
                }
                if(j<division-1){
                    int point1 = i*division*division+j*division+k;
                    int point2 = i*division*division+(j+1)*division+k;
                    memcpy(indices + offset_indices, &point1, sizeof(GLuint));
                    offset_indices += 1;
                    memcpy(indices + offset_indices, &point2, sizeof(GLuint));
                    offset_indices += 1;
                }
                if(k<division-1){
                    int point1 = i*division*division+j*division+k;
                    int point2 = i*division*division+j*division+k+1;
                    memcpy(indices + offset_indices, &point1, sizeof(GLuint));
                    offset_indices += 1;
                    memcpy(indices + offset_indices, &point2, sizeof(GLuint));
                    offset_indices += 1;
                }
            }
        }
    }
}

void createSphere(GLfloat* sphere, GLuint* indices, GLuint Longitude, GLuint Latitude) {
    GLfloat lon_step = 1.0f / Longitude;
    GLfloat lat_step = 1.0f / Latitude;
    GLuint offset_point = 0;
    for (int lat = 0; lat <= Latitude; lat++) {
        for (int lon = 0; lon <= Longitude; lon++) {
            QVector3D point1 = getPoint(lat * lat_step, lon * lon_step);
            memcpy(sphere + offset_point, &point1, 3 * sizeof(GLfloat));
            offset_point += 3;
        }
    }

    GLuint offset_indices = 0;
    for (int lat = 0; lat < Latitude; lat++) {
        for (int lon = 0; lon < Longitude; lon++) {
            GLuint point1 = lat * (Longitude+1) + lon;
            GLuint point2 = (lat+1) * (Longitude+1) + lon;
            GLuint point3 = lat * (Longitude+1) + (lon+1);
            GLuint point4 = (lat+1)  * (Longitude+1) +(lon+1);

            memcpy(indices + offset_indices, &point1, sizeof(GLuint));
            offset_indices++;
            memcpy(indices + offset_indices, &point3, sizeof(GLuint));
            offset_indices++;
            memcpy(indices + offset_indices, &point4, sizeof(GLuint));
            offset_indices++;
        }
    }
}

void MyOpenGLWidget::CreateCube(){
    cubeVAO.create();
    cubeVAO.bind();

    createCube(cubeVertices, cubeIndices, division);

    //  Create VBO (Vertex Buffer Object)
    cubeVBO.create();
    cubeVBO.bind();
    cubeVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    cubeVBO.allocate(cubeVertices, sizeof(cubeVertices));

    cubeEBO.create();
    cubeEBO.bind();
    cubeEBO.allocate(cubeIndices, sizeof(cubeIndices));

    // position attribute
    colorProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat)*6);
    colorProgram->enableAttributeArray(0);
    // color attribute
    colorProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat)*3, 3, sizeof(GLfloat)*6);
    colorProgram->enableAttributeArray(1);

    cubeVAO.release();
    cubeVBO.release();
}

int computeCMN(int m, int n) {
    if (n == 0 || m == n) return 1;
    int c1 = computeCMN(m - 1, n);
    int c2 = computeCMN(m - 1, n - 1);
    return c1 + c2;
}

void MyOpenGLWidget::InitCpara(){
    for(int i=0; i<division;i++){
        cpara[i]=computeCMN(division-1, i);
    }
}

void MyOpenGLWidget::RenewSphere(){
    for(int p=0;  p <3 * (lats+1) * (lons+1); p++){
        QVector3D localP(sphereVertices[p*3]+0.5f, sphereVertices[p*3+1]+0.5f, sphereVertices[p*3+2]+0.5f);
        QVector3D point(0.0,0.0,0.0);
        for(int i=0; i<division; i++){
            for(int j=0; j<division; j++){
                for(int k=0; k<division; k++){
                    int c = i *division*division+j*division+k;
                    QVector3D controlP(cubeVertices[6*c], cubeVertices[6*c+1], cubeVertices[6*c+2]);
                    float weight = cpara[i]*pow(localP.x(),i)*pow(1.0f-localP.x(),n-i) *
                            cpara[j]*pow(localP.y(),j)*pow(1.0f-localP.y(),n-j) *
                            cpara[k]*pow(localP.z(),k)*pow(1.0f-localP.z(),n-k);
                    point += weight * controlP;
                }
            }
        }
        sphereVBO.bind();
        sphereVBO.write((p*3)*sizeof (float), &point, sizeof(point));
        sphereVBO.release();
    }
}

void MyOpenGLWidget::CreateSphere(){
    sphereVAO.create();
    sphereVAO.bind();

    GLuint sphereIndices[ 3 * lats * lons];
    createSphere(sphereVertices, sphereIndices, lons, lats);

    //  Create VBO (Vertex Buffer Object)
    sphereVBO.create();
    sphereVBO.bind();
    sphereVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    sphereVBO.allocate(sphereVertices, sizeof(sphereVertices));

    sphereEBO.create();
    sphereEBO.bind();
    sphereEBO.allocate(sphereIndices, sizeof(sphereIndices));

    singleColorProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat)*3);
    singleColorProgram->enableAttributeArray(0);

    sphereVAO.release();
    sphereVBO.release();
}

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), cubeEBO(QOpenGLBuffer::IndexBuffer),sphereEBO(QOpenGLBuffer::IndexBuffer)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);

    //  The following is not compulsory
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format); // must be called before the widget or its parent window gets shown
}

MyOpenGLWidget::~MyOpenGLWidget(){
    if(cubeVBO.isCreated())
        cubeVBO.destroy();
    if(cubeVAO.isCreated())
        cubeVAO.destroy();
    if(colorProgram)
        delete colorProgram;
    if(singleColorProgram)
        delete singleColorProgram;
}

void MyOpenGLWidget::initializeGL()
{
    InitCpara();
    //  Initialize the OpenGL
    initializeOpenGLFunctions();
    glClearColor (0.125,0.125,0.125,1);

    // Initialize Shader
    // -----------------
    colorProgram = new QOpenGLShaderProgram();
    colorProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
    colorProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    if (colorProgram->link()) {
        qDebug("Shaders link success.");
    } else {
        qDebug("Shaders link failed!");
    }

    colorProgram->bind();
    CreateCube();

    // Bind Matrix to Shaders
    // ---------------------------
    QMatrix4x4 projection;
    projection.perspective(45.0f, 1.0f * width() / height(), 0.1f, 100.0f);
    colorProgram->setUniformValue("projection", projection);
    colorProgram->release();
    anchorProj = projection;

    // Initialize Shader
    // -----------------
    singleColorProgram = new QOpenGLShaderProgram();
    singleColorProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/singleColor.vert");
    singleColorProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/singleColor.frag");
    if (singleColorProgram->link()) {
        qDebug("Shaders link success.");
    } else {
        qDebug("Shaders link failed!");
    }
    singleColorProgram->bind();
    // position attribute
    CreateSphere();
    singleColorProgram->setUniformValue("projection", projection);
    singleColorProgram->release();

    clock.restart();
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    if(0 == h)
        h = 1;

    glViewport(0, 0, w, h);

    QMatrix4x4 projMatrix;
    projMatrix.setToIdentity();
    projMatrix.perspective(45.0f,  (GLfloat)w / (GLfloat)h, 0.01f, 100.0f);
    anchorProj = projMatrix;

    colorProgram->bind();
    int projMatrixLoc = colorProgram->uniformLocation("projection");
    colorProgram->setUniformValue(projMatrixLoc, projMatrix);
    colorProgram->release();

    singleColorProgram->bind();
    int projMatrixLoc2 = singleColorProgram->uniformLocation("projection");
    singleColorProgram->setUniformValue(projMatrixLoc2, projMatrix);
    singleColorProgram->release();
}

void MyOpenGLWidget::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    colorProgram->bind();

    float timeGap = clock.elapsed();
    clock.restart();
    qDebug()<<timeGap;

    if(startAnimation){
        float dirx = 0.001f * timeGap;
        float diry = 0.001f * timeGap;
        float dirz = 0.001f * timeGap;
        for(int i=0 ;i<division*division*division; i++){
            cubeVBO.bind();

            float x = cubeVertices[i*6];
            if(x-dirx<-1.0f || x+ dirx >1.0f){
                verticeXDir[i]=!verticeXDir[i];
            }
            if(verticeXDir[i]) x+=dirx;
            else x-=dirx;
            float parameterx[]={x};
            cubeVBO.write((i*6)*sizeof (float), parameterx, sizeof(parameterx));
            cubeVertices[i*6]=x;

            float y = cubeVertices[i*6+1];
            if(y-diry*0.5<-1.0f || y+ diry*0.5 >1.0f){
                verticeYDir[i]=!verticeYDir[i];
            }
            if(verticeYDir[i]) y+=diry*0.5;
            else y-=diry*0.5;
            float parametery[]={y};
            cubeVBO.write((i*6+1)*sizeof (float), parametery, sizeof(parametery));
            cubeVertices[i*6+1]=y;

            float z = cubeVertices[i*6+2];
            if(z-dirz*2<-1.0f || z+ dirz*2 >1.0f){
                verticeZDir[i]=!verticeZDir[i];
            }
            if(verticeZDir[i]) z+=dirz*2;
            else z-=dirz*2;
            float parameterz[]={z};
            cubeVBO.write((i*6+2)*sizeof (float), parameterz, sizeof(parameterz));
            cubeVertices[i*6+2]=z;

            cubeVBO.release();
            RenewSphere();
        }
    }

    QMatrix4x4 view;
    view.translate(QVector3D(0.0f, 0.0f, -3.0f));
    QVector3D cameraPosition (camera_r*cos(c*cameraTheta), cameraH, camera_r*sin(c*cameraTheta));
    view.lookAt(cameraPosition, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    colorProgram->setUniformValue("view", view);
    anchorView = view;

    QMatrix4x4 modelMatrix;
    colorProgram->setUniformValue("model", modelMatrix);
    anchorModel = modelMatrix;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(frameworkVisibility){
        cubeVAO.bind();
        cubeEBO.bind();
            glDrawElements(GL_LINES,  ((division*(division-1)*2)*division + division*division*(division-1))*2, GL_UNSIGNED_INT, nullptr);
        cubeVAO.release();
        colorProgram->release();
    }
    singleColorProgram->bind();
    singleColorProgram->setUniformValue("view", view);
    singleColorProgram->setUniformValue("model", modelMatrix);
    sphereVAO.bind();
    sphereEBO.bind();
        glDrawElements(GL_TRIANGLES, 3 * lats * lons, GL_UNSIGNED_INT, nullptr);
    sphereVAO.release();
    singleColorProgram->release();

    glDisable(GL_DEPTH_TEST);
    QPainter qp;
    qp.begin(this);

    if(frameworkVisibility)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        anchors.clear();
        for(int i=0; i<division*division*division; i++){
            QVector4D point(cubeVertices[i*6+0], cubeVertices[i*6+1], cubeVertices[i*6+2], 1);
            QVector4D projectedPoint = anchorProj * anchorView * point;
            QVector3D normalizedPoint(projectedPoint.x()/projectedPoint.w(), projectedPoint.y()/projectedPoint.w(), projectedPoint.z()/projectedPoint.w());
            QVector2D finialCoordinate((1+normalizedPoint.x())*width()/2, (1-normalizedPoint.y())*height()/2);
            anchors.push_back(finialCoordinate);

            if(i==anchorChosen){
                qp.setPen(QPen(Qt::white,3));
                qp.setBrush(Qt::magenta);
                QRect rect(finialCoordinate.x()-7,finialCoordinate.y()-7, 14, 14);
                qp.drawEllipse(rect);
            }else{
                qp.setPen(Qt::white);
                qp.setBrush(Qt::white);
                QRect rect(finialCoordinate.x()-3,finialCoordinate.y()-3, 6, 6);
                qp.drawEllipse(rect);
            }
        }
    }

    qp.end();
}

void MyOpenGLWidget::enterEvent(QEvent *)
{

}

void MyOpenGLWidget::leaveEvent(QEvent *)
{

}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    if(mouseMode==Press){

        int x = event->pos().x();
        int y = event->pos().y();

        cameraTheta += x - cameraOldX;
        cameraH += 0.03f*(y-cameraOldY);
        if(cameraH>1.0f) cameraH =1.0f;
        else if (cameraH<-1.0f) cameraH=-1.0f;
        cameraOldX=x; cameraOldY =y;
        update();
    }
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    // If Left is pressed
    if(event->button() == Qt::LeftButton)
   {
        mouseMode=Press;
        int x = event->pos().x();
        int y = event->pos().y();
        cameraOldX=x; cameraOldY =y;

        if(frameworkVisibility){
            bool haveHit = false;
            for(int i =0; i<division*division*division;i++){
                if(mouseHitPoint(anchors[i],event)){
                    anchorChosen = i;
                    haveHit = true;

                   mainwindow->slider_x->setValue(100*cubeVertices[6*anchorChosen]);
                   mainwindow->slider_y->setValue(100*cubeVertices[6*anchorChosen+1]);
                   mainwindow->slider_z->setValue(100*cubeVertices[6*anchorChosen+2]);

                    break;
                }
            }
            if(haveHit==false)
                anchorChosen=-1;
        }
    }
}

bool MyOpenGLWidget::mouseHitPoint(QVector2D& point, QMouseEvent *event)
{
   return fabs(event->pos().x()-point.x())<=5&& fabs(event->pos().y()-point.y())<=5;
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *event){// 鼠标释放事件
    mouseMode=Release;
}

void MyOpenGLWidget::setArrowCursor()
{
    setCursor(Qt::ArrowCursor);
    update();
}

void MyOpenGLWidget::setCrossCursor()
{
    setCursor(Qt::CrossCursor);
    update();
}


void MyOpenGLWidget::setMainWindow(MainWindow* i_mainwindow)
{
    mainwindow = i_mainwindow;
}

void MyOpenGLWidget::resetX(int x){
    if(anchorChosen!=-1 && !startAnimation)
    {
        cubeVBO.bind();
        float parameter[]={1.0f*x/100};
        cubeVBO.write(anchorChosen*6*sizeof (float), parameter, sizeof(parameter));
        cubeVertices[anchorChosen*6]=1.0f*x/100;
        cubeVBO.release();
        RenewSphere();
    }
}

void MyOpenGLWidget::resetY(int y){
    if(anchorChosen!=-1 && !startAnimation){
        cubeVBO.bind();
        float parameter[]={1.0f*y/100};
        cubeVBO.write((anchorChosen*6+1)*sizeof (float), parameter, sizeof(parameter));
        cubeVertices[anchorChosen*6+1]=1.0f*y/100;
        cubeVBO.release();
        RenewSphere();
    }
}

void MyOpenGLWidget::resetZ(int z){
    if(anchorChosen!=-1 && !startAnimation)
    {
        cubeVBO.bind();
        float parameter[]={1.0f*z/100};
        cubeVBO.write((anchorChosen*6+2)*sizeof (float), parameter, sizeof(parameter));
        cubeVertices[anchorChosen*6+2]=1.0f*z/100;
        cubeVBO.release();
        RenewSphere();
    }

}
