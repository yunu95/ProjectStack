#pragma once
#include <vector>
#include <string>
#include "IUnknown.h"

// ���ҽ� �Ŵ����� �ؽ�ó, �޽ÿ� ���� �׷��Ƚ� ���ҽ����� ��������� �ҷ����ų� �Ҵ������ϰ� ���� �� ����Ѵ�.
namespace yunuGI
{
    struct IResourceManagerDesc
    {

    };
    class IResourceManager : public IUnknown
    {
    public:
        virtual std::vector<std::string> GetMeshKeys() const abstract;
        virtual std::vector<std::string> GetSkeletonKeys()const abstract;
        virtual std::vector<std::string> GetTextureKeys()const abstract;
        virtual std::vector<std::string> AnimationKeys()const abstract;
        virtual void LoadFile(const char* filePath) = 0;
        virtual void UnloadResources() = 0;
    };
}

