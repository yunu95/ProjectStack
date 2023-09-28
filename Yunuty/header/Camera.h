#pragma once
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

namespace yunutyEngine::graphics
{
    class YunutyCycle;
    class YUNUTY_API Camera : public Component
    {
    private:
        yunuGI::GIPtr<yunuGI::ICamera> yunuGICamera;
        static Camera* mainCamera;
    protected:
        //virtual void Render() = 0;
        virtual void Update() override;
    public:
        Camera();
        void SetCameraMain();
        static Camera* GetMainCamera();
        friend YunutyCycle;
    };
}
