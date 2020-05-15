#include <QFile>
#include <QMessageBox>
#include "mainwindow.h"
#include <cmath>
#include <QTextStream>
#include <qdebug.h>
#include <QList>
#define PI 3.1415926
#define p0 (180*3600/PI)
triangleBase::triangleBase()
{
    all_point_num=0;
    know_edge_num=0;
    know_point_num=0;
    know_dirangel_num=0;
    observe_angle_num=0;
    alpha=0;
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
    know_e=new edge[know_edge_num];
    know_dir_ang=new dir_angle[know_dirangel_num];
    ang=new angle[observe_angle_num];
    L0=Matrix(observe_angle_num,1);
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
        know_e[i].isKnow=1;
        know_e[i].startID=list.at(0);
        know_e[i].endID=list.at(1);
        know_e[i].Length=list.at(2).toDouble();
    }
    for(int i=0;i<know_dirangel_num;i++)
    {
        list=ss.readLine().split(QRegExp("\\s+|,+"));
        know_dir_ang[i].startID=list.at(0);
        know_dir_ang[i].endID=list.at(1);
        know_dir_ang[i].degree=list.at(2).toDouble();
        know_dir_ang[i].minute=list.at(3).toDouble();
        know_dir_ang[i].second=list.at(4).toDouble();
        know_dir_ang[i].isKnow=true;
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
        L0.set(i,0,angleBase(ang[i].degree,ang[i].minute,ang[i].second).toDeg());
        list.clear();
    }
    //找到中间点
    int *count=new int[all_point_num];
    for (int i=0;i<all_point_num;i++)count[i]=0;
    for(int i=0;i<observe_angle_num;i++)
    {
        for(int j=0;j<all_point_num;j++)
        {
            if(ang[i].middleID==p[j].id)
            {
                count[j]++;
                break;
            }
        }
    }
    int max=count[0];
    for(int i=0;i<all_point_num;i++)
    {
        if(max<count[i])
        {
            max=count[i];
            middlepoint_id=i;
        }
    }
    //释放内存
    delete [] count;
}

void triangleBase::get_coordinate()
{
    //此函数只能计算与方位角增加相反方向的坐标，并且是按照逆时针顺序计算估值坐标，除了中间点，其余所有未知点都以中间点为一个已知点得出
    point A,B;
    angleBase C,D;
    X0=Matrix(2*(all_point_num-know_point_num),1);
    //先计算中间点坐标
    //判断中间点是否已知
    if(p[middlepoint_id].isKnow==false)
    {
        for(int i=0;i<all_point_num;i++)
        {
            //寻找已知点
            if(p[i].isKnow==true)
            {
                for(int j=0;j<observe_angle_num;j++)
                {
                    //判断夹角的中点是否为当前已知点，并且终点是否为中间点
                    if(ang[j].middleID==p[i].id&&ang[j].endID==p[middlepoint_id].id)
                    {
                        for(int k=0;k<all_point_num;k++)
                         //判断夹角起始点是否是已知点，否则会找出两个夹角
                        if(ang[j].startID==p[k].id&&p[k].isKnow==true)
                        {
                            //当前已知点赋给A，当前夹角赋值给C
                            A=p[i];
                            C.degree=ang[j].degree;
                            C.minute=ang[j].minute;
                            C.second=ang[j].second;
                        }
                    }
                    if(ang[j].middleID==p[i].id&&ang[j].startID==p[middlepoint_id].id)
                    {
                        for(int k=0;k<all_point_num;k++)
                        if(ang[j].endID==p[k].id&&p[k].isKnow==true)
                        {
                            B=p[i];
                            D.degree=ang[j].degree;
                            D.minute=ang[j].minute;
                            D.second=ang[j].second;
                        }
                    }
                }
            }
        }
        //qDebug()<<B.id<<" "<<A.id<<" "<<QString::number(C.toDeg())<<" "<<QString::number(D.toDeg());
        //p[middlepoint_id].isKnow=true;
        p[middlepoint_id].x=(B.x*sin(D.toReg())*cos(C.toReg())+A.x*cos(D.toReg())*sin(C.toReg())+(A.y-B.y)*sin(D.toReg())*sin(C.toReg()))/(sin(D.toReg())*cos(C.toReg())+cos(D.toReg())*sin(C.toReg()));
        p[middlepoint_id].y=(B.y*sin(D.toReg())*cos(C.toReg())+A.y*cos(D.toReg())*sin(C.toReg())+(B.x-A.x)*sin(D.toReg())*sin(C.toReg()))/(sin(D.toReg())*cos(C.toReg())+cos(D.toReg())*sin(C.toReg()));
        X0.set(2*(middlepoint_id-know_point_num),0,p[middlepoint_id].x);
        X0.set(2*(middlepoint_id-know_point_num)+1,0,p[middlepoint_id].y);
        //qDebug()<<p[middlepoint_id].x;
    }
    //计算其他点坐标
    for(int i=0;i<all_point_num;i++)
    {
        //判断是否为未知点
        if(p[i].isKnow==false&&p[i].id!=p[middlepoint_id].id)
        {
            for(int j=0;j<observe_angle_num;j++)
            {
                //判断夹角的起始点是否是中间点，夹角的终点是否为要求的待定点
                if(ang[j].startID==p[middlepoint_id].id&&ang[j].endID==p[i].id)
                {
                        C.degree=ang[j].degree;
                        C.minute=ang[j].minute;
                        C.second=ang[j].second;
                        //寻找起始夹角对应的已知点
                        for(int k=0;k<all_point_num;k++)
                        {
                            if(p[k].id==ang[j].middleID)
                                A=p[k];
                        }
                }
                if(ang[j].middleID==p[middlepoint_id].id&&ang[j].startID==p[i].id)
                {
                        D.degree=ang[j].degree;
                        D.minute=ang[j].minute;
                        D.second=ang[j].second;
                        for(int k=0;k<all_point_num;k++)
                        {
                            if(p[k].id==ang[j].middleID)
                                B=p[k];
                        }
                }
            }
            //qDebug()<<B.id<<" "<<A.id<<" "<<QString::number(C.toDeg())<<" "<<QString::number(D.toDeg());
            //p[i].isKnow=true;
            p[i].x=(B.x*sin(D.toReg())*cos(C.toReg())+A.x*cos(D.toReg())*sin(C.toReg())+(A.y-B.y)*sin(D.toReg())*sin(C.toReg()))/(sin(D.toReg())*cos(C.toReg())+cos(D.toReg())*sin(C.toReg()));
            p[i].y=(B.y*sin(D.toReg())*cos(C.toReg())+A.y*cos(D.toReg())*sin(C.toReg())+(B.x-A.x)*sin(D.toReg())*sin(C.toReg()))/(sin(D.toReg())*cos(C.toReg())+cos(D.toReg())*sin(C.toReg()));
            X0.set(2*(i-know_point_num),0,p[i].x);
            X0.set(2*(i-know_point_num)+1,0,p[i].y);
        }
        //qDebug()<<p[i].id<<" "<<QString("%1").arg(p[i].x,0,'f',8)<<" "<<QString("%1").arg(p[i].y,0,'f',8);
        //qDebug()<<p[i].isKnow;
       }
}

void triangleBase::get_edgeinf()
{
    auto get_dirangle=[](point &A,point&B)->angleBase{
    angleBase a;
    a=angleBase::RegtoengDeg(atan2((B.y-A.y),(B.x-A.x)));   //搭配atan2使用更佳，如果使用atan，去掉下面注释部分
    //qDebug()<<A.id<<" "<<A.x<<" "<<A.y<<" "<<'\n'<<B.id<<" "<<B.x<<" "<<B.y;
    //qDebug()<<atan2(B.y-A.y,B.x-A.x);
    /*if(A.x>B.x&&A.y<B.y)
    {
        a=a-180;
    }
    else if(A.x>B.x&&A.y>B.y)
    {
        a=a+180;
    }*/
    dir_angle temp(A.id,B.id,a);
    return temp;
    };
    angleBase an;
    point A,B;
    int i=0;
    for(int j=0;j<observe_angle_num;j++)
    {
        //找到所有边的，并给起点终点名称赋值（以中间点（不是夹角中间点，图的中间点）逆时针旋转，分别给中间点的几个夹角的
        //右边和对边赋值，一圈正好找完
        if(ang[j].middleID==p[middlepoint_id].id)
        {
            e[2*i].startID=ang[j].middleID;
            e[2*i].endID=ang[j].endID;
            e[2*i+1].startID=ang[j].endID;
            e[2*i+1].endID=ang[j].startID;
            i++;
        }
    }
    for(int i=0;i<(all_point_num-1)*2;i++)
    {
        for(int j=0;j<all_point_num;j++)
        {
            //找到起点终点信息
            if(e[i].startID==p[j].id)
            A=p[j];
            if(e[i].endID==p[j].id)
            B=p[j];
        }
        //qDebug()<<A.id<<" "<<A.x<<" "<<A.y<<" "<<'\n'<<B.id<<" "<<B.x<<" "<<B.y;
        //qDebug()<<A.id<<" "<<B.id;
        //所有边赋值长度
        e[i].Length=sqrt((B.y-A.y)*(B.y-A.y)+(B.x-A.x)*(B.x-A.x));
        //qDebug()<<e[i].Length;
        //所有边赋值方位角
        an=get_dirangle(A,B);
        e[i].degree=an.degree;
        e[i].minute=an.minute;
        e[i].second=an.second;
        //qDebug()<<QString::number(e[i].degree);
    }
}

void triangleBase::get_B_P_l()
{
    B=Matrix(observe_angle_num,2*(all_point_num-know_point_num));
    P=Matrix(observe_angle_num);
    l=Matrix(observe_angle_num,1);
    x=Matrix((all_point_num-know_point_num)*2,1);
    C=Matrix(know_dirangel_num+know_edge_num,2*(all_point_num-know_point_num));
    Wx=Matrix(know_dirangel_num+know_edge_num,1);
    X=Matrix((all_point_num-know_point_num)*2,1);
    V=Matrix(observe_angle_num,1);
    L=Matrix(observe_angle_num,1);
    point S,M,E;
    angleBase appro_angle;
    double a,b,c,d,appro_lenth;
    int sid,mid,eid;
    for (int i=0;i<observe_angle_num;i++)
    {
        //先找到夹角的三个点
        for(int j=0;j<all_point_num;j++)
        {
            if(ang[i].startID==p[j].id){S=p[j];sid=j-know_point_num;}
            else if(ang[i].middleID==p[j].id){M=p[j];mid=j-know_point_num;}
            else if(ang[i].endID==p[j].id){E=p[j];eid=j-know_point_num;}
        }
        //qDebug()<<S.isKnow<<" "<<M.isKnow<<" "<<E.isKnow;
        //
//        if(S.isKnow==true&&M.isKnow==true&&E.isKnow==true)
//        {
//           for(int j=0;j<B->getColNum();j++)
//           {
//               B->set(i,j,0);
//           }
//        }
        //计算系数值
        a=p0*(M.x-S.x)/((M.x-S.x)*(M.x-S.x)+(M.y-S.y)*(M.y-S.y));
        b=p0*(M.y-S.y)/((M.x-S.x)*(M.x-S.x)+(M.y-S.y)*(M.y-S.y));
        c=p0*(M.x-E.x)/((M.x-E.x)*(M.x-E.x)+(M.y-E.y)*(M.y-E.y));
        d=p0*(M.y-E.y)/((M.x-E.x)*(M.x-E.x)+(M.y-E.y)*(M.y-E.y));
        //qDebug()<<a<<" "<<b<<" "<<c<<" "<<d;
        //分情况
        //1.起点为已知点，其余为未知点
        //2.中间点为已知点，其余为未知点
        //3.终点为已知点，其余为未知点
        //4.起点，终点为已知点
        //5.起点，中间点为已知点
        //6.中间点，终点为已知点
        //7.都为未知点
        if(S.isKnow==true&&M.isKnow==false&&E.isKnow==false)
        {
            B.set(i,2*mid,c-a);
            B.set(i,2*mid+1,-(d-b));
            B.set(i,2*eid,-c);
            B.set(i,2*eid+1,d);
        }
        else if(S.isKnow==false&&M.isKnow==true&&E.isKnow==false)
        {
            B.set(i,2*sid,a);
            B.set(i,2*sid+1,-b);
            B.set(i,2*eid,-c);
            B.set(i,2*eid+1,d);
        }
        else if(S.isKnow==false&&M.isKnow==false&&E.isKnow==true)
        {
            B.set(i,2*sid,a);
            B.set(i,2*sid+1,-b);
            B.set(i,2*mid,c-a);
            B.set(i,2*mid+1,-(d-b));
        }
        else if(S.isKnow==true&&M.isKnow==true&&E.isKnow==false)
        {
            B.set(i,2*eid,-c);
            B.set(i,2*eid+1,d);
        }
        if(S.isKnow==true&&M.isKnow==false&&E.isKnow==true)
        {
            B.set(i,2*mid,c-a);
            B.set(i,2*mid+1,-(d-b));
        }
        else if(S.isKnow==false&&M.isKnow==true&&E.isKnow==true)
        {
            B.set(i,2*sid,a);
            B.set(i,2*sid+1,-b);
            //qDebug()<<i<<" "<<sid;
        }
        else if(S.isKnow==false&&M.isKnow==false&&E.isKnow==false)
        {
            B.set(i,2*sid,a);
            B.set(i,2*sid+1,-b);
            B.set(i,2*mid,c-a);
            B.set(i,2*mid+1,-(d-b));
            B.set(i,2*eid,-c);
            B.set(i,2*eid+1,d);
            //qDebug()<<i<<" "<<S.isKnow<<" "<<M.isKnow<<" "<<E.isKnow;
        }
    }
    //for(int i=0;i<B,getRowNum();i++)
        //for(int j=0;j<B.getColNum();j++)
           //qDebug()<<B.get(17,j);
    angleBase left,right;
    for(int i=0;i<observe_angle_num;i++)
    {
        for(int j=0;j<(all_point_num-1)*2;j++)
        {
            //夹角中间点为邻近边的起点
            if(ang[i].middleID==e[j].startID)
            {
                //夹角的起点或终点是邻边的终点（找寻以夹角中点为起点的边）
                //如果邻边的终点是夹角的起点，则为左边
                if(ang[i].startID==e[j].endID)
                {
                    left.degree=e[j].degree;
                    left.minute=e[j].minute;
                    left.second=e[j].second;
                    //qDebug()<<e[j].endID;
                }
                //如果邻近边的终点是夹角的终点，则为右边
                if(ang[i].endID==e[j].endID)
                {
                    right.degree=e[j].degree;
                    right.minute=e[j].minute;
                    right.second=e[j].second;
                    //qDebug()<<e[j].endID;
                }
            }
            //夹角中间点为邻边的终点
            if(ang[i].middleID==e[j].endID)
            {
                //夹角的起点或终点是邻边的起点点（找寻以夹角中点为终点的边）
                //如果邻边的起点是夹角的起点，则为左边
                if(ang[i].startID==e[j].startID)
                {
                    left.degree=e[j].degree;
                    left.minute=e[j].minute;
                    left.second=e[j].second;
                    //此处减去180是把夹角中间点变为起点，计算夹角是直接右角减左角
                    //这里减180很关键，如果结果为负值，直接加上360，对方位角无影响
                    //如果再下面加上180，会出现一些麻烦，具体见下方注释
                    left=left-180;
                    //qDebug()<<e[j].startID;
                }
                //如果邻边的起点点是夹角的终点，则为右边
                if(ang[i].endID==e[j].startID)
                {
                    right.degree=e[j].degree;
                    right.minute=e[j].minute;
                    right.second=e[j].second;
                    right=right-180;
                   //qDebug()<<e[j].startID;
                }
            }
        }
        //如果在这里加上180，会出现一些与上方代码逻辑不符的情况
        //假如夹角的中间点为邻边的起点，则邻边为左边时，实际参与计算的左边方位角差了180°
        //假如夹角的中间点为邻边的终点时，则邻边为右边时，实际参与计算的右边方位角差了180°
        //所以需要在上方代码中对上面两种情况分别做处理，并且还需判断是否属于上面两种情况
        //如果在不考虑上面情况，直接把两个邻边都已夹角中间点为起点，则只需考虑夹角的中间点是否为终点
        //如果是终点，则只需把相应邻边的方位角180即可，下方计算也只需要右角减左角即可
        //注释这么多只是提醒以后的自己，其实两种写法差别并不大，代码量相同，只是第二种更好理解一点
        angleBase temp=right-left;
        //qDebug()<<left.degree<<" "<<left.minute<<" "<<left.second<<" "<<right.degree<<" "<<right.minute<<" "<<right.second;
        //qDebug()<<temp.degree<<" "<<temp.minute<<" "<<temp.second;
        if(temp.minute==ang[i].minute)
        {
            l.set(i,0,ang[i].second-temp.second);
            //qDebug()<<ang[i].second<<" "<<temp.second;
        }
        //应对59°59'59''-60°00'00'的情况
        if(temp.minute!=ang[i].minute&&temp.second>ang[i].second)
        {
            l.set(i,0,ang[i].second-(temp.second+60));
        }
        //应对60°00'00'-59°59'59''的情况
        if(temp.minute!=ang[i].minute&&temp.second<ang[i].second)
        {
            l.set(i,0,(ang[i].second+60)-temp.second);
        }
        //qDebug()<<QString::number(l.get(i,0));
    }
    //构建C矩阵和Wx矩阵
    for(int i=0;i<know_edge_num;i++)
    {
        for(int j=0;j<(all_point_num-1)*2;j++)
        {
            //找到已知边对应的估计值边信息
            if((know_e[i].startID==e[j].startID&&know_e[i].endID==e[j].endID)||(know_e[i].endID==e[j].startID&&know_e[i].startID==e[j].endID))
            {
                for(int k=0;k<all_point_num;k++)
                {
                    if(know_e[i].startID==p[k].id){sid=k-know_point_num;S=p[k];}
                    else if(know_e[i].endID==p[k].id){eid=k-know_point_num;E=p[k];}
                }
                //计算系数
                a=(S.x-E.x)/(sqrt((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y)));
                b=(S.y-E.y)/(sqrt((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y)));
                appro_lenth=e[j].Length;
                //qDebug()<<e[j].startID<<" "<<e[j].endID<<" "<<e[j].Length;
            }
            /*else if(know_e[i].endID==e[j].startID&&know_e[i].startID==e[j].endID)
            {
                for(int k=0;k<all_point_num;k++)
                {
                    if(know_e[i].startID==p[k].id){sid=k-know_point_num;S=p[k];}
                    else if(know_e[i].endID==p[k].id){eid=k-know_point_num;E=p[k];}
                }
                a=(S.x-E.x)/(sqrt((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y)));
                b=(S.y-E.y)/(sqrt((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y)));
                appro_lenth=e[j].Length;
                //qDebug()<<e[j].startID<<" "<<e[j].endID<<" "<<e[j].Length;
            }*/
        }
        //qDebug()<<S.id<<" "<<S.x<<" "<<E.id<<" "<<E.x;
        //三种情况
        //1.起点已知，终点未知
        //2.起点未知，终点已知
        //3.都未知
        //appro_lenth=sqrt((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y));
        //qDebug()<<appro_lenth;
        if(S.isKnow==true&&E.isKnow==false)
        {
            C.set(i,2*eid,a);
            C.set(i,2*eid+1,b);
        }
        else if(S.isKnow==false&&E.isKnow==true)
        {
            C.set(i,2*sid,-a);
            C.set(i,2*sid*2,-b);
        }
        if(S.isKnow==false&&E.isKnow==false)
        {
            C.set(i,2*sid,-a);
            C.set(i,2*sid+1,-b);
            C.set(i,2*eid,a);
            C.set(i,2*eid+1,b);
        }
        Wx.set(i,0,appro_lenth-know_e[i].Length);
        //qDebug()<<Wx.get(i,0);
    }
    for(int i=0;i<know_dirangel_num;i++)
    {
        for(int j=0;j<(all_point_num-1)*2;j++)
        {
            //找到已知边对应的估计值方位角信息
            if(know_dir_ang[i].startID==e[j].startID&&know_dir_ang[i].endID==e[j].endID)
            {
                for(int k=0;k<all_point_num;k++)
                {
                    if(know_dir_ang[i].startID==p[k].id){sid=k-know_point_num;S=p[k];}
                    else if(know_dir_ang[i].endID==p[k].id){eid=k-know_point_num;E=p[k];}
                }
                //计算系数
                a=p0*(S.x-E.x)/((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y));
                b=p0*(S.y-E.y)/((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y));
                appro_angle.degree=e[j].degree;
                appro_angle.minute=e[j].minute;
                appro_angle.second=e[j].second;
                //qDebug()<<e[j].startID<<" "<<e[j].endID<<" "<<e[j].degree;
            }
            if(know_dir_ang[i].endID==e[j].startID&&know_dir_ang[i].startID==e[j].endID)
            {
                for(int k=0;k<all_point_num;k++)
                {
                    if(know_dir_ang[i].startID==p[k].id){sid=k-know_point_num;S=p[k];}
                    else if(know_dir_ang[i].endID==p[k].id){eid=k-know_point_num;E=p[k];}
                }
                a=p0*(S.x-E.x)/((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y));
                b=p0*(S.y-E.y)/((S.x-E.x)*(S.x-E.x)+(S.y-E.y)*(S.y-E.y));
                appro_angle.degree=e[j].degree;
                appro_angle.minute=e[j].minute;
                appro_angle.second=e[j].second;
                appro_angle=appro_angle-180;
                //qDebug()<<S.id;
                //qDebug()<<e[j].startID<<" "<<e[j].endID<<" "<<e[j].degree;
            }
        }
        //三种情况
        //1.起点已知，终点未知
        //2.起点未知，终点已知
        //3.都未知
        if(S.isKnow==true&&E.isKnow==false)
        {
            C.set(i+know_edge_num,2*eid,-a);
            C.set(i+know_edge_num,2*eid+1,b);
        }
        else if(S.isKnow==false&&E.isKnow==true)
        {
            C.set(i+know_edge_num,2*sid,a);
            C.set(i+know_edge_num,2*sid*2,-b);
        }
        if(S.isKnow==false&&E.isKnow==false)
        {
            C.set(i+know_edge_num,2*sid,a);
            C.set(i+know_edge_num,2*sid+1,-b);
            C.set(i+know_edge_num,2*eid,-a);
            C.set(i+know_edge_num,2*eid+1,b);
        }
        angleBase temp=angleBase(know_dir_ang[i].degree,know_dir_ang[i].minute,know_dir_ang[i].second);
        //qDebug()<<temp.degree<<" "<<temp.minute<<" "<<temp.second;
        if(appro_angle.minute==temp.minute)
        {
            Wx.set(i+know_edge_num,0,appro_angle.second-temp.second);
            //qDebug()<<ang[i].second<<" "<<temp.second;
        }
        //应对59°59'59''-60°00'00'的情况
        if(appro_angle.minute!=temp.minute&&temp.second>ang[i].second)
        {
            Wx.set(i+know_edge_num,0,appro_angle.second-(temp.second+60));
        }
        //应对60°00'00'-59°59'59''的情况
        if(appro_angle.minute!=temp.minute&&temp.second<ang[i].second)
        {
            Wx.set(i+know_edge_num,0,(appro_angle.second+60)-temp.second);
        }
        //qDebug()<<appro_angle.degree<<" "<<appro_angle.minute<<" "<<appro_angle.second;
        //qDebug()<<Wx.get(i+know_edge_num,0);
    }
    NBB=B.Trans()*P*B;
    NCC=C*NBB.Inverse()*C.Trans();
    W=B.Trans()*P*l;
    x=(NBB.Inverse()-NBB.Inverse()*C.Trans()*NCC.Inverse()*C*NBB.Inverse())*W+NBB.Inverse()*C.Trans()*NCC.Inverse()*Wx;
    X=X0+x;
    V=B*x-l;
    L=L0+V*(1/3600.0);
    alpha=V.Trans()*P*V/(observe_angle_num-2*(all_point_num-know_point_num));
    //qDebug()<<DegtoengDeg(L.get(1,0)).second;
    //qDebug()<<DegtoengDeg(L0.get(1,0)).second;
    //qDebug()<<L.get(1,0)<<" "<<L0.get(1,0)<<" "<<V.get(1,0)/3600;
}
