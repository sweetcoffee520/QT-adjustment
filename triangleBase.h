#ifndef TRIANGLEBASE_H
#define TRIANGLEBASE_H
#include "angleBase.h"
#include "QString"
#include "Matrix.h"
//前置声明
class point
{
public:
    point()
    {
        id="";
        isKnow=0;
        x=0;
        y=0;
    }
    point(const point &P)
    {
        id=P.id;
        isKnow=P.isKnow;
        x=P.x;
        y=P.y;
    }
    point(QString Id,double X,double Y,bool IK=0)
    {
        id=Id;
        isKnow=IK;
        x=X;
        y=Y;
    }
    //点号
    QString id;
    //是否已知
    bool isKnow;
    double x;
    double y;
};

//方位角
class dir_angle:public angleBase
{
public:
    dir_angle():isKnow(0){};
    dir_angle(QString sP,QString eP,angleBase a,bool IK=0):angleBase(a),isKnow(IK),startID(sP),endID(eP){};
    bool isKnow;
    QString startID;
    QString endID;
};

class edge:public dir_angle
{
public:
    edge()
    {
        LisKnow=0;
        Length=0;
    }
    edge(QString startP,QString endP,double LTH=0,bool IK=0)
    {
        LisKnow=IK;
        startID=startP;
        endID=endP;
        Length=LTH;
    }
    //是否已知
    bool LisKnow;
    //长度
    double Length;
};

//夹角
class angle:public angleBase
{
public:
    angle():id(0){};
    angle(angleBase p, int ID, QString sP, QString mP, QString eP):angleBase(p),id(ID),startID(sP),middleID(mP),endID(eP){};
    //角号
    int id;
    //
    QString startID;
    QString middleID;
    QString endID;
};

class triangleBase
{
public:
    triangleBase();
    //读取文件数据
    void readdata(QString data);
    //已知两点求方位角，第一个参数为起始点，第二个为终点
    void get_edgeinf();
    //前方交会求坐标
    void get_coordinate();
    //计算方位角系数
    void get_B_P_l();
    //总点数
    int all_point_num;
    //已知点数
    int know_point_num;
    //已知边数
    int know_edge_num;
    //已知方位角个数
    int know_dirangel_num;
    //观测角数
    int observe_angle_num;
    point *p;
    edge *e;
    angle *ang;
    //已知边
    edge *know_e;
    //已知方位角
    dir_angle *know_dir_ang;
    int middlepoint_id=0;
    Matrix B,P,l,x,C,W,Wx,NBB,NCC;
};

#endif // TRIANGLEBASE_H
