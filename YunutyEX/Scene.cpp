#include "YunutyEngine.h"
#include "Scene.h"
#include "GameObject.h"
#include "_GameObjectImpl.h"

using namespace yunutyEngine;


struct yunutyEngine::Scene::Impl
{
	string name;
	unordered_set<GameObject*> gameObjects;
	unordered_map<GameObject*, unique_ptr<GameObject>> children;
	vector<GameObject*> childrenIndexed;
	set<GameObject*> destroyList;
	unordered_map<const GameObject*, int> childIndexMap;
};
yunutyEngine::Scene::Scene() :impl(new Scene::Impl{})
{
}
yunutyEngine::Scene::~Scene()
{
	delete impl;
}


yunutyEngine::Scene* yunutyEngine::Scene::currentScene{ nullptr };
yunutyEngine::Scene* yunutyEngine::Scene::getCurrentScene()
{
	return currentScene;
}
void yunutyEngine::Scene::LoadScene(Scene* scene)
{
	currentScene = scene;
}
string yunutyEngine::Scene::getName()const
{
	return impl->name;
}
void yunutyEngine::Scene::setName(const string& name)
{
	this->impl->name = name;
}
yunutyEngine::GameObject* yunutyEngine::Scene::AddGameObject(IGameObjectParent* parent)
{
	auto gameObject = new GameObject(parent != nullptr ? parent : this);
	gameObject->impl->scene = this;
	/*if (!parent)
		gameObjects.insert(make_pair<gameObject, unique_ptr<GameObject>(gameObject));*/

	return gameObject;
}
yunutyEngine::GameObject* yunutyEngine::Scene::AddGameObject(string name, IGameObjectParent* parent)
{
	auto gameObject = AddGameObject(parent);
	gameObject->impl->name = name;
	return gameObject;
}
set<GameObject*>* yunutyEngine::Scene::GetDestroyList()
{
	return &impl->destroyList;
}
unique_ptr<yunutyEngine::GameObject> yunutyEngine::Scene::MoveChild(GameObject* child)
{
	unique_ptr<yunutyEngine::GameObject> ret = move(impl->children[child]);
	impl->children.erase(child);
	auto erasedIndex = impl->childIndexMap[child];
	impl->childIndexMap.erase(child);
	impl->childrenIndexed.erase(impl->childrenIndexed.begin() + erasedIndex);
	for (auto& each : impl->childIndexMap)
		if (each.second > erasedIndex)
			each.second--;
	return ret;
}

const vector<yunutyEngine::GameObject*>& yunutyEngine::Scene::GetChildren() const
{
	return impl->childrenIndexed;
}
void yunutyEngine::Scene::ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child)
{
	auto ptr = child.get();
	impl->children.insert(make_pair(ptr, unique_ptr<GameObject>()));
	impl->children[ptr] = move(child);
	impl->children.emplace(ptr, move(child));
	impl->childIndexMap.insert(make_pair(ptr, impl->childrenIndexed.size()));
	impl->childrenIndexed.push_back(ptr);
}
int yunutyEngine::Scene::GetChildIndex(const GameObject* child)const
{
	return impl->childIndexMap.find(child)->second;
}
void yunutyEngine::Scene::SetChildIndex(GameObject* child, int index)
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
void yunutyEngine::Scene::DestroyGameObject(GameObject* gameObj)
{
	if (!gameObj)
		return;
	impl->destroyList.insert(gameObj);
}
