module;
#include "YunutyEngine.h"
export module htkg.InGame.Laser;
import htkg.InGame.IButtonInteractable;
import htkg.ResourcesID;

export class Robot;
export class Button;
export struct Laser : public yunutyEngine::Component, public IButtonInteractable
{
    graphics::StaticMesh* laserMesh{ nullptr };
    Laser()
    {
        auto staticMeshComp = GetGameObject()->AddGameObject()->AddComponent<graphics::StaticMesh>();
        staticMeshComp->GetTransform()->scale = Vector3d(0.010, 0.010, 0.010);
        staticMeshComp->GetTransform()->rotation = Vector3d(90, 270,180.000);
        staticMeshComp->GetGI().LoadMesh(resourcesID::laserASE.c_str());

        staticMeshComp->GetGI().LoadDiffuseMap(resourcesID::laserDiffuseTex.c_str());
        staticMeshComp->GetGI().LoadNormalMap(resourcesID::laserNormalTex.c_str());
        staticMeshComp->GetGI().SetPickingMode(false);

        laserMesh = staticMeshComp->GetGameObject()->AddGameObject()->AddComponent<graphics::StaticMesh>();
        laserMesh->GetTransform()->position = Vector3d{ 30,0,50 };
        laserMesh->GetTransform()->scale = Vector3d{ 0.3,0.3,20 };
        laserMesh->GetTransform()->rotation = Vector3d{ 0,90,0 };
        laserMesh->GetGI().LoadMesh(resourcesID::laserBeamFBX.c_str());
        laserMesh->GetGI().LoadDiffuseMap(resourcesID::redTexture.c_str());
        laserMesh->GetGI().LoadNormalMap(resourcesID::laserNormalTex.c_str());
    }
    virtual ~Laser()
    {
    }
    // 레이저 그래픽만 끄고, 위험지역 판정은 그냥 Stage에서 알아서 처리하게 한다.
    virtual void Interact(Robot* invoker, Button* relatedButton);
    bool active;
};