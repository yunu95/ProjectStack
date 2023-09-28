#pragma once
#include "YunuGraphicsInterface.h"
#include "Camera.h"

struct yunutyEngine::graphics::Camera::Impl
{
	yunuGI::GIPtr<yunuGI::ICamera> yunuGICamera;
	static Camera* mainCamera;
};
