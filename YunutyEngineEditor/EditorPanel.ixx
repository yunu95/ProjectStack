/// 2023. 07. 09. �����
/// �����Ϳ��� ����� �гε��� �߻�ȭ(�������̽�)
/// �ش� �г��� ��ӹ޾� ��ü���� �г��� ������

module;
#include "imgui.h"
#include "imgui_internal.h"
export module ui.EditorPanel;

export constexpr unsigned int PANEL_LIST_INTERVAL{ 100 };

export enum class Panel_List
	: unsigned char
{
	COUNT = 6,
	HIERARCHY = 0,
	INSPECTOR = 1 * PANEL_LIST_INTERVAL,
	PROJECT = 2 * PANEL_LIST_INTERVAL,
	SCENE = 3 * PANEL_LIST_INTERVAL,
	GAMEVIEW = 4 * PANEL_LIST_INTERVAL,
	DEBUG = 5 * PANEL_LIST_INTERVAL
};

export class EditorPanel
{
public:
	static EditorPanel* CreatePanel(Panel_List panel);
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void OnUpdate(float ts) = 0;
	virtual void DrawPanel() = 0;
};
