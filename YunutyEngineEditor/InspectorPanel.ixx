/// ���� �������� ���� ������ ���� ���ȭ�� �г�
/// ������Ʈ�� �߰��ϰų� ������ ����
/// �߰��� ������Ʈ�� ���� ������ ����
/// 2023. 08. 23. ����ȭ

#pragma once
#include <string>
#include <functional>
#include "ButtonLogicManager.h"
#include "YunutyEngine.h"
#include "imgui.h"
import ui.EditorPanel;
import htkg.InGame.StaticMeshWrapper;
export module ui.InspectorPanel;

export class InspectorPanel
	: public EditorPanel
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnUpdate(float ts) override;
	virtual void DrawPanel() override;

	void DrawNowObjectInfo();

	template<typename enumType>
	//const char* enumSymbols[]
	//typename std::enable_if<std::is_enum<enumType>::value>::type
	void DrawEnumClass(enumType& data, const vector<const char*>& symbols, const std::string name);

	template<typename ComponentType>
	void DrawComponentInfo(ComponentType* instance);

	void SetTransformUI(std::string type, double& value);

	void DrawAddComponentButton();

	void DrawComponentList();

	void DrawIsActive(yunutyEngine::GameObject* nowObject);

private:
	void SetDebugFlag();
	void DrawLine(ImU32 lineColor, float thickness);
	void DrawCenterColorText(std::string text, ImVec4 color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
	
	// ����Ϸ��� Data�� ���ڿ��� �ƴ� ��쿡�� �Ű������� name ���� �־��� ��
	template<typename DataType>
	void DrawSpecificData(DataType& data, const std::string name = std::string());

	template<typename ComponentType>
	void DeleteComponent(ComponentType* instance, float cursorY);

	void DeleteMapInComPonent(yunutyEngine::Component* instance);

	const std::string GetFileNameInFolder(std::string buttonName);

	const std::string GetFileNameByKey(yunutyEngine::GameObject* object);

	void SetStaticMesh(StaticMeshWrapper* instance);

	void CopyStaticMesh(StaticMeshWrapper* instance, float xinterval=0.0f, float zInterval=0.0f);

public:
	static bool isAddComponetClick;
	static std::unordered_map<yunutyEngine::GameObject*, bool> isAcitvateList;
	static std::unordered_map<yunutyEngine::GameObject*, ButtonLogicManager::eButtonType> buttonTypeList;
	static std::unordered_map<yunutyEngine::GameObject*, std::string> imageList;
	static std::unordered_map<yunutyEngine::GameObject*, std::string> stringList;

private:
	bool m_DebugFlag = false;

	std::vector<bool> m_enumFlagList;
	unsigned int m_enumButtonIndex = 0;

	/// � ������ �ұ� �����
	std::vector<Component> m_ComponentList;

	char inputStringBuffer[256];

	ButtonLogicManager* m_pButtonLogicManager = nullptr;

	bool m_imageListPopUp = false;

	bool m_meshListPopUp = false;

	bool m_DiffuseListPopUp = false;

	bool m_NormalListPopUp = false;

	bool m_UITextPopUp = false;

};

// �Ʒ��� ������ �� �̷��� ����غ� �������� ����
//template<typename ComponentType>
//void DrawComponentInfo(ComponentType* instance)
//{
//	// ���õ���� ���
//}