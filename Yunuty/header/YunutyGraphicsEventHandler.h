#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <concepts>
#include "Object.h"
#include "Component.h"
#include "Rect.h"
#include "YunuGraphicsInterface.h"

// ī�޶�� ȭ�鿡 ���ӻ� �����ϴ� ��� �׷��� ��ҵ��� ����ϴ� ���� ���̴� ��ü�Դϴ�. 
// ī�޶��� ������ ������ ����ȯ��,Ȥ�� ī�޶��� Ư���� ���� �޶��� �� �ֱ� ������,
// ȭ���� �׷����� ���� ���̴� �Լ� Render�� �߻� �޼ҵ�� ���ǵ˴ϴ�.
#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunuGI
{
    class IUnknown;
}
namespace yunutyEngine::graphics
{
    class Renderer;
    class Rendererable;
    class YunutyCycle;
    class YUNUTY_API EventHandler : public Component
    {
    public:
        EventHandler()
        {
            if (auto itr = eventHandlers.find(GetGameObject()); itr == eventHandlers.end())
                eventHandlers[GetGameObject()] = std::set<EventHandler*>{};

            eventHandlers[GetGameObject()].insert(this);
        }
        ~EventHandler()
        {
            eventHandlers[GetGameObject()].erase(this);

            if (eventHandlers[GetGameObject()].empty())
                eventHandlers.erase(GetGameObject());
        }
        void SetClickCallback(std::function<void()> clickCallback) { onClick = clickCallback; }
        void SetHoverCallback(std::function<void()> hoverCallback) { onHover = hoverCallback; }
        void SetTargetGraphic(Rendererable* target) {
        }
    protected:
    private:
        static const set<EventHandler*>* GetEventHandlers(yunuGI::IRenderable*);
        static void CallEvents(yunuGI::IRenderable* renderable, function<void(EventHandler*)> todo);
        static std::unordered_map<GameObject*, std::set<EventHandler*>> eventHandlers;
        Rendererable* target;
        std::function<void()> onClick = []() {};
        std::function<void()> onHover = []() {};
        friend Renderer;
    };
}
