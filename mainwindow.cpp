#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QDebug>
#include <QByteArray>

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
    else {
    //QString filecontent=infile.readAll();
    QString filecontent=QString(infile.readAll());
    int i;
    i=filecontent.split('\n')[0].split(QRegExp("\\s+|,+")).length();
    if(i!=5)
    {QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("文件格式不正确"));return ;}
    else
    base.readdata(filecontent);
    {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("读取成功"));}
    infile.close();
    }
}

void MainWindow::on_actionstart_adjust_triggered()
{
    if(base.know_point_num==0&&base.know_edge_num==0&&base.all_point_num==0&&base.know_dirangel_num==0&&base.observe_angle_num==0)
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未读入文件"));return ;}
    else
    {
        //qDebug()<<base.know_point_num<<base.know_edge_num<<base.all_point_num<<base.know_dirangel_num<<base.observe_angle_num;
        base.get_coordinate();
        base.get_edgeinf();
        base.get_B_P_l();
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("平差成功"));}
    }
}

void MainWindow::on_actionorigin_data_triggered()
{
    if(base.know_point_num==0&&base.know_edge_num==0&&base.all_point_num==0&&base.know_dirangel_num==0&&base.observe_angle_num==0)
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未读入文件"));return ;}
    else
    {
    QStringList list;
    this->setCentralWidget(centralWidget);
    tableWidget->setParent(centralWidget);
    horizontalLayout->addWidget(tableWidget);
    tableWidget->clear();
    tableWidget->setColumnCount(5);
    tableWidget->setRowCount(base.observe_angle_num);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("点号")<<QStringLiteral("起始点")<<QStringLiteral("中间点")<<QStringLiteral("终点")<<QStringLiteral("度分秒"));
    for (int i=0;i<base.observe_angle_num;i++)
    {
        list<<QString::number(base.ang[i].id)<<base.ang[i].startID<<base.ang[i].middleID<<base.ang[i].endID<<QString::number(base.ang[i].degree)+" "+QString::number(base.ang[i].minute)+" "+QString::number(base.ang[i].second);
        int col=0;
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(0)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(1)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(2)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i,col++,new QTableWidgetItem(list.at(3)));
        tableWidget->setItem(i,col++,new QTableWidgetItem(list.at(4)));
        list.clear();
    }
    }
}

void MainWindow::on_actionadjusted_data_triggered()
{
    if(base.know_point_num==0&&base.know_edge_num==0&&base.all_point_num==0&&base.know_dirangel_num==0&&base.observe_angle_num==0)
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未读入文件"));return ;}
    else if(base.B.getRowNum()==0)
    {
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未平差"));return ;}
    }
    QStringList list;
    this->setCentralWidget(centralWidget);
    tableWidget->setParent(centralWidget);
    horizontalLayout->addWidget(tableWidget);
    tableWidget->clear();
    tableWidget->setColumnCount(5);
    tableWidget->setRowCount(base.observe_angle_num);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("点号")<<QStringLiteral("起始点")<<QStringLiteral("中间点")<<QStringLiteral("终点")<<QStringLiteral("度分秒"));
    for (int i=0;i<base.observe_angle_num;i++)
    {
        list<<QString::number(base.ang[i].id)<<base.ang[i].startID<<base.ang[i].middleID<<base.ang[i].endID<<QString::number(DegtoengDeg(base.L.get(i,0)).degree)+" "+QString::number(DegtoengDeg(base.L.get(i,0)).minute)+" "+QString::number(DegtoengDeg(base.L.get(i,0)).second);
        int col=0;
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(0)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(1)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(2)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i,col++,new QTableWidgetItem(list.at(3)));
        tableWidget->setItem(i,col++,new QTableWidgetItem(list.at(4)));
        list.clear();
    }
}

void MainWindow::on_actioncoor_data_triggered()
{
    if(base.know_point_num==0&&base.know_edge_num==0&&base.all_point_num==0&&base.know_dirangel_num==0&&base.observe_angle_num==0)
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未读入文件"));return ;}
    else if(base.B.getRowNum()==0)
    {
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未平差"));return ;}
    }
    QStringList list;
    this->setCentralWidget(centralWidget);
    tableWidget->setParent(centralWidget);
    horizontalLayout->addWidget(tableWidget);
    tableWidget->clear();
    tableWidget->setColumnCount(3);
    tableWidget->setRowCount(base.all_point_num-base.know_point_num);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("点名")<<QStringLiteral("X")<<QStringLiteral("Y"));
    for (int i=0;i<base.all_point_num-base.know_point_num;i++)
    {
        list<<base.p[i+base.know_point_num].id<<QString("%1").arg(base.p[i+base.know_point_num].x,0,'f',8)<<QString("%1").arg(base.p[i+base.know_point_num].y,0,'f',8);
        int col=0;
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(0)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(i,col++,new QTableWidgetItem(list.at(1)));
        tableWidget->setItem(i,col++,new QTableWidgetItem(list.at(2)));
        list.clear();
    }
}

void MainWindow::on_actionclose_triggered()
{
   close();
}

void MainWindow::on_actioncoor_data_2_triggered()
{
    if(base.know_point_num==0&&base.know_edge_num==0&&base.all_point_num==0&&base.know_dirangel_num==0&&base.observe_angle_num==0)
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未读入文件"));return ;}
    else if(base.B.getRowNum()==0)
    {
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未平差"));return ;}
    }
    else
    {
        QString filename=QFileDialog::getSaveFileName(this,QStringLiteral("保存文件"),"","*.txt");
        QFile outfile(filename);
        if(!outfile.open(QIODevice::WriteOnly|QIODevice::Text)){QMessageBox::information(this,QStringLiteral("提醒"),QStringLiteral("文件未打开"));
            return ;
        }
        QString ss="";
        for (int i=0;i<base.all_point_num-base.know_point_num;i++)
        ss+=base.p[i+base.know_point_num].id+" "+QString("%1").arg(base.p[i+base.know_point_num].x,10,'f',8)+" "+QString("%1").arg(base.p[i+base.know_point_num].y,10,'f',8)+'\n';
        outfile.write(ss.toUtf8());
        outfile.close();
    }

}

void MainWindow::on_actionobserve_data_triggered()
{

    if(base.know_point_num==0&&base.know_edge_num==0&&base.all_point_num==0&&base.know_dirangel_num==0&&base.observe_angle_num==0)
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未读入文件"));return ;}
    else if(base.B.getRowNum()==0)
    {
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未平差"));return ;}
    }
    else
    {
        QString filename=QFileDialog::getSaveFileName(this,QStringLiteral("保存文件"),"","*.txt");
        QFile outfile(filename);
        if(!outfile.open(QIODevice::WriteOnly|QIODevice::Text)){QMessageBox::information(this,QStringLiteral("提醒"),QStringLiteral("文件未打开"));
            return ;
        }
        QString ss="";
        for (int i=0;i<base.observe_angle_num;i++)
        ss+=base.ang[i].startID+" "+base.ang[i].middleID+" "+base.ang[i].endID+" "+QString::number(DegtoengDeg(base.L.get(i,0)).degree)+" "+QString::number(DegtoengDeg(base.L.get(i,0)).minute)+" "+QString::number(DegtoengDeg(base.L.get(i,0)).second)+'\n';
        outfile.write(ss.toUtf8());
        outfile.close();
    }

}

void MainWindow::on_actionshow_graphic_triggered()
{

}
