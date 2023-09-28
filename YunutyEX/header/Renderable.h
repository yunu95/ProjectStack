#pragma once
#include <unordered_map>
#include <functional>
#include <concepts>
#include "Object.h"
#include "Component.h"
#include "Rect.h"
#include "YunuGraphicsInterface.h"
#include "Transform.h"

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
    class IRenderable;
}
namespace yunutyEngine::graphics
{
    class YunutyCycle;
    template <typename YunuGIType >
    class Renderable;
    class Renderer;
    class EventHandler;
    // ��ü�� �Ǵ� Renderable
    // IRenderable ��ü�� ���� ������Ʈ�� ������Ű�� ���� ���� ����� ��� �ִ�.
    template <>
    class YUNUTY_API Renderable<yunuGI::IRenderable> : public Component
    {
    public:
        virtual ~Renderable()
        {
            yunuGIWrapperMap.erase(yunuGI);
        };
        virtual void Update()override
        {
            yunuGI->SetWorldTM(GetTransform()->GetWorldTM());
        }
    protected:
        Renderable(yunuGI::IRenderable* yunuGI) : yunuGI(yunuGI)
        {
            yunuGIWrapperMap[yunuGI] = this;
        }
        virtual void OnEnable()
        {
            yunuGI->SetActive(true);
        }
        virtual void OnDisable()
        {
            yunuGI->SetActive(false);
        }
    private:
        class Impl;
        Impl* impl;
        static std::unordered_map<yunuGI::IRenderable*, Renderable<yunuGI::IRenderable>*> yunuGIWrapperMap;
        yunuGI::IRenderable* yunuGI;
        friend Renderer;
        friend EventHandler;
    };
    // Ư���� Ÿ�Կ� ���� Renderable
    // �׷��Ƚ� ��ü�� yunuGIPtr�� ��� �����鼭 ���� ������ �Ѵ�.
    template <typename YunuGIType>
    class YUNUTY_API Renderable : public Renderable<yunuGI::IRenderable>
    {
    public:
        virtual YunuGIType& GetGI() { return *instancePtr.Get(); }
        friend Renderer;
        friend EventHandler;
    private:
        class Impl;
        Impl* impl;
        yunuGI::GIPtr<YunuGIType> instancePtr;
    protected:
        // �׷��Ƚ� ��ü�� �޾� �̺�Ʈ �ý��ۿ� ����ؾߵ�. �׷��� �����ڸ� �θ��� ���۷����� �����Ǿ� �־�� �ϴµ�?
        Renderable(YunuGIType* yunuGI) : Renderable<yunuGI::IRenderable>(yunuGI)
        {
            auto castedBasePtr = static_cast<yunuGI::IRenderable*>(yunuGI);
            static_assert(std::is_base_of<yunuGI::IRenderable, YunuGIType>());
            instancePtr = yunuGI;
        }
    };
}
