#pragma once
#include "Component.h"

using namespace yunutyEngine;

struct yunutyEngine::Component::Impl
{
	struct AddComponentDesc
	{
#if _DEBUG
		bool addAvailable;
#endif
		GameObject* gameObject;
		GUID guid;
	};
private:
	static AddComponentDesc addComponentDesc;
	static std::unordered_map<GUID, Component*, yutility::GenericHash<GUID>, yutility::GenericEqual<GUID>> guidPtrMap;
	GameObject* gameObject = nullptr;
	bool StartCalled = false;
	bool isActive{ true };
	GUID guid;

	friend Component;
	friend GameObject;
	friend YunutyCycle;
	friend Collider2D;
	friend Collider;
};
