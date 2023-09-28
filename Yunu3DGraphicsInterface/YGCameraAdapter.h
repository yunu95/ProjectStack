#pragma once
#include "IYunuGICamera.h"

namespace yunuGIAdapter
{
    class CameraAdapter : public yunuGI::ICamera
    {
    public:
        CameraAdapter() {};
        virtual ~CameraAdapter() {};
        virtual void SetWorldTM(const yunuGI::Matrix4x4& tm) override { };
        virtual void SetFOV(float fov) override {};
        virtual void SetNear(float near) override {};
        virtual void SetFar(float far) override {};
        virtual void SetAsMain() override {};
    };
}
