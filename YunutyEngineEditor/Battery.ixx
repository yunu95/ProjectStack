module;
#include "YunutyEngine.h"
export module htkg.InGame.Battery;
import htkg.InGame.IButtonInteractable;
import htkg.ResourcesID;

export class Robot;
export class Button;
export class Battery : public yunutyEngine::Component, public IButtonInteractable
{
public:
    int remainingBattery{ 0 };
    graphics::UIText* energyLeftText{ nullptr };
    Battery()
    {
        auto staticMeshComp = GetGameObject()->AddComponent<graphics::StaticMesh>();

        staticMeshComp->GetTransform()->scale = Vector3d(0.023, 0.023, 0.023);
        staticMeshComp->GetTransform()->rotation = Vector3d(-90, 0, 0);
        staticMeshComp->GetGI().LoadMesh(resourcesID::batteryASE.c_str());
        staticMeshComp->GetGI().LoadDiffuseMap(resourcesID::batteryDiffuseTex.c_str());
        staticMeshComp->GetGI().LoadNormalMap(resourcesID::batteryNormalTex.c_str());
        staticMeshComp->GetGI().SetPickingMode(false);

        energyLeftText = GetGameObject()->GetParentGameObject()->AddGameObject()->AddComponent<graphics::UIText>();
        energyLeftText->GetTransform()->scale = Vector3d::one * 0.25;
        energyLeftText->GetTransform()->position = Vector3d{ 0.5,1,0 };
        energyLeftText->GetTransform()->SetWorldRotation(Vector3d{ 90,0,0 });
        energyLeftText->GetGI().SetWorldSpace();
    }
    virtual ~Battery()
    {
    }
    virtual void Update()
    {
        energyLeftText->GetGI().SetText((TCHAR*)(L"%d"), remainingBattery);
    }
    virtual void Interact(Robot* invoker, Button* relatedButton)override;
};