#include "YunutyEngine.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "_ComponentImpl.h"
#include "_GameObjectImpl.h"
#include <stack>

using namespace yunutyEngine;
using namespace std;


yunutyEngine::GameObject::GameObject(IGameObjectParent* parent)
{
	impl = new Impl{};
	impl->transform = AddComponent<Transform>();
	SetParent(parent);
}
yunutyEngine::GameObject::~GameObject()
{
	delete impl;
}
void yunutyEngine::GameObject::BeforeAddComponent(GUID guid)
{
	Component::Impl::addComponentDesc.gameObject = this;
	Component::Impl::addComponentDesc.guid = guid;
#if _DEBUG
	Component::Impl::addComponentDesc.addAvailable = true;
#endif
}
void yunutyEngine::GameObject::AfterAddComponent(Component* comp)
{
	comp->impl->gameObject = this;
	impl->components.insert(make_pair(comp, unique_ptr<Component>(comp)));
	impl->indexedComponents.push_back(comp);
}
void yunutyEngine::GameObject::DoThingsOnParents(function<void(GameObject*)> todo)
{
	auto node = impl->parentGameObject;
	while (node)
	{
		todo(node);
		node = node->impl->parentGameObject;
	}
}
const vector<Component*>& yunutyEngine::GameObject::GetIndexedComponents()
{
	return impl->indexedComponents;
}
void yunutyEngine::GameObject::DeleteComponent(Component* component)
{
	// 벡터에서 컴포넌트를 삭제하는데에 쓰이는 매우 추한 코드, 물론 컴포넌트 갯수가 
	// 100개를 넘어가지도 않을테니 이런 코드는 좀 비효율적이어도 된다.
	for (auto itr = impl->indexedComponents.begin(); itr != impl->indexedComponents.end(); itr++)
		if (*itr == component)
		{
			impl->indexedComponents.erase(itr);
			break;
		}

	impl->components.erase(component);
}
bool yunutyEngine::GameObject::GetActive()
{
	if (!impl->selfActive)
		return false;
	if (impl->parentGameObject)
		return impl->parentGameObject->GetActive();
	else
		return true;
}
bool yunutyEngine::GameObject::GetSelfActive()
{
	return impl->selfActive;
}
void yunutyEngine::GameObject::SetSelfActive(bool selfActive)
{
	bool activeBefore = GetActive();
	bool activeAfter;

	impl->selfActive = selfActive;

	activeAfter = GetActive();
	if (activeBefore != activeAfter)
	{
		stack<GameObject*> activeStack;
		activeStack.push(this);
		while (!activeStack.empty())
		{
			auto child = activeStack.top();
			activeStack.pop();
			for (auto each : child->impl->childrenIndexed)
				if (each->impl->selfActive)
					activeStack.push(each);

			for (auto eachComp = child->impl->components.begin(); eachComp != child->impl->components.end(); eachComp++)
				if (activeAfter)
					eachComp->first->OnEnable();
				else
					eachComp->first->OnDisable();
		}
	}

}
GameObject* yunutyEngine::GameObject::GetParentGameObject()
{
	return impl->parentGameObject;
}
const GameObject* yunutyEngine::GameObject::GetParentGameObject()const
{
	return impl->parentGameObject;
}
Scene* yunutyEngine::GameObject::GetScene()
{
	return impl->scene;
}
void yunutyEngine::GameObject::SetParent(IGameObjectParent* parent)
{
	DoThingsOnParents([](GameObject* parent) {parent->impl->childrenNum--; });
	if (impl->parent)
		parent->ReceiveChild(impl->parent->MoveChild(this));
	else
		parent->ReceiveChild(move(unique_ptr<GameObject>(this)));
	impl->parent = parent;
	impl->parentGameObject = dynamic_cast<GameObject*>(parent);
	DoThingsOnParents([](GameObject* parent) {parent->impl->childrenNum++; });
}
// 이거 복잡도 n임.
unique_ptr<yunutyEngine::GameObject> yunutyEngine::GameObject::MoveChild(GameObject* child)
{
	auto ret = move(impl->children[child]);
	auto index = GetChildIndex(child);
	impl->children.erase(child);
	impl->childIndexMap.erase(child);
	impl->childrenIndexed.erase(impl->childrenIndexed.begin() + index);
	for (int i = index; i < impl->childrenIndexed.size(); i++)
		impl->childIndexMap[impl->childrenIndexed[i]] = i;

	return ret;
}

const vector<GameObject*>& yunutyEngine::GameObject::GetChildren()const
{
	return impl->childrenIndexed;
}
void yunutyEngine::GameObject::ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child)
{
	auto ptr = child.get();
	impl->children.insert(make_pair(ptr, unique_ptr<GameObject>()));
	impl->children[ptr] = move(child);
	impl->childIndexMap.insert(make_pair(ptr, impl->childrenIndexed.size()));
	impl->childrenIndexed.push_back(ptr);
}
int yunutyEngine::GameObject::GetChildIndex(const GameObject* child)const
{
	auto found = impl->childIndexMap.find(child);
	return found == impl->childIndexMap.end() ? -1 : found->second;
}
Transform* yunutyEngine::GameObject::GetTransform()
{
	return impl->transform;
}
const Transform* yunutyEngine::GameObject::GetTransform()const
{
	return impl->transform;
}
GameObject* yunutyEngine::GameObject::AddGameObject()
{
	return impl->scene->AddGameObject(this);
}
int yunutyEngine::GameObject::GetChildIndex()const
{
	return impl->parent->GetChildIndex(this);
}
void yunutyEngine::GameObject::SetChildIndex(int index)
{
	impl->parent->SetChildIndex(this, index);
}
void yunutyEngine::GameObject::SetChildIndex(GameObject* child, int index)
{
	if (index >= impl->children.size())
		index = impl->children.size() - 1;
	if (index < 0)
		index = 0;

	if (impl->children.find(child) == impl->children.end())
		return;
	auto origin = impl->childIndexMap[child];
	if (origin == index)
		return;

	if (origin > index)
	{
		for (int i = origin; i > index; i--)
			impl->childrenIndexed[i] = impl->childrenIndexed[i - 1];
		impl->childrenIndexed[index] = child;

		for (int i = origin; i >= index; i--)
			impl->childIndexMap[impl->childrenIndexed[i]] = i;
	}
	else
	{
		for (int i = origin; i < index; i++)
			impl->childrenIndexed[i] = impl->childrenIndexed[i + 1];
		impl->childrenIndexed[index] = child;

		for (int i = origin; i <= index; i++)
			impl->childIndexMap[impl->childrenIndexed[i]] = i;
	}
}
int yunutyEngine::GameObject::GetSceneIndex()const
{
	return GetSceneIndex(this);
}
int yunutyEngine::GameObject::GetSceneIndex(const GameObject* target)
{
	if (!target->impl->cachedSceneIndex.IsDirty())
		return target->impl->cachedSceneIndex;

#if _DEBUG
	messyIndexingCalled++;
#endif
	stack<const GameObject*> objStack;
	objStack.push(target);
	while (!objStack.empty())
	{
		auto obj = objStack.top();
		if (!obj->impl->cachedSceneIndex.IsDirty())
		{
			objStack.pop();
			if (!objStack.empty())
				objStack.top()->impl->cachedSceneIndex = objStack.top()->impl->cachedSceneIndex + obj->impl->cachedSceneIndex;
			continue;
		}
		else
		{
			int childIndex = obj->impl->parent->GetChildIndex(obj);
			if (childIndex == 0)
			{
				if (obj->impl->parentGameObject)
				{
					objStack.push(obj->impl->parentGameObject);
					obj->impl->cachedSceneIndex = 1;
					continue;
				}
				// 씬에서 가장 첫번째로 배치된 게임오브젝트
				else
				{
					obj->impl->cachedSceneIndex = 0;
					continue;
				}
			}
			else
			{
				auto brother = obj->impl->parent->GetChildren()[childIndex - 1];
				obj->impl->cachedSceneIndex = brother->impl->childrenNum + 1;
				objStack.push(brother);
			}
		}
	}
	return target->impl->cachedSceneIndex;
	//int childIndex = target->parent->GetChildIndex(target);
	//if (childIndex == 0)
	//{
	//    if (target->impl->parentGameObject)
	//        return GetSceneIndexRecursive(target->impl->parentGameObject, deltaIndex + 1);
	//    else
	//        return deltaIndex;
	//}
	//else
	//{
	//    auto brother = target->parent->GetChildren()[childIndex - 1];
	//    ret += brother->GetSceneIndex() + brother->childrenNum;
	//}

	//return target->cachedSceneIndex = ret;
}
string yunutyEngine::GameObject::getName()const
{
	return impl->name;
}
void yunutyEngine::GameObject::setName(const string& name)
{
	impl->name = name;
}
void yunutyEngine::GameObject::SetCacheDirty()
{
	impl->cachedSceneIndex.SetDirty();
}
unordered_map<Component*, unique_ptr<Component>>& yunutyEngine::GameObject::RefComponents()
{
	return impl->components;
}
vector<Component*>& yunutyEngine::GameObject::RefIndexedComponents()
{
	return impl->indexedComponents;
}

#if _DEBUG
int yunutyEngine::GameObject::messyIndexingCalled = 0;
#endif
