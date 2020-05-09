#include "triangleBase.h"
#include <QFile>
#include <QMessageBox>
#include "mainwindow.h"
#include <cmath>
#include <QTextStream>
#include <qdebug.h>
#include <QList>
triangleBase::triangleBase()
{

}

void triangleBase::readdata(QString data)
{
    QStringList list;
    QTextStream ss(&data);
    list=ss.readLine().split(QRegExp("\\s+|,+"));
    all_point_num=list.at(0).toInt();
    know_point_num=list.at(1).toInt();
    know_edge_num=list.at(2).toInt();
    know_dirangel_num=list.at(3).toInt();
    observe_angle_num=list.at(4).toInt();
    //ss>>all_point_num>>know_point_num>>know_edge_num>>know_edge_num>>know_dirangel_num;
    p=new point[all_point_num];
    e=new edge[(all_point_num-1)*2];
    dir_ang=new dir_angle[know_dirangel_num];
    ang=new angle[observe_angle_num];
    list.clear();
    QString ssr=ss.readLine();
    list=ssr.split(QRegExp("\\s+|,+"));
    for(int i=0;i<list.length();i++)
    {
        p[i].id=list.at(i);
    }
    list.clear();
    for(int i=0;i<know_point_num;i++)
    {
        list=ss.readLine().split(QRegExp("\\s+|,+"));
        for(int j=0;j<all_point_num;j++)
        {
            if(p[j].id==list.at(0))
            {
                p[j].x=list.at(1).toDouble();
                p[j].y=list.at(2).toDouble();
                p[j].isKnow=1;
            }
        }
        list.clear();
    }
    for(int i=0;i<know_edge_num;i++)
    {
        list=ss.readLine().split(QRegExp("\\s+|,+"));
        e[i].id=list.at(0);
        e[i].isKnow=1;
        e[i].startID=list.at(0);
        e[i].endID=list.at(1);
        e[i].Length=list.at(2).toDouble();
    }
    for(int i=0;i<know_dirangel_num;i++)
    {
        list=ss.readLine().split(QRegExp("\\s+|,+"));
        dir_ang[i].startID=list.at(0);
        dir_ang[i].endID=list.at(1);
        dir_ang[i].degree=list.at(2).toDouble();
        dir_ang[i].minute=list.at(3).toDouble();
        dir_ang[i].second=list.at(4).toDouble();
    }
    for(int i=0;i<observe_angle_num;i++)
    {
        list=ss.readLine().split(QRegExp("\\s+|,+"));
        ang[i].id=list.at(0).toInt();
        ang[i].startID=list.at(1);
        ang[i].middleID=list.at(2);
        ang[i].endID=list.at(3);
        ang[i].degree=list.at(4).toDouble();
        ang[i].minute=list.at(5).toDouble();
        ang[i].second=list.at(6).toDouble();
        list.clear();
    }
}
dir_angle triangleBase::get_dirangle(point &A,point &B)
{
    angleBase a;
    a=RegtoengDeg(atan((B.y-A.y)/(B.x-A.x)));
    if(A.x>B.x&&A.y<B.y)
    {
        a=a-180;
    }
    else if(A.x>B.x&&A.y>B.y)
    {
        a=a+180;
    }
    else if(A.x<B.x&&A.y>B.y)
    {
        a=360-a;
    }
    dir_angle temp1;
    dir_angle temp(A.id,B.id,a);
    return temp;
}
