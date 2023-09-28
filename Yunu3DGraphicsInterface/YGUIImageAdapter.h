#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "YunuGraphicsInterface.h"
#include "YGRenderableAdapter.h"

namespace yunuGIAdapter
{
    class UIImageAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::IUIImage
    {
    public:
        UIImageAdapter() :RenderableAdapter() { }
        virtual void SetWorldTM(const yunuGI::Matrix4x4& worldTM) { };
        virtual void SetActive(bool isActive) {};
        virtual void SetImage(LPCSTR fileName) {};
        virtual void SetScreenSpace() {}
        virtual void SetWorldSpace() {}
        virtual void SetPickingMode(bool isPickingModeOn) {}
    private:
    };
}
