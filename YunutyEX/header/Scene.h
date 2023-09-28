#pragma once
#include "IGameObjectParent.h"
#include <iostream>
#include "Object.h"
#include <unordered_map>
#include <unordered_set>
#include <set>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif


using namespace std;
namespace yunutyEngine
{
	class GameObject;
	class YunutyCycle;
	class YUNUTY_API Scene :
		public Object, public IGameObjectParent
	{
	public:
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene();
		virtual ~Scene();
		static Scene* currentScene;
		static Scene* getCurrentScene();
		static void LoadScene(Scene* scene);
		virtual const vector<GameObject*>& GetChildren()const override;
		string getName()const;
		void setName(const string& name);
		void DestroyGameObject(GameObject* gameObj);
		GameObject* AddGameObject(IGameObjectParent* parent = nullptr);
		GameObject* AddGameObject(string name, IGameObjectParent* parent = nullptr);
	protected:
		unique_ptr<yunutyEngine::GameObject> MoveChild(GameObject* child) override;
		void ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child)override;
		int GetChildIndex(const GameObject* child)const override;
	private:
		struct Impl;
		Impl* impl{ nullptr };
		virtual void SetChildIndex(GameObject* child, int index);
		set<GameObject*>* GetDestroyList();
		friend YunutyCycle;
	};
}
