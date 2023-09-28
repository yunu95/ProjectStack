module;
#include <cstdlib>
#include "ButtonLogicManager.h"
#include "YunutyEngine.h"
#include "DebugManager.h"
module htkg.ButtonLogicManager;
import htkg.InGame.Stage;
import htkg.ResourcesID;
import htkg.SaveLoad;

ButtonLogicManager* ButtonLogicManager::instance{ nullptr };
ButtonLogicManager::ButtonLogicManager()
{
	instance = this;
	SetLogicMap();
}

ButtonLogicManager::~ButtonLogicManager()
{

}

void ButtonLogicManager::UiCanvasLogic(std::string activeCanvas)
{
	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		yunutyEngine::SoundSystem::PlayMusic(resourcesID::music_main_bgm);
		if (obj->getName() == "Canvas")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == activeCanvas)
				{
					child->SetSelfActive(true);
				}
				else
				{
					child->SetSelfActive(false);
				}
			}
		}
		else if (obj->getName() != "Editor Camera")
		{
			obj->SetSelfActive(false);
		}
	}
}

void ButtonLogicManager::OpenMainMenu()
{
	UiCanvasLogic("Main Menu Canvas");
}

// Main Menu
void ButtonLogicManager::GameStartButton()
{
	UiCanvasLogic("Stage Select Canvas");
}

void ButtonLogicManager::CreditButton()
{
	UiCanvasLogic("Credit Canvas");
}

void ButtonLogicManager::GameQuitButton()
{
	DebugManager::GetInstance()->m_isPlayMode = false;
	Stage::LoadNextStage(0);
	UiCanvasLogic("Title Canvas");
}

// Stage Select
void ButtonLogicManager::PlayButton()
{
	yunutyEngine::SoundSystem::PlayMusic(resourcesID::music_ingame_bgm);
	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		if (obj->getName() == "InGame")
		{
			obj->SetSelfActive(true);
		}

		else if (obj->getName() == "Canvas")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == "Ingame UI Canvas")
				{
					child->SetSelfActive(true);
				}
				else
				{
					child->SetSelfActive(false);
				}
			}
		}

		else if (obj->getName() != "Editor Camera")
		{
			obj->SetSelfActive(false);
		}
	}
	Stage::LoadNextStage(-1);
}

void ButtonLogicManager::StageButton()
{
	// �������� ����? UI
}

void ButtonLogicManager::BackButton()
{
	OpenMainMenu();
}

// Ingame
void ButtonLogicManager::RunButton()
{
	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		if (obj->getName() == "Ingame")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == "Play Button")
				{
					child->SetSelfActive(false);
				}
				break;
			}
		}
	}
	Stage::Run();

	// ���� ���������� �ٽ� true�� Ȱ��ȭ�� �ٲ������ / �ƴϸ� �׳� ��� ���ְ� 
}

void ButtonLogicManager::MenuButton()
{
	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		if (obj->getName() == "InGame")
		{
			auto textVec = SaveLoad::GetComponentsFromChildren<yunutyEngine::graphics::UIText>(obj);
			for (auto index : textVec)
			{
				index->SetActive(false);
			}
		}

		else if (obj->getName() == "Canvas")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == "Ingame Menu Canvas")
				{
					child->SetSelfActive(true);
				}
				else
				{
					child->SetSelfActive(false);
				}
			}
		}
	}
}

// Ingame Menu
void ButtonLogicManager::ContinueButton()
{
	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		if (obj->getName() == "InGame")
		{
			auto textVec = SaveLoad::GetComponentsFromChildren<yunutyEngine::graphics::UIText>(obj);
			for (auto index : textVec)
			{
				index->SetActive(true);
			}
		}

		if (obj->getName() == "Canvas")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == "Ingame Menu Canvas")
				{
					child->SetSelfActive(false);
				}
				else if (child->getName() == "Ingame UI Canvas")
				{
					child->SetSelfActive(true);
				}
			}
			break;
		}
	}
}

void ButtonLogicManager::RestartButton()
{
	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		if (obj->getName() == "InGame")
		{
			obj->SetSelfActive(true);

		}

		else if (obj->getName() == "Canvas")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == "Ingame UI Canvas")
				{
					child->SetSelfActive(true);
				}
				else
				{
					child->SetSelfActive(false);
				}
			}
		}
	}

	// + ����ŸƮ�� �ΰ��� �ʱ�ȭ�� �͵� �ʱ�ȭ
	Stage::Reset();
}


void ButtonLogicManager::MenuQuitButton()
{
	OpenMainMenu();
}

// Stage Clear
void ButtonLogicManager::NextStageButton()
{
	Stage::LoadNextStage(Stage::currentStage ? Stage::currentStage->m_StageIndex : -1);

	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		if (obj->getName() == "InGame")
		{
			obj->SetSelfActive(true);
		}

		else if (obj->getName() == "Canvas")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == "Ingame UI Canvas")
				{
					child->SetSelfActive(true);
				}
				else
				{
					child->SetSelfActive(false);
				}
			}
		}
	}
}

void ButtonLogicManager::StageClearLogic()
{
	// �������� Ŭ���� ���� ���� �־��ּ���
	yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_stage_success_effect);

	// �������� Ŭ����� UI �׸���
	DrawStageClearCanvas();
}

void ButtonLogicManager::StageFailLogic()
{
	yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_stage_fail_effect);
	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		if (obj->getName() == "InGame")
		{
			obj->SetSelfActive(false);
		}

		else if (obj->getName() == "Canvas")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == "Stage Fail Canvas")
				{
					child->SetSelfActive(true);
				}
				else if (child->getName() == "Ingame UI Canvas")
				{
					child->SetSelfActive(false);
				}
			}
		}
	}
}

void ButtonLogicManager::DrawStageClearCanvas()
{
	for (auto obj : yunutyEngine::Scene::getCurrentScene()->GetChildren())
	{
		if (obj->getName() == "InGame")
		{
			obj->SetSelfActive(false);
		}

		else if (obj->getName() == "Canvas")
		{
			for (auto child : obj->GetChildren())
			{
				if (child->getName() == "Stage Clear Canvas")
				{
					child->SetSelfActive(true);
				}
				else if (child->getName() == "Ingame UI Canvas")
				{
					child->SetSelfActive(false);
				}
			}
		}
	}
}

const std::unordered_map<ButtonLogicManager::eButtonType, std::function<void()>>& ButtonLogicManager::GetLogicMap()
{
	return m_buttonLogic;
}

void ButtonLogicManager::SetLogicMap()
{
	m_buttonLogic.insert(std::make_pair(eButtonType::None, nullptr));

	m_buttonLogic.insert(std::make_pair(eButtonType::GameStart, [this]() { GameStartButton(); }));
	m_buttonLogic.insert(std::make_pair(eButtonType::Credit, [this]() { CreditButton(); }));
	m_buttonLogic.insert(std::make_pair(eButtonType::GameQuit, [this]() { GameQuitButton(); }));

	m_buttonLogic.insert(std::make_pair(eButtonType::Play, [this]() { PlayButton(); }));
	m_buttonLogic.insert(std::make_pair(eButtonType::Stage, [this]() { StageButton(); }));
	m_buttonLogic.insert(std::make_pair(eButtonType::Back, [this]() { BackButton(); }));

	m_buttonLogic.insert(std::make_pair(eButtonType::Run, [this]() { RunButton(); }));
	m_buttonLogic.insert(std::make_pair(eButtonType::Menu, [this]() { MenuButton(); }));

	m_buttonLogic.insert(std::make_pair(eButtonType::Continue, [this]() { ContinueButton(); }));
	m_buttonLogic.insert(std::make_pair(eButtonType::Restart, [this]() { RestartButton(); }));
	m_buttonLogic.insert(std::make_pair(eButtonType::MenuQuit, [this]() { MenuQuitButton(); }));

	m_buttonLogic.insert(std::make_pair(eButtonType::NextStage, [this]() { NextStageButton(); }));
	m_buttonLogic.insert(std::make_pair(eButtonType::StageClear, [this]() { DrawStageClearCanvas(); }));

}
