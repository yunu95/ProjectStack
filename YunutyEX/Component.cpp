#include "YunutyEngine.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
#include "_ComponentImpl.h"

using namespace yunutyEngine;

Component::Impl::AddComponentDesc Component::Impl::addComponentDesc =
Component::Impl::AddComponentDesc
{
#if _DEBUG
	.addAvailable = false,
#endif
	.gameObject = nullptr,
	.guid = 0,
};
std::unordered_map<GUID, Component*, yutility::GenericHash<GUID>, yutility::GenericEqual<GUID>> Component::Impl::guidPtrMap;

Component::Component()
{
#if _DEBUG
	if (Impl::addComponentDesc.addAvailable != true)
	{
		_wassert(_CRT_WIDE("derived class of component class can only be created through the \"AddComponent\" function, which is the member function of gameobject class instance."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}
	Impl::addComponentDesc.addAvailable = false;
#endif
	impl->gameObject = Impl::addComponentDesc.gameObject;
	if (Impl::addComponentDesc.guid == GUID_NULL)
		UuidCreate(&Impl::addComponentDesc.guid);
	impl->guid = Impl::addComponentDesc.guid;
	Impl::guidPtrMap[impl->guid] = this;
}
Component::~Component()
{
	Impl::guidPtrMap.erase(impl->guid);
}
yunutyEngine::GameObject* yunutyEngine::Component::GetGameObject()
{
	return impl->gameObject;
}
const yunutyEngine::GameObject* yunutyEngine::Component::GetGameObject()const
{
	return impl->gameObject;
}
Transform* yunutyEngine::Component::GetTransform()
{
	return impl->gameObject->GetTransform();
}
const Transform* yunutyEngine::Component::GetTransform()const
{
	return impl->gameObject->GetTransform();
}
bool yunutyEngine::Component::GetActive()
{
	return impl->isActive;
}
GUID yunutyEngine::Component::GetGUID()
{
	return impl->guid;
}
std::wstring yunutyEngine::Component::GetGUIDWStr()
{
	wchar_t guidStr[40];
	StringFromGUID2(impl->guid, guidStr, sizeof(guidStr) / sizeof(wchar_t));
	return std::wstring(guidStr);
}
void yunutyEngine::Component::SetActive(bool active)
{
	if (impl->isActive && !active)
		OnDisable();
	if (!impl->isActive && active)
		OnEnable();

	impl->isActive = active;
}
yunutyEngine::Component* yunutyEngine::Component::FindComponent(UUID guid)
{
	if (auto found = Impl::guidPtrMap.find(guid); found != Impl::guidPtrMap.end())
		return found->second;
	else
		return nullptr;
}
