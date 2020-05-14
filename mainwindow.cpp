#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QDebug>
#include <QByteArray>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(centralWidget);
    dialog.installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)   //用过滤器eventFilter（）拦截dialog中的QEvent::Paint事件
{
    if(watched == &dialog && event->type() == QEvent::Paint && drawflag==true)
        paint();

    return QWidget::eventFilter(watched,event);
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
    horizontalLayout->removeWidget(label);
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
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(3)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
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
    horizontalLayout->removeWidget(label);
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
        tableWidget->setItem(i,col,new QTableWidgetItem(list.at(3)));
        tableWidget->item(i,col++)->setTextAlignment(Qt::AlignCenter);
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
    horizontalLayout->removeWidget(label);
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
    if(base.know_point_num==0&&base.know_edge_num==0&&base.all_point_num==0&&base.know_dirangel_num==0&&base.observe_angle_num==0)
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未读入文件"));return ;}
    else if(base.B.getRowNum()==0)
    {
        {QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("未平差"));return ;}
    }
    else
    {
        drawflag=true;
        dialog.show();
    }
}

void MainWindow::paint()
{
    double maxx,minx,maxy,miny;
    maxx=base.p[0].x;
    maxy=base.p[0].y;
    minx=maxx;
    miny=maxy;
    for(int i=0;i<base.all_point_num;i++)
    {
        if(maxx<base.p[i].x){maxx=base.p[i].x;}
        if(minx>base.p[i].x){minx=base.p[i].x;}
        if(maxy<base.p[i].y){maxy=base.p[i].y;}
        if(miny>base.p[i].y){miny=base.p[i].y;}
    }
    QPainter painter(&dialog);
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.drawText(QRect(100,100,100,100),base.p[0].id);
    //painter.scale(1,-1);
    //投射逻辑坐标系：把最小x,最小y设为窗口原点，后面两参数为逻辑宽度。
    painter.setWindow(miny,-minx,(maxy-miny)*1.2,(maxx-minx)*1.2);
    //painter.setViewport(0,0,width(),height());
    //painter.drawText(QRect(miny+1000,-minx+1000,1000,1000),base.p[0].id);
    painter.translate(0.1*(maxy-miny)*1.2,0.9*(maxx-minx)*1.2);
    //translate相对于逻辑坐标系，而不是原始窗口坐标系
    //painter.translate(0.1*(maxx-minx),0.9*(maxy-miny));
   // painter.setViewport(0,0,2400,1500);
    //理解：坐标轴位置不变，把y轴上所有值变为相反数，包括原点坐标值(坐标轴位置不变，坐标轴上所有数乘以所给参数)
    //painter.drawText(QPointF(base.p[0].y,base.p[0].x),base.p[0].id);
    QPen pen(QColor(0,0,0),20);
    painter.setPen(pen);
    QFont font = painter.font();
    font.setPixelSize(600);
    painter.setFont(font);
    for (int i=0;i<base.all_point_num;i++)
    {
        painter.drawText(QRectF(base.p[i].y-(maxy-miny)*1.2/60,-base.p[i].x,(maxy-miny)*1.2/30,(maxy-miny)*1.2/30),Qt::AlignCenter,base.p[i].id);
    }
    pen.setColor(QColor(90,189,255));
    painter.setPen(pen);
    painter.scale(1,-1);
    QPainterPath path(QPointF(base.p[0].y,base.p[0].x));
   // painter.drawText(QRectF(miny+1000,minx+1000,1000,1000),Qt::AlignCenter,base.p[0].id);
    painter.drawLine(base.p[base.middlepoint_id].y,base.p[base.middlepoint_id].x,base.p[0].y,base.p[0].x);
    for(int i=1;i<base.all_point_num;i++)
    {
        if(base.p[i].id!=base.p[base.middlepoint_id].id)
        {
        path.lineTo(QPointF(base.p[i].y,base.p[i].x));
        painter.drawLine(QLineF(base.p[base.middlepoint_id].y,base.p[base.middlepoint_id].x,base.p[i].y,base.p[i].x));
        }
    }
    path.closeSubpath();
    painter.drawPath(path);
    painter.end();
    qDebug()<<"sdf";
}

void MainWindow::on_actionhelp_triggered()
{
    QString ss="先读入文件，然后平差，此程序只适用中心三角网";
    QMessageBox::information(this,QStringLiteral("帮助"),ss);
}
