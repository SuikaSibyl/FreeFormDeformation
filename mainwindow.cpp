#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniUI()
{
    fLabCurFile = new QLabel;
    fLabCurFile->setMinimumWidth(150);
    fLabCurFile->setText("Lab1");
    ui->statusbar->addWidget(fLabCurFile);

    progressBar1=new QProgressBar;
    progressBar1->setMaximumWidth(200);
    progressBar1->setMinimum(5);
    progressBar1->setMaximum(50);
    progressBar1->setValue(ui->textEdit->font().pointSize());
    ui->statusbar->addWidget(progressBar1);

    ui->toolBar->addSeparator();
    slider_x = new QSlider;
    slider_x->setOrientation(Qt::Horizontal);  // 水平方向
    slider_x->setMinimum(-150);
    slider_x->setMaximum(150);
    slider_x->setValue(0);
    slider_x->setMinimumWidth(10);
    slider_x->setMaximumWidth(200);
    ui->toolBar->addWidget(new QLabel("Slide X axis   "));
    ui->toolBar->addWidget(slider_x);
    connect(slider_x,SIGNAL(valueChanged(int)),this,SLOT(on_actSlide_X(int)));

    ui->toolBar->addSeparator();
    slider_y = new QSlider;
    slider_y->setOrientation(Qt::Horizontal);  // 水平方向
    slider_y->setMinimum(-150);
    slider_y->setMaximum(150);
    slider_y->setValue(0);
    slider_y->setMinimumWidth(10);
    slider_y->setMaximumWidth(200);
    ui->toolBar->addWidget(new QLabel("Slide Y axis   "));
    ui->toolBar->addWidget(slider_y);
    connect(slider_y,SIGNAL(valueChanged(int)),this,SLOT(on_actSlide_Y(int)));

    ui->toolBar->addSeparator();
    slider_z = new QSlider;
    slider_z->setOrientation(Qt::Horizontal);  // 水平方向
    slider_z->setMinimum(-150);
    slider_z->setMaximum(150);
    slider_z->setValue(0);
    slider_z->setMinimumWidth(10);
    slider_z->setMaximumWidth(200);
    ui->toolBar->addWidget(new QLabel("Slide Z axis   "));
    ui->toolBar->addWidget(slider_z);
    connect(slider_z,SIGNAL(valueChanged(int)),this,SLOT(on_actSlide_Z(int)));

    setCentralWidget(ui->openGLWidget);
    ui->openGLWidget->setMouseTracking(true);
    ui->openGLWidget->setMainWindow(this);
}

void MainWindow::ddebug(){
    qDebug()<<"wtf";
    slider_x->setValue(50);
//    qDebug()<<slider_x;
//    slider_x->maximum();
//    qDebug()<<"wtf"<<slider_x->maximum();
}

void MainWindow::on_actStart_triggered()
{
    ui->openGLWidget->animationToggle();
}

void MainWindow::on_actStop_triggered()
{
    ui->openGLWidget->animationToggle();
}

void MainWindow::on_actSlide_X(int x)
{
    ui->openGLWidget->resetX(x);
}

void MainWindow::on_actSlide_Y(int y)
{
    ui->openGLWidget->resetY(y);
}

void MainWindow::on_actSlide_Z(int z)
{
    ui->openGLWidget->resetZ(z);
}


void MainWindow::on_actEye_triggered()
{
    ui->openGLWidget->visibilityToggle();
}
