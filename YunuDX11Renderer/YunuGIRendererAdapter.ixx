#ifdef USE_MODULE
module;
#include "IYunuGI3DRenderer.h"
#include "YunuDX11Renderer.h"
#include <d3d11.h>

#include <Windows.h>
#include <memory>
export module YunuGIAdapter.RendererAdapter;

// YunuGI �������̽� �� Renderer�� �������̽��� ���ߴ� �����
export namespace YunuGIAdapter
{
    class RendererAdapter : public YunuGI::I3DRenderer
    {
    private:
        //YunuDX11Renderer YunuDX11Renderer::GetInstance();
    public:
        virtual ~RendererAdapter() { YunuDX11Renderer::SingleInstance.Release(); }
        // DX�� �ʱ�ȭ�Ѵ�.
        virtual bool Initialize() { return YunuDX11Renderer::SingleInstance.Get().Initialize(); }

        // �׸��� �׸� ��� �����츦 ����
        virtual void SetOutputWindow(HWND hWnd) { YunuDX11Renderer::SingleInstance.Get().SetOutputWindow(hWnd); }
        // ������Ʈ�� ���� ���忡���� ���� �ʴ´�. �� �������̽��� ���� ����������μ� �����ϸ�, ���� ���� ���� �����Ǿ�� �Ѵ�.
        virtual void Update(float deltaTime) { YunuDX11Renderer::SingleInstance.Get().Update(deltaTime); }
        // ������ ����
        virtual void BeginRender() { YunuDX11Renderer::SingleInstance.Get().BeginRender(); };
        virtual void Render() {
            std::vector<D3D11_VIEWPORT> viewports{
                D3D11_VIEWPORT{ 0,0,1,1}
            };
            YunuDX11Renderer::SingleInstance.Get().Render(viewports);
        };

        // ������ ������. �׸� �� �� �׸� ���¿��� Present
        virtual void EndRender() { YunuDX11Renderer::SingleInstance.Get().EndRender(); };

        // ������ �����Ѵ�.
        virtual void Release() { YunuDX11Renderer::SingleInstance.Release(); };
        virtual void OnMouseDown(int x, int y) { YunuDX11Renderer::SingleInstance.Get().OnMouseDown(x, y); }
        virtual void OnMouseUp(int x, int y) { YunuDX11Renderer::SingleInstance.Get().OnMouseUp(x, y); }
        virtual void OnMouseMove(int btnState, int x, int y) { YunuDX11Renderer::SingleInstance.Get().OnMouseMove(btnState, x, y); }

        // â ũ�Ⱑ ������ �� ó���ؾ� �� �͵�
        virtual void OnResize() { YunuDX11Renderer::SingleInstance.Get().OnResize(); }

        // Ȥ���� ���� �׷��Ƚ� ������ �˾ƾ� �� ��
        virtual ID3D11Device* QueryD3d11Device();
        //{ return YunuDX11Renderer::SingleInstance.Get().md3dDevice; };
        virtual ID3D11DeviceContext* QueryD3dDeviceContext() { return YunuDX11Renderer::SingleInstance.Get().md3dDeviceContext; }
    };
}
#endif
