#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->modeComboBox, SIGNAL(currentIndexChanged(int)), ui->openGLRenderWidget, SLOT(modeChanged(int)));
    connect(ui->openGLRenderWidget, SIGNAL(modeComboBoxShouldChange(int)), ui->modeComboBox, SLOT(setCurrentIndex(int)));
    connect(ui->openGLRenderWidget, SIGNAL(LineEditDisable(bool)), ui->xPosEdit, SLOT(setDisabled(bool)));
    connect(ui->openGLRenderWidget, SIGNAL(LineEditDisable(bool)), ui->yPosEdit, SLOT(setDisabled(bool)));
    connect(ui->openGLRenderWidget, SIGNAL(LineEditDisable(bool)), ui->zPosEdit, SLOT(setDisabled(bool)));

    connect(ui->openGLRenderWidget, SIGNAL(clearLineEdit()), ui->xPosEdit, SLOT(clear()));
    connect(ui->openGLRenderWidget, SIGNAL(clearLineEdit()), ui->yPosEdit, SLOT(clear()));
    connect(ui->openGLRenderWidget, SIGNAL(clearLineEdit()), ui->zPosEdit, SLOT(clear()));

    connect(ui->openGLRenderWidget, SIGNAL(setWorldCoord(QVector3D)), this, SLOT(setCoords(QVector3D)));

    connect(ui->xPosEdit, SIGNAL(returnPressed()), this, SLOT(CoordinateChanged()));
    connect(ui->yPosEdit, SIGNAL(returnPressed()), this, SLOT(CoordinateChanged()));
    connect(ui->zPosEdit, SIGNAL(returnPressed()), this, SLOT(CoordinateChanged()));

    connect(this, SIGNAL(CoordinateHasChanged(float, float, float)), ui->openGLRenderWidget, SLOT(CoordinateChanged(float, float, float)));

    connect(ui->wireModelButton, SIGNAL(clicked(bool)), ui->openGLRenderWidget, SLOT(modelWireMode(bool)));

    connect(ui->setRootBtn, SIGNAL(clicked()), ui->openGLRenderWidget, SLOT(setRootJoint()));

    connect(ui->bindButton, SIGNAL(clicked()), ui->openGLRenderWidget, SLOT(bindSkeleton()));

    QLabel* hintLabel = new QLabel(this);
    hintLabel->setText("hint messgae");
    ui->modifyGridLayout->addWidget(hintLabel, 7, 0, 2, 3);
    connect(ui->openGLRenderWidget, SIGNAL(hint(QString)), hintLabel, SLOT(setText(QString)));

    QLabel* helpLabel = new QLabel(this);
    helpLabel->setText("使用说明:\n "
                       "1.创建模式:左键选中关节，右键新建关节\n"
                       "  按 Q 停止这一条关节链的创建\n"
                       "  或者左键其他位置停止创建\n"
                       "  左键选中已有关节，可以继续创建\n"
                       "2.选择模式:可以修改关节的世界坐标\n"
                       "  可以确定基节点，基节点的移动会带动\n"
                       "  其他关节移动\n "
                       "3.点击[绑定骨架]，提示绑定完成后，\n"
                       "  可以进入运动模式，修改关节位置会\n"
                       "  带动模型节点变化, 也可以继续创建\n"
                       "  或修改关节，然后重新绑定骨架\n"
                       "4.快捷键：\n"
                       "  W/S - 摄像头的前进/后退\n"
                       "  A/D - 摄像头向左/向右\n"
                       "  L/R - 从左侧/右侧看模型\n"
                       "  F/T - 从正前/顶上看模型\n"
                       "  滚轮 - 放大缩小视角\n"
                       "  左键拽动 - 摄像头左右转\n");
    helpLabel->setMaximumWidth(300);
    ui->modifyGridLayout->addWidget(helpLabel, 10, 0, 17, 3);

}

void MainWindow::setCoords(QVector3D coords)
{
    ui->xPosEdit->setText(QString::number(coords.x()));
    ui->yPosEdit->setText(QString::number(coords.y()));
    ui->zPosEdit->setText(QString::number(coords.z()));
}

void MainWindow::CoordinateChanged(){
    float x = ui->xPosEdit->text().toFloat();
    float y = ui->yPosEdit->text().toFloat();
    float z = ui->zPosEdit->text().toFloat();
    emit CoordinateHasChanged(x, y, z);

}

MainWindow::~MainWindow()
{
    delete ui;
}
