module;
#include "YunutyEngine.h"
module htkg.InGame.Battery;
import htkg.InGame;
import htkg.ResourcesID;

void Battery::Interact(Robot* invoker, Button* relatedButton)
{
    yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_button_effect);
    invoker->battery = this;
    if (remainingBattery == 0)
    {
        invoker->state = Robot::State::BeingDrained;
    }
    else
    {
        invoker->state = Robot::State::Draining;
    }
}
