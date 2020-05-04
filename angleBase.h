#ifndef ANGLEBASE_H
#define ANGLEBASE_H
#include <iostream>
#include <QTextStream>
#include <sstream>
class angleBase
{
public:
    angleBase();
    //度分秒输入
    angleBase(int d, int m, int s);
    //拷贝
    angleBase(const angleBase &m);
    //小数输入，flag为0是工程角度，为1是弧度，默认为0
    angleBase(double &m,bool flag=0);
    angleBase operator=(const angleBase &m);
    angleBase operator-(const angleBase &m);
    angleBase operator+(const angleBase &m);
    angleBase operator-(int m);
    angleBase operator+(int m);
    QString show_angle();
    friend std::ostringstream& operator<< (std::ostringstream &os, const angleBase &m);
    friend QTextStream& operator<<(QTextStream &os, const angleBase &m);
    double toReg();   //工程角度转化成弧度
    double toDeg();   //工程角度化成数学角度
    int degree;
    int minute;
    double second;
};
//数学角度转弧度
double DegtoReg(double degree);
//弧度转数学角度
double RegtoDeg(double reg);
//数学角度转工程角度
angleBase DegtoengDeg(double m);
//弧度转工程角度
angleBase RegtoengDeg(double m);
#endif // ANGLEBASE_H

