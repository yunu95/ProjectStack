module;
#include "YunutyEngine.h"
#include <queue>
#include <deque>
#include "Timer.h"
//#include <functional>
export module htkg.InGame.Robot;

import htkg.ResourcesID;
import htkg.InGame.Arrow;
//import htkg.Timer;

//export class Timer;
export class Battery;
export class Tile;
// 시리얼라이제이션 불필요
export class Robot :public yunutyEngine::Component
{
public:
    enum class State
    {
        Ready,
        Walking,
        Draining,
        BeingDrained,
        Shutdown,
        Victorious,
    };
    State state{ State::Ready };
    Tile* moveCurrent{ nullptr };
    Tile* moveNext{ nullptr };
    Tile* currentTile{ nullptr };
    std::deque<Tile*> moveQueue;
    std::deque<Arrow*> arrows;
    Timer actionTimer;

    int energy{ 0 };
    Battery* battery{ nullptr };
    graphics::UIText* energyLeftText{ nullptr };

    Robot()
    {
        auto staticMeshComp = GetGameObject()->AddComponent<graphics::StaticMesh>();
        staticMeshComp->GetGI().LoadMesh(resourcesID::robotFBX.c_str());
        staticMeshComp->GetGI().LoadDiffuseMap(resourcesID::robotDiffuseTex.c_str());
        staticMeshComp->GetGI().LoadNormalMap(resourcesID::robotNormalTex.c_str());
        staticMeshComp->GetGI().SetPickingMode(false);
        energyLeftText = GetGameObject()->AddGameObject()->AddComponent<graphics::UIText>();
        energyLeftText->GetTransform()->scale = Vector3d::one * 0.25;
        energyLeftText->GetTransform()->position = Vector3d{ 0.5,0,0 };
        energyLeftText->GetTransform()->rotation = Vector3d{ 90,0,0 };
        energyLeftText->GetGI().SetWorldSpace();

        actionTimer.duration = 0.3;
        actionTimer.onUpdate = [=](float t) {this->TimerUpdateCallback(t); };
        actionTimer.onExpiration = [=]() {this->TimerExpirationCallback(); };
    }
    virtual ~Robot()
    {
        ClearArrow();
    }
    virtual void Update()
    {
        actionTimer.Update(Time::GetDeltaTime());
        energyLeftText->GetGI().SetText((TCHAR*)(L"%d"), energy);
    }
    void TimerUpdateCallback(float normT);
    void TimerExpirationCallback();
    void PreProceed();
    void Proceed();
    void PostProceed();
    void AppendArrow(Vector3d origin, Vector3d dest, double arrowWidth)
    {
        if (!arrows.empty())
            arrows.back()->SetAsTip(false);
        auto arrow = Arrow::CreateArrow(origin, dest, arrowWidth);
        arrows.push_back(arrow);
    }
    void PopArrow()
    {
        Scene::getCurrentScene()->DestroyGameObject(arrows.back()->GetGameObject());
        arrows.pop_back();
    }
    void ClearArrow()
    {
        for (auto each : arrows)
            Scene::getCurrentScene()->DestroyGameObject(each->GetGameObject());
        arrows.clear();
    }
    void HideArrow()
    {
        for (auto each : arrows)
            each->GetGameObject()->SetSelfActive(false);
    }
};