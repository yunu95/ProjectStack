module;
#include "imgui.h"
import graphics.GameViewCamera;
module ui.EditorLayer;

using namespace graphics;

void EditorLayer::OnAttach()
{
	/// �г� ������ Ȯ���ϰ�, ���ʴ�� �г��� �����ϰ� �ʱ�ȭ ��
	panelList.reserve((size_t)Panel_List::COUNT);
	for (size_t i = 0; i < (size_t)Panel_List::COUNT * PANEL_LIST_INTERVAL; i += PANEL_LIST_INTERVAL)
	{
		panelList.push_back(EditorPanel::CreatePanel((Panel_List)i));
		panelList[i / PANEL_LIST_INTERVAL]->Init();
	}
}

void EditorLayer::OnDetach()
{
	for (auto& each : panelList)
	{
		each->Shutdown();
	}

	for (auto& each : cameraList)
	{
		each->Shutdown();
	}

	cameraList.clear();
	panelList.clear();
}

void EditorLayer::OnUpdate(float ts)
{
	for (auto& each : cameraList)
	{
		each->OnUpdate(ts);
	}

	for (auto& each : panelList)
	{
		each->OnUpdate(ts);
	}
}

void EditorLayer::OnUIRender()
{
	for (auto& each : panelList)
	{
		each->DrawPanel();
	}

	ImGui::ShowDemoWindow();
}
