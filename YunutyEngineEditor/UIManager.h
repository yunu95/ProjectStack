#pragma once
#include <functional>
#include "ButtonLogicManager.h"
#include "YunutyEngine.h"

/// �ڵ� �󿡼� UI�� ���� Image�� Button�� ���鶧 �Լ� �ϳ��� ���ϰ� ����� ���� Ŭ����
/// 2023.08.23 ����ȭ

class UIManager
{
private:
	UIManager();
	~UIManager();

	static UIManager* instance;

public:
	static UIManager* GetInstance();
	static void Delete();

public:
	void CreateImageButton(float posX, float posY, float sclX, float sclY, bool isAcitvate = true, std::string objectName = "", yunutyEngine::GameObject* parentObject = nullptr, std::string imageSource = "Textures\\red.dds", ButtonLogicManager::eButtonType buttonType = ButtonLogicManager::eButtonType::None);
	void CreateImage(float posX, float posY, float sclX, float sclY, bool isAcitvate = true, std::string objectName = "", yunutyEngine::GameObject* parentObject = nullptr, std::string imageSource = "Textures\\red.dds");
	void CreateTextButton(float posX, float posY, float sclX, float sclY, bool isAcitvate = true, std::string objectName = "", yunutyEngine::GameObject* parentObject = nullptr, TCHAR* text =(TCHAR*)L"NULL", ButtonLogicManager::eButtonType buttonType = ButtonLogicManager::eButtonType::None);

	ButtonLogicManager* m_pButtonManager;
};

