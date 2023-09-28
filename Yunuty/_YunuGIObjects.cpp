#include <assert.h>
#include "GraphicsRenderer.h"
#include "_YunuGIObjects.h"

namespace yunutyEngine::graphics
{
    void _YunuGIObjects::LoadDll(const std::wstring& dllFileName)
    {
        // CreateResourceManager �ڵ� ¥�� �����ϸ� ��
        HINSTANCE hDLL{ LoadLibraryW(dllFileName.c_str()) };
        assert(hDLL != nullptr, L"�׷��Ƚ� ������ �ش��ϴ� DLL ������ �����ϴ�!");

        yunuGI::Global::CreateRenderer = reinterpret_cast<decltype(yunuGI::Global::CreateRenderer)>(GetProcAddress(hDLL, "CreateRenderer"));
        assert(yunuGI::Global::CreateRenderer != nullptr, L"�׷��Ƚ� ������ ���� �Լ��� �����ϴ�!");

        yunuGI::Global::CreateFactory = reinterpret_cast<decltype(yunuGI::Global::CreateFactory)>(GetProcAddress(hDLL, "CreateFactory"));
        assert(yunuGI::Global::CreateFactory != nullptr, L"�׷��Ƚ� ���丮 ���� �Լ��� �����ϴ�!");

        yunuGI::Global::CreateEventHandler = reinterpret_cast<decltype(yunuGI::Global::CreateEventHandler)>(GetProcAddress(hDLL, "CreateEventHandler"));
        assert(yunuGI::Global::CreateEventHandler != nullptr, L"�׷��Ƚ� �̺�Ʈ �ý��� �ڵ鷯 ���� �Լ��� �����ϴ�!");

        yunuGI::Global::CreateResourceManager = reinterpret_cast<decltype(yunuGI::Global::CreateResourceManager)>(GetProcAddress(hDLL, "CreateResourceManager"));
        assert(yunuGI::Global::CreateResourceManager != nullptr, L"�׷��Ƚ� ���ҽ� �Ŵ��� ���� �Լ��� �����ϴ�!");

        yunuGI::Global::ReleaseYunuGI = reinterpret_cast<decltype(yunuGI::Global::ReleaseYunuGI)>(GetProcAddress(hDLL, "ReleaseYunuGI"));
        assert(yunuGI::Global::ReleaseYunuGI != nullptr, L"�׷��Ƚ� �������̽� ������ �Լ��� �����ϴ�!");

        renderer = yunuGI::Global::CreateRenderer({});
        assert(renderer.Get() != nullptr, L"�׷��Ƚ� �������� �������� �ʾҽ��ϴ�!");
        //renderer->Initialize();

        eventHandler = yunuGI::Global::CreateEventHandler({});
        assert(eventHandler.Get() != nullptr, L"�׷��Ƚ� �̺�Ʈ �ý��� �ڵ鷯�� �������� �ʾҽ��ϴ�!");

        resourceManager = yunuGI::Global::CreateResourceManager({});
        assert(resourceManager.Get() != nullptr, L"�׷��Ƚ� ���ҽ� �Ŵ����� �������� �ʾҽ��ϴ�!");

        factory = yunuGI::Global::CreateFactory(yunuGI::IFactoryDesc());
        assert(factory.Get() != nullptr, L"�׷��Ƚ� �������� �������� �ʾҽ��ϴ�!");
    }
}
