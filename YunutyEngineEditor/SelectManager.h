#pragma once
#include "YunutyEngine.h"

/// ���� ���õ� ������Ʈ�� ���������� �����ϱ� ���� Ŭ����
/// ��𼭵� ���� ���õ� ������Ʈ�� ���������� �����ϱ� ���� �̱������� ����
/// 2023.08.23 ����ȭ

class SelectManager
{
private:
	SelectManager();
	~SelectManager();

	static SelectManager* instance;


public:
	static SelectManager* GetInstance();
	static void Delete();

public:
	yunutyEngine::GameObject*& GetRecentHierarchy();
	void SetRecentHierarchy(yunutyEngine::GameObject* obj);

	const set<yunutyEngine::GameObject*>& GetHierarchyListSet();
	void InsertHierarchyList(yunutyEngine::GameObject* obj);
	void DeleteHierarchyList(yunutyEngine::GameObject* obj);

	void InitializeHierarchy();

	yunutyEngine::Vector3d& GetNowRotation();

	void SetNowRotation(yunutyEngine::GameObject* obj);

private:
	set<yunutyEngine::GameObject*> m_HierarchyListSet;
	yunutyEngine::GameObject* m_recentHierarchy;

	yunutyEngine::Vector3d m_nowRotation;
};


