#include "angleBase.h"
#include <cmath>
#define pi 3.1415926

angleBase::angleBase()
    {
        degree = 0;
        minute = 0;
        second = 0;
    }
    angleBase::angleBase(int d, int m, double s)
    {
        d %= 360;
        degree = d;
        minute = m;
        second = s;
    }
    angleBase::angleBase(const angleBase &m)
    {
        this->degree = m.degree;
        this->minute = m.minute;
        this->second = m.second;
    }
    angleBase::angleBase(double m,bool flag)
    {
        if(flag==0)
        {
            this->degree = int(m);
            this->minute = int(m * 60 - degree * 60);
            this->second = (m * 60 - degree * 60 - minute) * 60;
        }
        else
        {
            double temp;
            temp=m * 180/pi;
            this->degree=int(temp);
            this->minute=int(m * 60 - degree * 60);
            this->second=(m * 60 - degree * 60 - minute) * 60;
        }
    }
    angleBase angleBase::operator=(const angleBase &m)
    {
        degree = m.degree;
        minute = m.minute;
        second = m.second;
        return *this;
    }
    angleBase angleBase::operator-(const angleBase &m)
    {
        angleBase res = *this;
        res.second = res.second - m.second;
        while(res.second<0)
        {
            res.second += 60;
            res.minute--;
        }
        res.minute = res.minute - m.minute;
        while(res.minute<0)
        {
            res.minute +=60;
            res.degree--;
        }
        res.degree = res.degree - m.degree;
        while (res.degree < 0) res.degree += 360;
        res.degree %= 360;
        return res;
    }
    angleBase angleBase::operator+(const angleBase &m)
    {
        angleBase res = *this;
        res.second = second + m.second;
        while (res.second>=60)
        {
            res.second -= 60;
            res.minute++;
        }
        res.minute = res.minute + m.minute;
        while(res.minute>=60)
        {
            res.minute -= 60;
            res.degree++;
        }
        res.degree = res.degree + m.degree;
        res.degree = res.degree % 360;
        return res;
    }
    angleBase angleBase::operator-(int m)
    {
        angleBase temp = *this;
        temp.degree -= m;
        while (temp.degree < 0) temp.degree += 360;
        temp.degree %= 360;
        return temp;
    }
    angleBase angleBase::operator+(int m)
    {
        angleBase temp = *this;
        temp.degree += m;
        temp.degree %= 360;
        return temp;
    }
    angleBase operator-(int m, angleBase &ang)
    {
        angleBase temp = ang;
        temp.degree = m - ang.degree;
        while (temp.degree < 0) temp.degree += 360;
        temp.degree %= 360;
        return temp;
    }
    double angleBase::toReg()
    {
        double temp;
        temp = (degree + minute / 60.0 + second / 3600.0) * 3.1415926 / 180.0;
        return temp;
    }
    double angleBase::toDeg()
    {
        double temp;
        temp = (degree + minute / 60.0 + second / 3600.0);
        return temp;
    }
    void angleBase::show_angle()
    {
        std::cout << degree << "°" << minute << "'" << second << "''";
    }
    std::ostream& operator<<(std::ostream &os,const angleBase &m)
    {
        os << m.degree << "°" << m.minute << "'" << m.second << "''"<<'\n';
        return os;
    }

    double DegtoReg(double m)
    {
        while(m>=360)
        {
            m -= 360;
        }
        while (m<0)
        {
            m += 360;
        }
        double reg = m * pi / 180.0;
        //reg=reg-int(reg/(2*pi))*pi;
        return reg;
    }

    double angleBase::RegtoDeg(double reg)
    {
        while (reg<0)
        {
            reg += 2 * pi;
        }
        while(reg>=2*pi)
        {
            reg -= 2 * pi;
        }
        double degree = reg * 180 / pi;
        return degree;
    }

    angleBase angleBase::DegtoengDeg(double m)
    {
        while(m<0)
        {
            m += 360;
        }
        m = fmod(m,360);
        angleBase temp;
        temp.degree = int(m);
        temp.minute = int(m * 60 - temp.degree * 60);
        temp.second = (m * 60 - temp.degree * 60 - temp.minute) * 60;
        return temp;
    }

    angleBase angleBase::RegtoengDeg(double reg)
    {
        while (reg<0)
        {
            reg += 2 * pi;
        }
        reg = fmod(reg,2*pi);
        angleBase temp;
        double n = RegtoDeg(reg);
        temp = DegtoengDeg(n);
        return temp;
    }
