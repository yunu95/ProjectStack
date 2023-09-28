#pragma once
#include "IYunuGIEventHandler.h"

class IRenderableObject;

namespace yunuGIAdapter
{
    class EventHandlerAdapter : public yunuGI::IEventHandler
    {
    public:
        virtual void AddClickCallback(CallbackPtr callback) override { }
        virtual yunuGI::IRenderable* Click(float normalizedX, float normalizedY) override {}
    private:
    };
}