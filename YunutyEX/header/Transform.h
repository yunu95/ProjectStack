#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "YunuGIMatrix4x4.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunutyEngine
{
    class GameObject;
    class YUNUTY_API Transform :
        public Component
    {
    public:
        // �� �Ʒ��� position, rotation, scale �� ���� �������� ĳ���� ���� Setter�� ���� �������� �մϴ�.
        Vector3d position = Vector3d();
        Quaternion rotation;
        Vector3d scale = Vector3d::one;
        Vector3d GetWorldPosition()const;
        Quaternion GetWorldRotation()const;
        Vector3d GetWorldScale()const;
        yunuGI::Matrix4x4 GetLocalTM()const;
        yunuGI::Matrix4x4 GetWorldTM()const;
        void SetWorldPosition(const Vector3d& position);
        void SetWorldRotation(const Quaternion& rotation);
    private:
        void SetCacheDirty();
    };
}
