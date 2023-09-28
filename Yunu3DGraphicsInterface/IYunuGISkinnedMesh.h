#pragma once
#include "IYunuGIRenderable.h"
#include "YunuGIMatrix4x4.h"
#include "IUnknown.h"

namespace yunuGI
{
    class ISkinnedMesh : virtual public yunuGI::IRenderable
    {
    public:
        virtual void LoadMesh(const char* fileName) abstract;
        virtual void LoadDiffuseMap(const char* fileName)abstract;
        virtual void LoadNormalMap(const char* fileName)abstract;
        virtual void SetPickingMode(bool isPickingModeOn)abstract;
        // �� ������ �ֵ�
        virtual void SetAnimation(const char* animID) {};
        virtual void SetAnimationRepeat(bool repeat) {};
        virtual void PlayAnimation() {};
    };

    struct ISkinnedMeshDesc
    {
    };
}
