module;
#include "YunutyEngine.h"
#include "Timer.h"
module htkg.InGame.Robot;
import htkg.InGame.Tile;

void Robot::TimerUpdateCallback(float normT)
{
    switch (state)
    {
    case Robot::State::Ready:
        break;
    case Robot::State::Walking:
        GetTransform()->SetWorldPosition
        (Vector3d::Lerp(moveCurrent->GetTransform()->GetWorldPosition(), moveNext->GetTransform()->GetWorldPosition(), 1 - (1 - normT) * (1 - normT)));
        break;
    case Robot::State::Draining:
        break;
    case Robot::State::BeingDrained:
        break;
    default:
        break;
    }
}
void Robot::TimerExpirationCallback()
{
    if (currentTile->tileType == Tile::TileType::Destination)
        state = State::Victorious;
    if (currentTile->_isDangerous)
    {
        yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_robot_oring_effect);
        energy = 0;
        state = State::Shutdown;
        return;
    }
    switch (state)
    {
    case Robot::State::Ready:
        break;
    case Robot::State::Walking:
        state = Robot::State::Ready;
        if (currentTile->_button)
        {
            currentTile->_button->target->Interact(this, currentTile->_button);
        }
        energy--;
        break;
    case Robot::State::Draining:
        break;
    case Robot::State::BeingDrained:
        break;
    default:
        break;
    }
}
void Robot::PreProceed()
{
    switch (state)
    {
    case Robot::State::Ready:
        break;
    case Robot::State::Walking:
        break;
    case Robot::State::Draining:
        break;
    case Robot::State::BeingDrained:
        break;
    default:
        break;
    }
}
void Robot::Proceed()
{
    switch (state)
    {
    case Robot::State::Ready:
        if (energy == 0 && currentTile->tileType != Tile::TileType::Destination)
        {
            state = Robot::State::Shutdown;
            break;
        }

        if (!moveQueue.empty())
            while (moveQueue.front() == currentTile)
                moveQueue.pop_front();

        if (!moveQueue.empty())
        {
            yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_robot_move_effect);
            moveCurrent = currentTile;
            moveNext = moveQueue.front();
            currentTile = moveNext;
            moveQueue.pop_front();
            state = State::Walking;
        }
        else
        {
        }
        return;


        break;
    case Robot::State::Walking:
        break;
    case Robot::State::Draining:
        if (battery->remainingBattery > 0)
        {
            yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_robot_charge_effect);
            battery->remainingBattery--;
            energy++;
        }
        else
        {
            yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_robot_click_effect);
            state = State::Ready;
        }
        break;
    case Robot::State::BeingDrained:
        if (energy > 0)
        {
            yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_robot_down_effect);
            battery->remainingBattery++;
            energy--;
        }
        else
        {
            yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_robot_oring_effect);
            state = State::Shutdown;
        }
        break;
    default:
        break;
    }
    actionTimer.Start();
}
void Robot::PostProceed()
{

    switch (state)
    {
    case Robot::State::Ready:
        break;
    case Robot::State::Walking:
        break;
    case Robot::State::Draining:
        break;
    case Robot::State::BeingDrained:
        break;
    default:
        break;
    }
}

