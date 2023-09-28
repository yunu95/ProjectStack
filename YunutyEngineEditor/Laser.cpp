module;
#include "YunutyEngine.h"
module htkg.InGame.Laser;
import htkg.InGame.Robot;
import htkg.ResourcesID;
void Laser::Interact(Robot* invoker, Button* relatedButton)
{
    yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_button_effect);
    if (active)
        yunutyEngine::SoundSystem::PlaySoundfile(resourcesID::sound_laser_off_effect);
    active = false;
    laserMesh->SetActive(false);
}
