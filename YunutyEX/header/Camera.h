#pragma once
#include "Object.h"
#include "Component.h"
#include "Rect.h"

// ī�޶�� ȭ�鿡 ���ӻ� �����ϴ� ��� �׷��� ��ҵ��� ����ϴ� ���� ���̴� ��ü�Դϴ�. 
// ī�޶��� ������ ������ ����ȯ��,Ȥ�� ī�޶��� Ư���� ���� �޶��� �� �ֱ� ������,
// ȭ���� �׷����� ���� ���̴� �Լ� Render�� �߻� �޼ҵ�� ���ǵ˴ϴ�.
#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunutyEngine::graphics
{
    class YunutyCycle;
    class YUNUTY_API Camera : public Component
    {
    private:
        struct Impl;
        Impl* impl;
    protected:
        virtual void Update() override;
    public:
        Camera();
        virtual ~Camera();
        void SetCameraMain();
        static Camera* GetMainCamera();
        friend YunutyCycle;
    };
}
