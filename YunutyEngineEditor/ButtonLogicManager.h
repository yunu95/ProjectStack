#pragma once
#include <unordered_map>
#include <functional>
#include <initializer_list>
#include "YunutyEngine.h"

/// ��� ���� �� ��ư�� ���� enum Ŭ����, ��ư ������ ����ִ� Ŭ����
/// 2023.08.23 ����ȭ

class ButtonLogicManager
{
public:
	enum class eButtonType
	{
		None,
		GameStart, Credit, GameQuit,
		Play, Stage, Back,
		Run, Menu,
		Continue, Restart, MenuQuit,
		NextStage,
		StageClear
	};

public:
	static ButtonLogicManager* instance;
	ButtonLogicManager();
	~ButtonLogicManager();

public:
	const std::unordered_map<eButtonType, std::function<void()>>& GetLogicMap();
	void OpenMainMenu();

	void UiCanvasLogic(std::string activeCanvas);

	// Main Menu
	void GameStartButton();
	void CreditButton();
	void GameQuitButton();

	// Stage Select
	void PlayButton();
	// int�� �������� ��ܰ����� ������ ��� ��
	void StageButton();
	void BackButton();

	// Ingame
	void RunButton();
	void MenuButton();

	// Ingame Menu
	void ContinueButton();
	void RestartButton();
	void MenuQuitButton();

	// Stage Clear
	void NextStageButton();
	// restart ��ư�� Ingame Menu�� �ִ� ���� �����ؼ� ����ص� �� �� ����

	void StageClearLogic();

	void StageFailLogic();

	void DrawStageClearCanvas();

private:
	void SetLogicMap();

	std::unordered_map<eButtonType, std::function<void()>> m_buttonLogic;
};

// template <ButtonLogic::eButtonType ButtonType>
// void ButtonLogic::ButtonClick()
// {
// 	if constexpr (ButtonType == ButtonLogic::eButtonType::eGameStart)
// 	{
// 		int a = 5;
// 	}
// }

