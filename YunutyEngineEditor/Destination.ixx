module;
#include "YunutyEngine.h"
export module htkg.InGame.Destination;
import htkg.ResourcesID;

export class Destination : public Component
{
public:
    Destination()
    {
        auto staticMeshComp = GetGameObject()->AddComponent<graphics::StaticMesh>();
        staticMeshComp->GetTransform()->position = Vector3d(0, 0.265, 0.0);
        staticMeshComp->GetTransform()->scale = Vector3d(0.012, 0.012, 0.012);
        staticMeshComp->GetTransform()->rotation = Vector3d(90, 0, 0.000);

        staticMeshComp->GetGI().LoadMesh(resourcesID::destinationASE.c_str());
        staticMeshComp->GetGI().LoadDiffuseMap(resourcesID::destinationDiffuseTex.c_str());
        staticMeshComp->GetGI().LoadNormalMap(resourcesID::destinationNormalTex.c_str());
    }
};