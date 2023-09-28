/// ���� �������� ���� ������ ���� ���ȭ�� �г�
/// ������Ʈ �߰�, ����, ������Ʈ �̸��� ���� ������ ����
/// ���콺 �巡�׸� ���� ���� �� ���� ������Ʈ�� ���� ���� ������ �ٲٴ� ���� ����
/// 2023. 08. 23. ����ȭ

module;
#include <vector>
#include "YunutyEngine.h"
#include "imgui_internal.h"
export import ui.EditorPanel;
export module ui.HierarchyPanel;

export class HierarchyPanel
	: public EditorPanel
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnUpdate(float ts) override;
	virtual void DrawPanel() override;

private:
	void CreateOneTreeNode(yunutyEngine::GameObject* root);
	
	void RightClickPopUp(GameObject* obj);

	void ClickLogic(GameObject* obj);

	void LeftClickSelected(GameObject* obj);

	void DragLogic(GameObject* obj);

	void TextNearMouse(std::string text);

	void RightClick();

	void CalculateBlank();

	void DeleteObject(GameObject* obj);

private:
	int m_NamingNum = 0;

	bool m_popUpFlag = false;

	bool m_setNameFlag = false;

	bool m_rightFlag = false;

	bool m_isDragging = false;

	yunutyEngine::GameObject* m_dragTarget;

	float m_prevYcursor = 0.0f;

	std::vector<yunutyEngine::GameObject*> m_ObjectList;

	std::vector<int> m_IndexList;

	char inputStringBuffer[256];
};
