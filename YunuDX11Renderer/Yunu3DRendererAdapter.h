#pragma once
#include "IYunuGI3DRenderer.h"
#include "YunuDX11Renderer.h"

#ifdef YunuDX11DLL_EXPORTS
#define YunuDX11API __declspec(dllexport)
#else
#define YunuDX11API __declspec(dllimport)
#endif

#include <Windows.h>
#include <memory>


class Yunu3DRendererAdapter : public Yunu3DAPI::IYunuGI3DRenderer
{
private:
    //YunuDX11Renderer YunuDX11Renderer::GetInstance();
public:
    virtual ~Yunu3DRendererAdapter() { YunuDX11Renderer::GetInstance().Release(); }
    // DX�� �ʱ�ȭ�Ѵ�.
    virtual bool Initialize() { return YunuDX11Renderer::GetInstance().Initialize(); }

    // �׸��� �׸� ��� �����츦 ����
    virtual void SetOutputWindow(HWND hWnd) { YunuDX11Renderer::GetInstance().SetOutputWindow(hWnd); }
    // ������Ʈ�� ���� ���忡���� ���� �ʴ´�. �� �������̽��� ���� ����������μ� �����ϸ�, ���� ���� ���� �����Ǿ�� �Ѵ�.
    virtual void Update(float deltaTime) { YunuDX11Renderer::GetInstance().Update(deltaTime); }
    // ������ ����
    virtual void BeginRender() { YunuDX11Renderer::GetInstance().BeginRender(); };
    virtual void Render() {
        std::vector<D3D11_VIEWPORT> viewports{
            D3D11_VIEWPORT{ 0,0,1,1}
        };
        YunuDX11Renderer::GetInstance().Render(viewports);
    };

    // ������ ������. �׸� �� �� �׸� ���¿��� Present
    virtual void EndRender() { YunuDX11Renderer::GetInstance().EndRender(); };

    // ������ �����Ѵ�.
    virtual void Release() { YunuDX11Renderer::GetInstance().Release(); };
    virtual void OnMouseDown(int x, int y) { YunuDX11Renderer::GetInstance().OnMouseDown(x, y); }
    virtual void OnMouseUp(int x, int y) { YunuDX11Renderer::GetInstance().OnMouseUp(x, y); }
    virtual void OnMouseMove(int btnState, int x, int y) { YunuDX11Renderer::GetInstance().OnMouseMove(btnState, x, y); }

    // â ũ�Ⱑ ������ �� ó���ؾ� �� �͵�
    virtual void OnResize() { YunuDX11Renderer::GetInstance().OnResize(); }

    // Ȥ���� ���� �׷��Ƚ� ������ �˾ƾ� �� ��
    virtual ID3D11Device* QueryD3d11Device() { return YunuDX11Renderer::GetInstance().md3dDevice; }
    virtual ID3D11DeviceContext* QueryD3dDeviceContext() { return YunuDX11Renderer::GetInstance().md3dDeviceContext; }
};
