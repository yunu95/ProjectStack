#pragma once
#include "GameObject.h"

struct GameObject::Impl
{
	Scene* scene = nullptr;
	string name = "";
	bool selfActive = true;
	int childrenNum = 0;
	IGameObjectParent* parent = nullptr;
	GameObject* parentGameObject = nullptr;
	Transform* transform = nullptr;
	unordered_map<GameObject*, unique_ptr<GameObject>> children;
	vector<GameObject*> childrenIndexed;
	unordered_map<const GameObject*, int> childIndexMap;
	// sceneIndex is Updated per every cycle.
	int sceneIndex = 0;
	mutable _Cache<int> cachedSceneIndex;
	unordered_map<Component*, unique_ptr<Component>> components;
	vector<Component*> indexedComponents;
};
