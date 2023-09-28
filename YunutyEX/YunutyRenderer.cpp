#include "YunutyRenderer.h"
#include "YunutyEngine.h"
#include "IYunuGI3DRenderer.h"
#include <assert.h>

using namespace YunutyEngine::Graphics;
unique_ptr<YunutyRenderer> YunutyEngine::Graphics::YunutyRenderer::instance = nullptr;

YunutyRenderer& YunutyEngine::Graphics::YunutyRenderer::GetInstance()
{
    if (!instance.get())
        instance = make_unique<YunutyRenderer>();
    return *instance;
}
void YunutyEngine::Graphics::YunutyRenderer::Render()
{
    assert(graphicsRenderer != nullptr, L"�׷��Ƚ� �������� ���� �����Դϴ�!");
    graphicsRenderer->BeginRender();
    graphicsRenderer->Render();
    afterRenderAction();
    graphicsRenderer->EndRender();
}
void YunutyEngine::Graphics::YunutyRenderer::Update(float delta)
{
    graphicsRenderer->Update(delta);
}
void YunutyEngine::Graphics::YunutyRenderer::LoadGraphicsDll(const wstring& dllFileName)
{
    HINSTANCE hDLL{ LoadLibrary(dllFileName.c_str()) };
    assert(hDLL != nullptr, L"�׷��Ƚ� ������ �ش��ϴ� DLL ������ �����ϴ�!");

    using FunctionType = Yunu3DAPI::IYunuGI3DRenderer* (*)();
    FunctionType CreateRenderer{ (FunctionType)GetProcAddress(hDLL, "CreateRenderer") };
    assert(CreateRenderer != nullptr, L"�׷��Ƚ� ������ ���� �Լ��� �����ϴ�!");
    graphicsRenderer = CreateRenderer();
    assert(graphicsRenderer != nullptr, L"�׷��Ƚ� �������� �ᱹ �������� �ʾҽ��ϴ�!");
    graphicsRenderer->Initialize();
}
void YunutyEngine::Graphics::YunutyRenderer::SetOutputWindow(const HWND& hwnd)
{
    assert(graphicsRenderer != nullptr, L"�׷��Ƚ� �������� ���� �����Դϴ�!");
    graphicsRenderer->SetOutputWindow(hwnd);
}
void YunutyEngine::Graphics::YunutyRenderer::SetAfterRenderAction(const function<void()>& action)
{
    afterRenderAction = action;
}

ID3D11Device* YunutyEngine::Graphics::YunutyRenderer::QueryD3d11Device()
{
    return graphicsRenderer->QueryD3d11Device();
};
ID3D11DeviceContext* YunutyEngine::Graphics::YunutyRenderer::QueryD3d11DeviceContext()
{
    return graphicsRenderer->QueryD3dDeviceContext();
}