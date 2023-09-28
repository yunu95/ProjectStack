#include "YunutyEngine.h"
#include "Camera.h"
#include "_CameraImpl.h"
#include "_YunuGIObjects.h"
using namespace yunutyEngine::graphics;

Camera* yunutyEngine::graphics::Camera::Impl::mainCamera{ nullptr };
yunutyEngine::graphics::Camera::Camera()
	:impl(new Impl{})
{
	if (impl->mainCamera == nullptr)
		impl->mainCamera = this;
	impl->yunuGICamera = _YunuGIObjects::SingleInstance().factory->CreateCamera({});
}
yunutyEngine::graphics::Camera::~Camera()
{
	delete impl;
}
void yunutyEngine::graphics::Camera::Update()
{
	GetTransform()->scale = Vector3d::one;
	impl->yunuGICamera->SetWorldTM(GetTransform()->GetWorldTM());
}
void yunutyEngine::graphics::Camera::SetCameraMain()
{
	impl->mainCamera = this;
	impl->yunuGICamera->SetAsMain();
}
Camera* yunutyEngine::graphics::Camera::GetMainCamera()
{
	return Impl::mainCamera;
}
