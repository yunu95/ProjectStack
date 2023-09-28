#pragma once
#include "Vector3.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunutyEngine
{
    class YUNUTY_API Quaternion
    {
    private:
    public:
        double w = 1;
        double x = 0;
        double y = 0;
        double z = 0;
        Quaternion();
        Quaternion(double w, double x, double y, double z);
        Quaternion(const Vector3d& euler);
        Quaternion& operator=(const Vector3d& euler);
        Quaternion operator*(const Quaternion& other)const;
        Quaternion inverse()const;
        Vector3d operator*(const Vector3d& other)const;
        Vector3d Forward()const;
        Vector3d Up()const;
        Vector3d Right()const;
        Vector3d Euler()const;
        // ȸ������ forward ���͸� �����ϰ�, up ���� foward�� �����ϰ� ���� ��, right ���� �����Ͽ� ���Ѵ�. 
        static Quaternion MakeWithForwardUp(const Vector3d& foward, const Vector3d& up);
        static Quaternion MakeWithAxes(const Vector3d& right, const Vector3d& up, const Vector3d& forward);
        static Quaternion MakeWithAxes(const Vector3f& right, const Vector3f& up, const Vector3f& forward);
    };
}
