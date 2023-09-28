#pragma once
#include "YunuGIMatrix4x4.h"
#include "IUnknown.h"

namespace yunuGI
{
    class ICamera : public IUnknown
    {
    public:
        // ī�޶��� ���� ��ġ�� ������ view matrix�� �����Ѵ�.
        virtual void SetWorldTM(const Matrix4x4& tm) = 0;
        // ī�޶��� �þ߰��� �����Ѵ�.
        virtual void SetFOV(float fov) = 0;
        // ī�޶� ����ü�� near �÷��α����� �Ÿ��� �����Ѵ�.
        virtual void SetNear(float near) = 0;
        // ī�޶� ����ü�� far �÷��α����� �Ÿ��� �����Ѵ�.
        virtual void SetFar(float far) = 0;
        // ī�޶� ���� ī�޶�� �����.
        virtual void SetAsMain() = 0;
    };
    struct ICameraDesc
    {
    };
}
