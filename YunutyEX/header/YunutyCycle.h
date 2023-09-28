#pragma once
#include <thread>
#include "Object.h"
#include <functional>
#include <mutex>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunutyEngine
{
    class Component;
    class YUNUTY_API YunutyCycle : Object
    {
    private:
        thread updateThread;
        bool isGameRunning{ false };

        void ActiveComponentsDo(function<void(Component*)> todo);
        void ActiveComponentsDo(void (Component::* method)());
        vector<Component*> GetActiveComponents();
        vector<GameObject*> GetGameObjects(bool onlyActive = true);
        vector<std::function<void()>> afterUpdateActions;
        std::mutex actionReservationMutex;
        
        static void UpdateComponent(Component* component);
        static void StartComponent(Component* component);
        void ThreadFunction();
    protected:
        static YunutyCycle* _instance;
        YunutyCycle();
        virtual ~YunutyCycle();
        virtual void ThreadUpdate();
    public:
        // �������� �������� ������ �� ������ false�� �θ� �ȴ�.
        bool autoRendering = true;
        static YunutyCycle& SingleInstance();
        virtual void Initialize();
        virtual void Release();
        void Play();
        void Stop();
        void Pause();
        // ������ Ȱ���ϴ� ������� ���ü� ������ ���涧 ����ϴ� �Լ�
        // ������Ʈ�� ������ ���۽�ų �Լ��� ����Ѵ�. �ѹ� ����� �Լ��� �״�� �����ȴ�.
        void ReserveActionAfterUpdate(std::function<void()> action);
        void SetMaxFrameRate();
        bool IsGameRunning();
    };
}
