#pragma once
#include "I3DRenderer.h"
#include "DX11Define.h"
#include "LazyObjects.h"
#include <windows.h>

/// DirectX 11 �������� ���� ������
/// 2023.06.27 MJKIM

class DXTKFont;
class MZCamera;
class IRenderableObject;
class GridBuilder;
class DeferredRenderer;

class XLParticleEffect;
class XLParticleSystem;

namespace yunuGIAdapter
{
    class RendererAdapter;
}
class MZDX11Renderer : public MZ3DAPI::I3DRenderer
{
public:
    MZDX11Renderer();
    ~MZDX11Renderer();

    static MZRenderer::LazyObjects<MZDX11Renderer> Instance;
    friend MZRenderer::LazyObjects<MZDX11Renderer>;
    friend yunuGIAdapter::RendererAdapter;

    float GetScreenWidth() { return m_screenWidth; }
    float GetScreenHeight() { return m_screenHeight; }

    // ���� �ʱ�ȭ
    bool Initialize();
    void SetOutputWindow(unsigned int hWnd);
    // ������Ʈ
    void Update(float deltaTime);

    // ����
    void Render();

    // ���� ����
    void Finalize();

public:
    // â ��ȯ ����
    virtual void ResizeResolution(unsigned int width, unsigned int height);
    void OnResize();
    float GetAspectRatio() const;

private:
    float m_screenWidth;
    float m_screenHeight;

public:
    // ���콺 �Է� ����
    void OnMouseDown(int btnState, int x, int y);
    void OnMouseUp(int x, int y);
    void OnMouseMove(int btnState, int x, int y);

    // �̺�Ʈ(Ŭ��, �ø� ��) ����
    IRenderableObject* Pick(float normalizedX, float normalizedY);

private:
    // Adapter Info
    DXGI_ADAPTER_DESC1 m_AdapterDesc1;
    HRESULT GetAdapterInfo();


private:
    void BeginRender();
    void EndRender();
    // ����� ���� ���
    int switcher;
    void DrawStatus();

private:
    HWND m_hWnd;
    UINT m_4xMsaaQuality;
    bool m_enable4xMsaa;

    ComPtr<ID3D11Device> m_d3dDevice;					// D3D11 ����̽�	
    ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		// ����̽� ���ؽ�Ʈ	
    ComPtr<IDXGISwapChain> m_swapChain;					// ����ü��
    //ComPtr<ID3D11Texture2D> m_depthStencilBuffer;		// ���� ���ٽ� ����
    ComPtr<ID3D11RenderTargetView> m_backBufferRTV;	// ����� ���� Ÿ�� ��
    //ComPtr<ID3D11DepthStencilView> m_depthStencilView;	// ���� ���ٽ� ��
    //D3D11_VIEWPORT m_viewPort;							// �� ��Ʈ
    ComPtr<ID3D11Texture2D> m_backBufferTexture;
    D3D_DRIVER_TYPE m_d3dDriverType;
    D3D_FEATURE_LEVEL m_featureLevel;

private:
    DXTKFont* m_pFont;
    float m_fontLineSpace;

    MZCamera* m_pCamera;
    float m_cameraSpeed;

    POINT m_lastMousePos;
    float m_deltaTime;

    /// Builder
    //GridBuilder* m_gridBuilder;

private:

	XLParticleSystem* m_pParticleSystem;
};

extern "C" __declspec(dllexport) MZ3DAPI::I3DRenderer * CreateDX11GraphicsInstance();
extern "C" __declspec(dllexport) void ReleaseDX11GraphicsInstance(MZ3DAPI::I3DRenderer * instance);