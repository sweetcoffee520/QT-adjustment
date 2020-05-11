#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionopen_triggered()
{
    QString filepath=QFileDialog::getOpenFileName(this,QStringLiteral("打开文件"),"","*.txt");
    QFile infile(filepath);
    if(!infile.open(QIODevice::ReadOnly|QIODevice::Text)){QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("文件未打开"));
        return ;
    }
    //QString filecontent=infile.readAll();
    QString filecontent=QString(infile.readAll());
    base.readdata(filecontent);
    base.get_coordinate();
    base.get_edgeinf();
    base.get_B_P_l();
    infile.close();
}
