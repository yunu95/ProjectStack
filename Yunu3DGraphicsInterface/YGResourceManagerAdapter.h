#pragma once
#include <vector>
#include "YunuGraphicsInterface.h"

namespace yunuGIAdapter
{
    class ResourceManagerAdapter : public yunuGI::IResourceManager
    {
    public:

        virtual std::vector<std::string> GetMeshKeys() const override { return std::vector<std::string>{}; }
        virtual std::vector<std::string> GetSkeletonKeys() const override { return std::vector<std::string>{}; }
        virtual std::vector<std::string> GetTextureKeys() const override { return std::vector<std::string>{}; }
        virtual std::vector<std::string> AnimationKeys() const override { return std::vector<std::string>{}; }
        virtual void LoadFile(const char* filePath) override
        {
        };
        virtual void UnloadResources() override
        {
        };
    };
}
