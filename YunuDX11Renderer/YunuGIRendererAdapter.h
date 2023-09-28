#pragma once
#include "IYunuGI3DRenderer.h"
#include "YunuDX11Renderer.h"
#include <d3d11.h>
#include <Windows.h>
#include <memory>

// YunuGI �������̽� �� Renderer�� �������̽��� ���ߴ� �����
namespace yunuGIAdapter
{
    class RendererAdapter : public yunuGI::I3DRenderer
    {
    private:
        //YunuDX11Renderer YunuDX11Renderer::GetInstance();
    public:
        RendererAdapter(const yunuGI::I3DRendererDesc& desc) {
            YunuDX11Renderer::SingleInstance.Get().Initialize();
            YunuDX11Renderer::SingleInstance.Get().ResizeResolution(1024, 768);
        }
        virtual ~RendererAdapter() { YunuDX11Renderer::SingleInstance.Release(); }
        // DX�� �ʱ�ȭ�Ѵ�.
        //virtual bool Initialize() { return true; /*return YunuDX11Renderer::SingleInstance.Get().Initialize(); */ }

        // �׸��� �׸� ��� �����츦 ����
        virtual void SetOutputWindow(unsigned long long int hWnd) { YunuDX11Renderer::SingleInstance.Get().SetOutputWindow(reinterpret_cast<HWND>(hWnd)); }
        virtual void Update(float deltaTime) { YunuDX11Renderer::SingleInstance.Get().Update(deltaTime); }
        // ����
        virtual void Render() {

            std::vector<D3D11_VIEWPORT> viewports{
                D3D11_VIEWPORT{ 0, 0, 1, 1}
            };
            /*std::vector<D3D11_VIEWPORT> viewports{
                D3D11_VIEWPORT{ 0,0,0.1,0.1}
            };*/
            //YunuDX11Renderer::SingleInstance.Get().BeginRender();
            YunuDX11Renderer::SingleInstance.Get().RenderToTexture(viewports);
            //YunuDX11Renderer::SingleInstance.Get().EndRender();
        };

        // ������ �����Ѵ�.
        //virtual void Release() { YunuDX11Renderer::SingleInstance.Release(); };
        virtual void ResizeResolution(unsigned int width, unsigned int height) override
        {
            YunuDX11Renderer::SingleInstance.Get().ResizeResolution(width, height);
        }

        // Ȥ���� ���� �׷��Ƚ� ������ �˾ƾ� �� ��
        virtual ID3D11Device* QueryD3d11Device() { return YunuDX11Renderer::SingleInstance.Get().md3dDevice; };
        virtual ID3D11DeviceContext* QueryD3dDeviceContext() { return YunuDX11Renderer::SingleInstance.Get().md3dDeviceContext; }
        virtual ID3D11ShaderResourceView* QueryD3D11RenderOutputSRV()
        {

            //return (ID3D11ShaderResourceView*)(sharedHandle);
            //return tempSrv;


            return DeferredBuffers::SingleInstance.Get().GetShaderResourceView(DeferredBuffers::GBUFFERTYPE::gNormal);
        }
        virtual void* QuerySharedOutputHandle()
        {
            return DeferredBuffers::SingleInstance.Get().GetSharedBackBufferTextureHandle();
        }
    };
}
