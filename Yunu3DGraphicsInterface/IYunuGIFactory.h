#pragma once
#include "IYunuGIStaticMesh.h"
#include "IYunuGISkinnedMesh.h"
#include "IYunuGIDirectionalLight.h"
#include "IYunuGICamera.h"
#include "IYunuGIDebugCube.h"
#include "IYunuGIUIImage.h"
#include "IYunuGIUIText.h"
#include "IUnknown.h"

// ���丮 ��ü�� ī�޶�, �޽��� ���� �������� �ʿ��� ��ü���� �����Ѵ�.
namespace yunuGI
{
    struct IFactoryDesc
    {
    };
    class IFactory : public yunuGI::IUnknown
    {
    public:
        virtual yunuGI::ICamera* CreateCamera(const yunuGI::ICameraDesc& desc) = 0;
        virtual yunuGI::IStaticMesh* CreateStaticMesh(const yunuGI::IStaticMeshDesc& desc) = 0;
        virtual yunuGI::ISkinnedMesh* CreateSkinnedMesh(const yunuGI::ISkinnedMeshDesc& desc)=0;
        virtual yunuGI::IDebugCube* CreateDebugCube(const yunuGI::IDebugCubeDesc& desc) = 0;
        virtual yunuGI::IUIImage* CreateUIImage(const yunuGI::IUIImageDesc& desc) = 0;
        virtual yunuGI::IUIText* CreateUIText(const yunuGI::IUITextDesc& desc) = 0;
        virtual yunuGI::IDirectionalLight* CreateDirectionalLight(const yunuGI::IDirectionalLightDesc& desc) = 0;
    };
}
