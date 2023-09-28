module;
#include "Component.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "YunutyEngine.h"
#include "Timer.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <functional>
#include "ButtonLogicManager.h"
module htkg.InGame.Stage;

import htkg.InGame.Battery;
import htkg.InGame.Button;
import htkg.InGame.Laser;
import htkg.InGame.Robot;
import htkg.InGame.Tile;
import htkg.ResourcesID;
import htkg.SaveLoad;
import htkg.ButtonLogicManager;
import ui.DebugPanel;

Stage* Stage::currentStage{ nullptr };
void Stage::LoadNextStage(int currentStageIndex)
{
    std::map<int, Stage*, std::less<>> stages;
    for (auto stage : SaveLoad::GetComponentsFromChildren<Stage>(Scene::getCurrentScene()))
    {
        stages[stage->m_StageIndex] = stage;
        stage->GetGameObject()->SetSelfActive(false);
    }
    auto itr = stages.upper_bound(currentStageIndex);
    Stage::currentStage = itr == stages.end() ? nullptr : itr->second;

    if (Stage::currentStage == nullptr)
        ButtonLogicManager::instance->GetLogicMap().at(ButtonLogicManager::eButtonType::MenuQuit)();
    else
        Stage::currentStage->ResetStage();
}
void Stage::SetDangerZone()
{
    SetAllTileSafe();
    for (auto tile_col : tiles)
        for (auto tile : tile_col)
            if (tile->tileType == Tile::TileType::Laser && tile->_laser->active)
                SetDangerZone(tile);
}
void Stage::Proceed()
{
    SetDangerZone();
    bool stillAlive = false;
    for (auto robot : robots)
        stillAlive |= robot->state != Robot::State::Shutdown;

    if (stillAlive == false)
        ButtonLogicManager::instance->StageFailLogic();

    for (auto robot : robots)
        if (robot->state == Robot::State::Victorious)
            ButtonLogicManager::instance->StageClearLogic();

    for (auto robot : robots)
        robot->PreProceed();
    for (auto robot : robots)
        robot->Proceed();
    for (auto robot : robots)
        robot->PostProceed();

    if (!isPlaying)
        playingTimer.isActive = false;
}
void Stage::ResetStage()
{
    yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_turn_effect);
    isPlaying = false;
    GetGameObject()->SetSelfActive(true);
    constexpr double perTile = 8.5 / 5.0;
    graphics::Camera::GetMainCamera()->GetTransform()->SetWorldPosition(GetCenterPosition() + Vector3d::up * perTile * fmax(m_goalHeightSize, m_goalWidthSize) - Vector3d::forward * m_goalWidthSize * 0.2);
    graphics::Camera::GetMainCamera()->GetTransform()->SetWorldRotation(Quaternion::MakeWithForwardUp(Vector3d::down, Vector3d::forward));
    robots.clear();
    buttons.clear();
    lasers.clear();
    batteries.clear();
    for (auto tile_col : tiles)
        for (auto tile : tile_col)
            ResetTile(tile);
    for (auto tile_col : tiles)
        for (auto tile : tile_col)
            switch (tile->tileType)
            {
            case Tile::TileType::Robot:
                robots.push_back(tile->_robot);
                break;
            case Tile::TileType::Button:
                buttons.push_back(tile->_button);
                tile->_button->mechanismIndex = tile->mechanismIndex;
                //buttons[tile->mechanismIndex] = tile->_button;
                break;
            case Tile::TileType::Laser:
                lasers[tile->mechanismIndex] = tile->_laser;
                break;
            case Tile::TileType::Battery:
                batteries[tile->mechanismIndex] = tile->_battery;
                break;
            default:
                break;
            }
    for (auto each : buttons)
    {
        if (lasers.find(each->mechanismIndex) != lasers.end())
            each->target = lasers[each->mechanismIndex];
        if (batteries.find(each->mechanismIndex) != batteries.end())
            each->target = batteries[each->mechanismIndex];
    }
}

