#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"
#include "YGRenderableAdapter.h"
#include "IYunuGIDebugCube.h"

namespace yunuGIAdapter
{
    class DebugCubeAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::IDebugCube
    {
    public:
        DebugCubeAdapter() :RenderableAdapter() { }
        virtual void SetWorldTM(const yunuGI::Matrix4x4& worldTM) override{ };
        virtual void SetActive(bool isActive) override{ };
        virtual void SetFillModeSolid() override{ };
        virtual void SetFillModeWireframe() override{ };
        virtual void SetColor(const yunuGI::Color& color) override{};
    private:
    };
}
