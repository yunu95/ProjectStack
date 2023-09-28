#pragma once
#include "YunuGraphicsInterface.h"

namespace yunuGIAdapter
{
    class RendererAdapter : public yunuGI::I3DRenderer
    {
    public:
        RendererAdapter() { };
        virtual ~RendererAdapter() {}

        virtual void SetOutputWindow(UINT64 hWnd) { };
        virtual void Update(float deltaTime) { };
        virtual void Render() { };
        virtual void ResizeResolution(unsigned int width, unsigned int height) {};
        virtual void* QuerySharedOutputHandle() { return nullptr; }
    };
}
