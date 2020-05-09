#ifndef TRIANGLEBASE_H
#define TRIANGLEBASE_H
#include "angleBase.h"
#include "QString"
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

class edge
{
public:
    edge()
    {
        id="";
        isKnow=0;
        Length=0;
        startID="";
        endID="";
    }
    edge(QString ID,QString startP,QString endP,double LTH,bool IK=0)
    {
        id=ID;
        isKnow=IK;
        startID=startP;
        endID=endP;
        Length=LTH;
    }
    //编号
    QString id;
    //是否已知
    bool isKnow;
    //起始点
    QString startID;
    //终点
    QString endID;
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

class triangleBase
{
public:
    triangleBase();
    //读取文件数据
    void readdata(QString data);
    //已知两点求方位角，第一个参数为起始点，第二个为终点
    void get_coordinate(point &A,point &B,angle &a1,angle &a2);
    dir_angle get_dirangle(point &A,point &B);
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
    dir_angle *dir_ang;

};

#endif // TRIANGLEBASE_H
