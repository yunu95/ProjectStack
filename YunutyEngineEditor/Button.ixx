module;
#include "YunutyEngine.h"
export module htkg.InGame.Button;
import htkg.ResourcesID;

export class IButtonInteractable;
export class Button : public Component
{
public:
    int mechanismIndex{0};
    Button()
    {
        auto staticMeshComp = GetGameObject()->AddComponent<graphics::StaticMesh>();

        staticMeshComp->GetTransform()->scale = Vector3d(0.01, 0.01, 0.005);
        staticMeshComp->GetTransform()->rotation = Vector3d(-90, 0, 0);
        staticMeshComp->GetGI().LoadMesh(resourcesID::buttonASE.c_str());
        staticMeshComp->GetGI().LoadDiffuseMap(resourcesID::batteryDiffuseTex.c_str());
        staticMeshComp->GetGI().LoadNormalMap(resourcesID::batteryNormalTex.c_str());
        staticMeshComp->GetGI().SetPickingMode(false);
    }
    IButtonInteractable* target{ nullptr };
};