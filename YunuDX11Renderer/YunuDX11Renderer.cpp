#include <Windows.h>
#include <wrl.h>
#include "DDSTextureLoader.h"
#include "YunuDX11Renderer.h"
#include "SimpleMath.h"

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )	// adapter info

#include <d3dcompiler.h>	// fx compile

//#include <DirectXColors.h>
#include "DXTKFont.h"

//#include <DirectXMath.h>
#include "YunuMath.h"

#include "Camera.h"

#include "Axis.h"
#include "Grid.h"
#include "SkinnedMesh.h"
//#include "Box.h"
#include "Crate.h"
//#include "SiegeTank.h"
#include "../Yunu3DLib/ASEParser.h"
#include "DeferredBuffers.h"
#include "TextureImage.h"
#include "DebugCone.h"
#include "DebugArrow.h"
#include "DebugSphere.h"

//#ifdef _DEBUG
//#pragma comment( lib, "Lib/Effects11d.lib" )
//#else
//#pragma comment( lib, "Lib/Effects11.lib" )
//#endif

using namespace render_object;
using namespace render_object::Helper;

LazyObject<YunuDX11Renderer> YunuDX11Renderer::SingleInstance;
YunuDX11Renderer::YunuDX11Renderer()
    : mMainWndCaption(L"D3D11 Application"),
    md3dDriverType(D3D_DRIVER_TYPE_HARDWARE),
    mClientWidth(800),
    mClientHeight(600),
    mEnable4xMsaa(false),
    mhMainWnd(0),
    mAppPaused(false),
    mMinimized(false),
    mMaximized(false),
    mResizing(false),
    m4xMsaaQuality(0),

    md3dDevice(0),
    md3dDeviceContext(0),
    mSwapChain(0),
    mDepthStencilBuffer(0),
    mRenderTargetView(0),
    mDepthStencilView(0),
    m_pFont(nullptr), m_deltaTime(0),

    mBoxVB(0), mBoxIB(0), mFX(0), mTech(0),
    mfxWorldViewProj(0), mInputLayout(0),
    mWireframeRS(0), mSolidRS(0), NormalDSS(nullptr),
    m_WorldAxes(nullptr), m_Grid(nullptr),
    m_MeshObject(nullptr), m_TestBox(nullptr), m_TestCrate(nullptr)
{
    ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));

    m_pFont = new DXTKFont();
    m_pCamera = new Camera();
    m_pCamera->SetMain();
    //m_pCamera->SetPosition(8.0f, 8.0f, -8.0f);
    m_pCamera->LookAt(XMFLOAT3(8.0f, 8.0f, -8.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1.0f, 0));


    // Mesh
    XMMATRIX I = XMMatrixIdentity();
    XMStoreFloat4x4(&mWorld, I);
    XMStoreFloat4x4(&mView, I);
    XMStoreFloat4x4(&mProj, I);

}

YunuDX11Renderer::~YunuDX11Renderer()
{
    /// ������/������ �Ҵ�/������ �ݴ� ������
    SafeDelete(m_TestCrate);
    SafeDelete(m_TestBox);
    SafeDelete(m_MeshObject);

    SafeDelete(m_Grid);
    SafeDelete(m_WorldAxes);

    SafeDelete(m_pCamera);
    SafeDelete(m_pFont);

    //mBoxVB->Release();
    //mBoxIB->Release();
    mFX->Release();
    mInputLayout->Release();
    NormalDSS->Release();
    mWireframeRS->Release();
    mSolidRS->Release();

    mRenderTargetView->Release();
    mDepthStencilView->Release();
    mSwapChain->Release();
    mDepthStencilBuffer->Release();

    // Restore all default settings.
    if (md3dDeviceContext)
    {
        md3dDeviceContext->ClearState();
    }

    md3dDeviceContext->Release();
    md3dDevice->Release();
}

bool YunuDX11Renderer::Initialize()
{
    /// ����� ������ ��� DX11 ����� ���

    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    /// D3����̽��� ����̽� ���ؽ�Ʈ�� �����Ѵ�.

    // ���� ����
    ///D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        0,                 // default adapter
        md3dDriverType,
        0,                 // no software device
        createDeviceFlags,
        0, 0,              // default feature level array
        D3D11_SDK_VERSION,
        &md3dDevice,
        &featureLevel,
        &md3dDeviceContext);

    if (FAILED(hr))
    {
        MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
        return false;
    }

    if (featureLevel != D3D_FEATURE_LEVEL_11_0)
    {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
        return false;
    }

    /// ��Ƽ���ø� ��ܰ���� �Ǵ��� üũ
    // Check 4X MSAA quality support for our back buffer format.
    // All Direct3D 11 capable devices support 4X MSAA for all render 
    // target formats, so we only need to check quality support.

    HR(md3dDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
    assert(m4xMsaaQuality > 0);


    // Mesh
    //BuildGeometryBuffers();
    //BuildFX();
    //BuildFX_Create();
    //BuildVertexLayout();

    // Render State
    CreateRenderStates();
    // ���۵� ���� �ʱ�ȭ

    // ��Ʈ ����
    m_pFont->Create(md3dDevice, mSolidRS, NormalDSS);

    Effects::InitAll(md3dDevice);
    InputLayouts::InitAll(md3dDevice);
    // Axis
    m_WorldAxes = new Axis(md3dDevice, md3dDeviceContext, mWireframeRS);
    m_WorldAxes->Initialize();
    m_debugAxis = new Axis(md3dDevice, md3dDeviceContext, mWireframeRS);
    m_debugAxis->Initialize();

    m_Grid = new Grid(md3dDevice, md3dDeviceContext, mWireframeRS);
    m_Grid->Initialize();

    m_pASEParser = new ASEParser();
    m_pASEParser->Init();
    //m_pASEParser->Load((LPSTR)"../ASEFile/teapot.ASE");
    //m_pASEParser->Load((LPSTR)"ASEFile/Cylinder.ASE");
    //m_pASEParser->Load((LPSTR)"ASEFile/babypig_walk_6x.ASE");

    {
        m_pASEParser->Load((LPSTR)"ASEFile/Cylinder.ASE");
        m_MeshObject = new SkinnedMesh(md3dDevice, md3dDeviceContext, mSolidRS);
        m_MeshObject->Initialize();
        m_MeshObject->materialPath = L"./Textures/";
        m_MeshObject->LoadGeometry(m_pASEParser->GetMeshes(), m_pASEParser->nodeIdxByBoneIdx);
        m_MeshObject->LoadAnimation(m_pASEParser->m_list_animation, m_pASEParser->m_scenedata);
        m_MeshObject->LoadDiffuseMap(m_pASEParser->m_list_materialdata[0]->m_map_diffuse->m_bitmap);
        m_MeshObject->SetRootTM(
            //XMMatrixScaling(0.1f, -0.1f, 0.1f) *
            XMMatrixRotationAxis({ 1,0,0 }, -1.57f) *
            XMMatrixTranslation(-50, 0, 0) *
            XMMatrixScaling(0.1f, 0.1f, 0.1f)
        );
    }

    {
        m_pASEParser->Load((LPSTR)"ASEFile/babypig_idle_6x.ASE");
        m_MeshObject2 = new SkinnedMesh(md3dDevice, md3dDeviceContext, mSolidRS);
        m_MeshObject2->Initialize();
        m_MeshObject2->materialPath = L"./Textures/";
        m_MeshObject2->LoadGeometry(m_pASEParser->GetMeshes(), m_pASEParser->nodeIdxByBoneIdx);
        m_MeshObject2->LoadAnimation(m_pASEParser->m_list_animation, m_pASEParser->m_scenedata);
        m_MeshObject2->LoadDiffuseMap(m_pASEParser->m_list_materialdata[0]->m_map_diffuse->m_bitmap);
        m_MeshObject2->SetRootTM(
            //XMMatrixScaling(0.1f, -0.1f, 0.1f) *
            XMMatrixRotationAxis({ 1,0,0 }, -1.57f) *
            //XMMatrixTranslation(0, 50, 50)*
            XMMatrixScaling(0.1f, 0.1f, 0.1f)
        );
    }

    {
        m_pASEParser->Load((LPSTR)"ASEFile/babypig_walk_6x.ASE");
        m_MeshObject3 = new SkinnedMesh(md3dDevice, md3dDeviceContext, mSolidRS);
        m_MeshObject3->Initialize();
        m_MeshObject3->materialPath = L"./Textures/";
        m_MeshObject3->LoadGeometry(m_pASEParser->GetMeshes(), m_pASEParser->nodeIdxByBoneIdx);
        m_MeshObject3->LoadAnimation(m_pASEParser->m_list_animation, m_pASEParser->m_scenedata);
        m_MeshObject3->LoadDiffuseMap(m_pASEParser->m_list_materialdata[0]->m_map_diffuse->m_bitmap);
        m_MeshObject3->SetRootTM(
            //XMMatrixScaling(0.1f, -0.1f, 0.1f) *
            XMMatrixRotationAxis({ 1,0,0 }, -1.57f) *
            XMMatrixTranslation(50, 0, 0) *
            XMMatrixScaling(0.1f, 0.1f, 0.1f)
        );
    }

    m_debugObj_Dir = new DebugArrow(md3dDevice, md3dDeviceContext, mWireframeRS, { 1,0,0,1 }, 0.1f, { 1,2,1 });
    m_debugObj_Point = new DebugSphere(md3dDevice, md3dDeviceContext, mWireframeRS, { 0,1,0,1 });
    m_debugObj_Spot = new DebugCone(md3dDevice, md3dDeviceContext, mWireframeRS, { 0,0,1,1 }, 7, 20);
    m_debugObj_Spot2 = new DebugArrow(md3dDevice, md3dDeviceContext, mWireframeRS, { 0,0,1,1 }, 0.1f, { 1,2,1 });

    ID3D11ShaderResourceView* textureSRV;
    ID3D11Resource* textureResource;
    //CreateDDSTextureFromFile(md3dDevice, L"Textures/WoodCrate01.dds", &textureResource, &textureSRV);
    for (UINT bufferType = 0; bufferType < DeferredBuffers::GBUFFER_COUNT; bufferType++)
    {
        auto eBufferType = static_cast<DeferredBuffers::GBUFFERTYPE>(bufferType);
        constexpr float rectWidth = 2.0 / static_cast<float>(DeferredBuffers::GBUFFER_COUNT);
        m_deferredDebugTextures[eBufferType] = new TextureImage(md3dDevice, md3dDeviceContext, mSolidRS);
        m_deferredDebugTextures[eBufferType]->LoadImageTexture(DeferredBuffers::SingleInstance.Get().GetShaderResourceView(eBufferType));
        m_deferredDebugTextures[eBufferType]->SetRect(1, 1 - rectWidth, -1 + rectWidth * bufferType, -1 + rectWidth * (bufferType + 1));
    }

    //m_debugObj_Point->Initialize();
    //m_debugObj_Spot->Initialize();
    //m_debugObj_Spot2->Initialize();
    //m_debugObj_Dir->Initialize();

    mDirLights[0].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mDirLights[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.f, 1.0f);
    mDirLights[0].Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
    mDirLights[0].Direction = XMFLOAT3(0.777f, 0.777f, 0.0f);

    mPointLights[0].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mPointLights[0].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.f, 1.0f);
    mPointLights[0].Specular = XMFLOAT4(0.0f, 0.4f, 0.0f, 1.0f);
    mPointLights[0].Att = XMFLOAT3(0.01f, 0.01f, 0.01f);
    mPointLights[0].Position = XMFLOAT3(0, 10, 10);
    mPointLights[0].Range = 30;

    mSpotLights[0].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mSpotLights[0].Diffuse = XMFLOAT4(0.0f, 0.0f, 1.f, 1.0f);
    mSpotLights[0].Specular = XMFLOAT4(0.0f, 0.0f, 0.4f, 1.0f);
    mSpotLights[0].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
    mSpotLights[0].Att = XMFLOAT3(0.1, 0, 0.0);
    mSpotLights[0].Position = XMFLOAT3(0, 10, 10);
    mSpotLights[0].Range = 20;
    mSpotLights[0].Spot = 32;


    // ����� ������ ��´�.
    GetAdapterInfo();

    return true;
}
void YunuDX11Renderer::SetOutputWindow(HWND hWnd)
{
    mhMainWnd = (HWND)hWnd;
    // ���â�� �ٲ������ ����ü�ε� �ٲ۴�.

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = mClientWidth;
    sd.BufferDesc.Height = mClientHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Use 4X MSAA? 
    if (mEnable4xMsaa)
    {
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    // No MSAA
    else
    {
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = mhMainWnd;
    //sd.OutputWindow = 0;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    // To correctly create the swap chain, we must use the IDXGIFactory that was
    // used to create the device.  If we tried to use a different IDXGIFactory instance
    // (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
    // This function is being called with a device from a different IDXGIFactory."

    // D3����̽��κ��� dxgi����̽��� ���´�.
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = 0;
    HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf()));

    // dxgi����̽��κ��� dxgi����͸� ���´�.
    Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = 0;
    HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)dxgiAdapter.GetAddressOf()));

    // dxgi����ͷκ��� dxgi���丮�� ���´�.
    Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory = 0;
    HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)dxgiFactory.GetAddressOf()));

    // ���� ����ü���� �����Ѵ�.
    HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));
}
void YunuDX11Renderer::ResizeResolution(unsigned int width, unsigned int height)
{
    mClientWidth = width;
    mClientHeight = height;

    // �ƿ�ǲ �����찡 �ٲ������ ��������� �Ͼ�� ������ �� ��� �Ѵ�.
    assert(md3dDeviceContext);
    assert(md3dDevice);

    // Release the old views, as they hold references to the buffers we
    // will be destroying.  Also release the old depth/stencil buffer.

    if (mRenderTargetView)
        mRenderTargetView->Release();
    if (mDepthStencilView)
        mDepthStencilView->Release();
    if (mDepthStencilBuffer)
        mDepthStencilBuffer->Release();

    if (mSwapChain)
    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
        HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
        HR(md3dDevice->CreateRenderTargetView(backBuffer.Get(), 0, &mRenderTargetView));
    }
    //HR(md3dDevice->CreateShaderResourceView(backBuffer.Get(), nullptr, mRenderTargetSRV.GetAddressOf()));
    // Create the depth/stencil buffer and view.

    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = mClientWidth;
    depthStencilDesc.Height = mClientHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // Use 4X MSAA? --must match swap chain MSAA values.
    if (mEnable4xMsaa)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    // No MSAA
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    HR(md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
    HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));


    // Bind the render target view and depth/stencil view to the pipeline.
    /// ����Ÿ�ٺ�, ����/���ĽǺ並 ���������ο� ���ε��Ѵ�.

    //md3dDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

    // ���۵� ������ �ٽ� �ʱ�ȭ�Ѵ�.
    DeferredBuffers::SingleInstance.Get().Initialize(md3dDevice, mClientWidth, mClientHeight, 1, 0, 1, 1);
    DeferredBuffers::SingleInstance.Get().RenderTargetFrontBuffer(md3dDeviceContext);

    // Set the viewport transform.
    /// ����Ʈ ��ȯ�� �����Ѵ�.

    mScreenViewport.TopLeftX = 0;
    mScreenViewport.TopLeftY = 0;
    mScreenViewport.Width = static_cast<float>(mClientWidth);
    mScreenViewport.Height = static_cast<float>(mClientHeight);
    mScreenViewport.MinDepth = 0.0f;
    mScreenViewport.MaxDepth = 1.0f;

    md3dDeviceContext->RSSetViewports(1, &mScreenViewport);

    m_pCamera->SetLens(0.25f * YunuMath::Pi, GetAspectRatio(), 1.0f, 1000.0f);

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    /// â�� ũ�Ⱑ �������Ƿ�, ��Ⱦ�� ������Ʈ�ϰ� ���� ����� �����Ѵ�.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * YunuMath::Pi, GetAspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

// deltaTime�� ��
void YunuDX11Renderer::Update(float deltaTime)
{
    m_deltaTime = deltaTime;
    m_pCamera = Camera::GetMainCamera();

    // Axis
    m_WorldAxes->Update(XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());
    m_Grid->Update(XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());
    // Mesh Object
    XMVECTOR headScale;
    XMVECTOR headQuat;
    XMVECTOR headTrans;

    XMMATRIX headRotationMatrix;

    static XMMATRIX headInitialTM = m_MeshObject->GetBoneLocalTM("Biped-Head");
    static float mElapsed = 0;
    mElapsed += deltaTime;
    XMMatrixDecompose(
        &headScale, &headQuat, &headTrans,
        headInitialTM
    );
    headRotationMatrix = XMMatrixRotationQuaternion(headQuat) * XMMatrixRotationAxis({ 1,0,0 }, mElapsed * 1.1f);
    //m_debugAxis->Update(XMMatrixTranslationFromVector(headTrans), m_pCamera->View(), m_pCamera->Proj());
    m_MeshObject->Update(deltaTime, m_pCamera);
    m_MeshObject2->Update(deltaTime, m_pCamera);
    m_MeshObject3->Update(deltaTime, m_pCamera);
    //m_MeshObject2->Update(deltaTime, m_pCamera);

    // Box����
    //m_TestBox->Update(XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());

    // Crate ����
    //m_TestCrate->Update(m_pCamera, deltaTime);
    //m_SiegeTank->Update(m_pCamera);

    /*m_debugObj_Dir->Update(m_pCamera);
    m_debugObj_Point->Update(m_pCamera);
    m_debugObj_Spot->Update(m_pCamera);
    m_debugObj_Spot2->Update(m_pCamera);*/
    Update_LightsControl(deltaTime);
}

/// <summary>
/// ������ �ϱ� ��
/// </summary>
void YunuDX11Renderer::BeginRender()
{
}

// BeginRender()�� EndRender() ���̿� ȣ�� �� ������ ����ϴ� �׽�Ʈ�� �Լ�.
void YunuDX11Renderer::RenderToTexture(vector<D3D11_VIEWPORT>& viewPorts)
{
    assert(md3dDeviceContext);

    // ����Ÿ�� �並 Ŭ�����Ѵ�.
    //md3dDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&YunuColors::DeepDarkGray));

    // �������Ľ� �並 Ŭ�����Ѵ�.
    //md3dDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    float blackColor[]{ 0,0,0,1 };
    //DeferredBuffers::SingleInstance.Get().RenderTargetGBuffers(md3dDeviceContext);
    DeferredBuffers::SingleInstance.Get().SwapBackBuffer();
    DeferredBuffers::SingleInstance.Get().ClearRenderTargets(md3dDeviceContext, blackColor);
    XMMATRIX I = XMMatrixIdentity();
    XMStoreFloat4x4(&mWorld, I);
    XMStoreFloat4x4(&mView, I);
    XMStoreFloat4x4(&mProj, I);


    /// WVP TM���� ����
    // Set constants
    XMMATRIX world = XMLoadFloat4x4(&mWorld);
    XMMATRIX view = XMLoadFloat4x4(&mView);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);

    /// ī�޶� ����� ��
    m_pCamera->UpdateViewMatrix();
    //view = m_pCamera->View();
    //proj = m_pCamera->Proj();
    view = Camera::GetMainCamera()->View();
    proj = Camera::GetMainCamera()->Proj();

    // ��� ���� ������ ���ؼ� ������������� ����� �������ش�. (��ü�� ĳ�ø� �ٲٴ� ���̸� GPU�� ������۰� �ٷ� ���ŵǴ� ���� �ƴϴ�.)

    md3dDeviceContext->OMSetDepthStencilState(NormalDSS, 0);

    // �׸���� ���� �׸���.
    m_Grid->RenderToTexture();

    m_WorldAxes->RenderToTexture();
    m_MeshObject->RenderToTexture();
    m_MeshObject2->RenderToTexture();
    m_MeshObject3->RenderToTexture();

    /*if (m_isDirectionalLightActive)
        m_debugObj_Dir->Render();
    if (m_isPointLightActive)
        m_debugObj_Point->Render();
    if (m_isSpotLightActive)
    {
        m_debugObj_Spot->Render();
        m_debugObj_Spot2->Render();
    }*/

    for (UINT bufferType = 0; bufferType < DeferredBuffers::GBUFFER_COUNT; bufferType++)
        m_deferredDebugTextures[bufferType]->LoadImageTexture(DeferredBuffers::SingleInstance.Get().GetShaderResourceView(bufferType));
    for (auto each : m_deferredDebugTextures)
        each->RenderToTexture();

    DirectX::SimpleMath::Vector4 _yellow(1.f, 1.f, 0.f, 1.f);
    DirectX::SimpleMath::Vector4 _white(1.f, 1.f, 1.f, 1.f);

    m_pFont->DrawTextColor(200, 100, _yellow, (TCHAR*)L"1 : Directional Light\n2 : Point Light\n3 : Spot Light\n4 : Toggle white light / RGB light");

    // �����ֱ�
    static float _addedTime = 0;
    static float _FPS = 0;
    static float _deltaTimeMS = 0;

    // �����ֱ�� 0.2��
    if (0.2f < _addedTime)
    {
        _FPS = (1.0f / m_deltaTime);
        _deltaTimeMS = m_deltaTime * 1000.0f;

        _addedTime = 0;
    }

    _addedTime += (m_deltaTime);

    // FPS, deltaTime�� �׸���.
    m_pFont->DrawTextColor(0, 10, _white, (TCHAR*)L"FPS : %.2f", _FPS);
    m_pFont->DrawTextColor(0, 24, _white, (TCHAR*)L"DTime : %.4f ms", _deltaTimeMS);

    // ���ķ���, ����� ���� ���¸� �׸���.
    this->Draw_Status();

    // Restore default.
    md3dDeviceContext->RSSetState(0);
    md3dDeviceContext->Flush();
    if (mSwapChain)
        HR(mSwapChain->Present(0, 0));
}

/// <summary>
/// ������ ������. �׸� �� �� �׸� ���¿��� Present
/// </summary>
void YunuDX11Renderer::EndRender()
{
}

/// <summary>
/// �������� �����Ѵ�.
/// </summary>
//void YunuDX11Renderer::Release()
//{
//    // DX�� �ڿ� �������� �Ҹ��ڶ�� ������� Ÿ�̹��� �ִ�.
//    // ����� �� �� �ڿ��� (��ũ��Ʈ, ��ü���� ���۷��� ���� ��)�� ó���Ѵ�.
//}
void YunuDX11Renderer::SetSwapChainAsRenderTarget()
{
    md3dDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

/// <summary>
/// ���ķ���, �ػ�, ����� �� �̰����� ��� ���
/// ���� �������� ����. �������� �л����� ������ ����� ���̹Ƿ�..
/// </summary>
void YunuDX11Renderer::Draw_Status()
{
    DirectX::SimpleMath::Vector4 _white(1.f, 1.f, 1.f, 1.f);	// white

    // ���ķ���
    int _yPos = 50;
    m_pFont->DrawTextColor(0, _yPos, _white, (TCHAR*)L"Feature Level : %x", featureLevel);

    // ����� ����
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"Description: %s", m_AdapterDesc1.Description);
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"VendorID: %u", m_AdapterDesc1.VendorId);
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"DeviceID: %u", m_AdapterDesc1.DeviceId);
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"SubSysID: %u", m_AdapterDesc1.SubSysId);
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"Revision: %u", m_AdapterDesc1.Revision);
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"VideoMemory: %lu MB", m_AdapterDesc1.DedicatedVideoMemory / 1024 / 1024);
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"SystemMemory: %lu MB", m_AdapterDesc1.DedicatedSystemMemory / 1024 / 1024);
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"SharedSysMemory: %lu MB", m_AdapterDesc1.SharedSystemMemory / 1024 / 1024);
    m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"AdpaterLuid: %u.%d", m_AdapterDesc1.AdapterLuid.HighPart, m_AdapterDesc1.AdapterLuid.LowPart);


    // ī�޶� ����
    m_pFont->DrawTextColor(0, _yPos += 28, _white, (TCHAR*)L"Camera Pos : %.2f / %.2f / %.2f", m_pCamera->GetPosition().x, m_pCamera->GetPosition().y, m_pCamera->GetPosition().z);
}

float YunuDX11Renderer::GetAspectRatio() const
{
    return static_cast<float>(mClientWidth) / mClientHeight;
}

//void YunuDX11Renderer::BuildGeometryBuffers()
//{
//    /// ���ؽ� ���۸� �����Ѵ�. 
//    // Create vertex buffer
//    /// ������ü�� ��� ���� �Ƹ��������, ��������� ���ų� �� ���� ����� ���̵��� 
//    /// ��ġ�� ��¦ ������ ��ٸ������·� �������.
//
//    Vertex vertices[] =
//    {
//        { XMFLOAT3(-0.3f, -0.3f, -0.01f), XMFLOAT4((const float*)&Colors::White)   },
//        { XMFLOAT3(-0.3f, +0.3f, 0.01f), XMFLOAT4((const float*)&Colors::Black)   },
//        { XMFLOAT3(+0.3f, +0.3f, 0.01f), XMFLOAT4((const float*)&Colors::Red)     },	// ��� ����
//        { XMFLOAT3(+0.3f, -0.3f, 0.01f), XMFLOAT4((const float*)&Colors::Green)   },
//        { XMFLOAT3(-1.0f, -1.0f, +0.5f), XMFLOAT4((const float*)&Colors::Blue)    },
//        { XMFLOAT3(-1.0f, +1.0f, +0.5f), XMFLOAT4((const float*)&Colors::Yellow)  },
//        { XMFLOAT3(+1.0f, +1.0f, +0.5f), XMFLOAT4((const float*)&Colors::Cyan)    },
//        { XMFLOAT3(+1.0f, -1.0f, +0.5f), XMFLOAT4((const float*)&Colors::Magenta) }
//    };
//
//    D3D11_BUFFER_DESC vbd;
//    vbd.Usage = D3D11_USAGE_IMMUTABLE;
//    vbd.ByteWidth = sizeof(Vertex) * 8;
//    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    vbd.CPUAccessFlags = 0;
//    vbd.MiscFlags = 0;
//    vbd.StructureByteStride = 0;
//    D3D11_SUBRESOURCE_DATA vinitData;
//    vinitData.pSysMem = vertices;
//    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));
//
//
//    /// �ε��� ���۸� �����Ѵ�.
//    // Create the index buffer
//
//    UINT indices[] =
//    {
//        // front face
//        0, 1, 2,
//        0, 2, 3,
//
//        // back face
//        4, 6, 5,
//        4, 7, 6,
//
//        // left face
//        4, 5, 1,
//        4, 1, 0,
//
//        // right face
//        3, 2, 6,
//        3, 6, 7,
//
//        // top face
//        1, 5, 6,
//        1, 6, 2,
//
//        // bottom face
//        4, 0, 3,
//        4, 3, 7
//    };
//
//    D3D11_BUFFER_DESC ibd;
//    ibd.Usage = D3D11_USAGE_IMMUTABLE;
//    ibd.ByteWidth = sizeof(UINT) * 36;
//    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    ibd.CPUAccessFlags = 0;
//    ibd.MiscFlags = 0;
//    ibd.StructureByteStride = 0;
//    D3D11_SUBRESOURCE_DATA iinitData;
//    iinitData.pSysMem = indices;
//    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB));
//}


// 2. ������ �� ���� �ε��ؼ� �����ϴ� ����
//void YunuDX11Renderer::BuildFX()
//{
//    std::ifstream fin("./fx/color.fxo", std::ios::binary);
//
//    fin.seekg(0, std::ios_base::end);
//    int size = (int)fin.tellg();
//    fin.seekg(0, std::ios_base::beg);
//    std::vector<char> compiledShader(size);
//
//    fin.read(&compiledShader[0], size);
//    fin.close();
//
//    HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
//        0, md3dDevice, &mFX));
//
//    // ����Ʈ ��ü�� ���ؼ� ��ũ���� ���´�.
//    mTech = mFX->GetTechniqueByName("ColorTech");
//
//    // ����Ʈ ��ü�� ���ؼ� ��������� ������ �����´�.
//    mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
//}

// 1. ������ �ϴ� ����
//void YunuDX11Renderer::BuildFX_Create()
//{
//    DWORD shaderFlags = 0;
//#if defined( DEBUG ) || defined( _DEBUG )
//    shaderFlags |= D3D10_SHADER_DEBUG;
//    shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif
//
//    ID3D10Blob* compiledShader = 0;
//    ID3D10Blob* compilationMsgs = 0;
//    //HRESULT hr = D3DX11CompileFromFile(L"FX/color.fx", 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
//    HRESULT hr = D3DCompileFromFile(L"FX/wire.fx", 0, 0, 0, "fx_5_0", shaderFlags, 0, &compiledShader, &compilationMsgs);
//
//    // compilationMsgs can store errors or warnings.
//    if (compilationMsgs != 0)
//    {
//        ///MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
//        compilationMsgs->Release();
//    }
//
//    // Even if there are no compilationMsgs, check to make sure there were no other errors.
//    //if(FAILED(hr))
//    //{
//    //	DXTrace(__FILEW__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
//    //}
//
//    HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
//        0, md3dDevice, &mFX));
//
//    // Done with compiled shader.
//    compiledShader->Release();
//
//    mTech = mFX->GetTechniqueByName("ColorTech");
//    mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
//}

//void YunuDX11Renderer::BuildVertexLayout()
//{
//    // Create the vertex input layout.
//    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
//    {
//        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
//    };
//
//    // Create the input layout
//    D3DX11_PASS_DESC passDesc;
//    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
//    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
//        passDesc.IAInputSignatureSize, &mInputLayout));
//}

void YunuDX11Renderer::CreateRenderStates()
{
    // Render State �� Rasterizer State
    D3D11_RASTERIZER_DESC solidDesc;
    ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
    solidDesc.FillMode = D3D11_FILL_SOLID;
    solidDesc.CullMode = D3D11_CULL_BACK;
    solidDesc.FrontCounterClockwise = false;
    solidDesc.DepthClipEnable = true;

    HR(md3dDevice->CreateRasterizerState(&solidDesc, &mSolidRS));


    D3D11_RASTERIZER_DESC wireframeDesc;
    ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
    wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireframeDesc.CullMode = D3D11_CULL_BACK;
    wireframeDesc.FrontCounterClockwise = false;
    wireframeDesc.DepthClipEnable = true;

    HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));


    //
    // ��Ʈ�� DSS
    //
    D3D11_DEPTH_STENCIL_DESC equalsDesc;
    ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    equalsDesc.DepthEnable = true;
    equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// ���̹��ۿ� ����� �Ѵ�
    equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    HR(md3dDevice->CreateDepthStencilState(&equalsDesc, &NormalDSS));
}

// ����̽�/GPU ���� ȹ�� <��� #1> 
// DXGI 1.1 / DXGI Factory ��� / DX11 �̻�� ����, DXGI 1.0 �� 1.1 ȥ�����. DXGI 1.3 �� Windows 8.1 �̻��ʼ�.

// DXGI 1.1 ����
HRESULT YunuDX11Renderer::GetAdapterInfo()
{
    // DXGI�������� �ٸ�
    IDXGIAdapter1* pAdapter;
    IDXGIFactory1* pFactory = NULL;

    HRESULT hr = S_OK;

    // DXGIFactory ��ü ����(DXGI.lib �ʿ�)
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
    {
        return E_FAIL;
    }

    hr = pFactory->EnumAdapters1(0, &pAdapter);

    if (hr != DXGI_ERROR_NOT_FOUND)
    {
        pAdapter->GetDesc1(&m_AdapterDesc1);
    }

    pAdapter->Release();
    pFactory->Release();

    return S_OK;
}
void YunuDX11Renderer::Update_LightsControl(float delta)
{
    static PointLight emptyPointLights[3]{};
    static DirectionalLight emptyDirectionalLights[3]{};
    static SpotLight emptySpotLights[3]{};
    if (WasKeyJustPressed<'1'>())
    {
        m_isDirectionalLightActive = !m_isDirectionalLightActive;
        Effects::BasicTexFX->SetDirLights(m_isDirectionalLightActive ? mDirLights : emptyDirectionalLights);
        Effects::skinnedBasicTexFX->SetDirLights(m_isDirectionalLightActive ? mDirLights : emptyDirectionalLights);
    }
    if (WasKeyJustPressed<'2'>())
    {
        m_isPointLightActive = !m_isPointLightActive;
        Effects::skinnedBasicTexFX->SetPointLights(m_isPointLightActive ? mPointLights : emptyPointLights);
    }
    if (WasKeyJustPressed<'3'>())
    {
        m_isSpotLightActive = !m_isSpotLightActive;
        Effects::skinnedBasicTexFX->SetSpotLights(m_isSpotLightActive ? mSpotLights : emptySpotLights);
    }
    if (WasKeyJustPressed<'4'>())
    {
        m_isWhiteLight = !m_isWhiteLight;
        if (m_isWhiteLight)
        {
            mDirLights[0].Diffuse = { 1,1,1,1 };
            mPointLights[0].Diffuse = { 1,1,1,1 };
            mSpotLights[0].Diffuse = { 1,1,1,1 };
            mDirLights[0].Specular = { 1,1,1,1 };
            mPointLights[0].Specular = { 1,1,1,1 };
            mSpotLights[0].Specular = { 1,1,1,1 };
        }
        else
        {
            mDirLights[0].Diffuse = { 1,0,0,1 };
            mPointLights[0].Diffuse = { 0,1,0,1 };
            mSpotLights[0].Diffuse = { 0,0,1,1 };
            mDirLights[0].Specular = { 1,0,0,1 };
            mPointLights[0].Specular = { 0,1,0,1 };
            mSpotLights[0].Specular = { 0,0,1,1 };
        }
    }
    if (WasKeyJustPressed<'5'>())
    {

    }
    static float angle = 0;
    constexpr float angleDirDelta = 30;
    constexpr float anglePointDelta = 60;
    constexpr float angleSpotDelta = 90;
    // rotate lights
    //angle += delta * 30;

    if (m_isPointLightActive)
    {
        float x = 4 * cos((angle + anglePointDelta) * YunuMath::Pi / 180.f);
        float y = 10;
        float z = 4 * sin((angle + anglePointDelta) * YunuMath::Pi / 180.f);
        XMStoreFloat4x4(&m_debugObj_Point->mWorld, XMMatrixTranslation(x, y, z));
        mPointLights[0].Position = XMFLOAT3(x, y, z);
        Effects::BasicTexFX->SetPointLights(mPointLights);
        Effects::skinnedBasicTexFX->SetPointLights(mPointLights);
    }

    if (m_isDirectionalLightActive)
    {
        float x = cos((angle + angleDirDelta) * YunuMath::Pi / 180.f);
        float y = 1;
        float z = sin((angle + angleDirDelta) * YunuMath::Pi / 180.f);
        XMVECTOR dir{ XMVector3Normalize({x,y,z}) };
        m_debugObj_Dir->SetArrow(dir * -3, dir * 10);
        XMStoreFloat3(&mDirLights[0].Direction, -dir);
        Effects::BasicTexFX->SetDirLights(mDirLights);
        Effects::skinnedBasicTexFX->SetDirLights(mDirLights);
    }
    if (m_isSpotLightActive)
    {
        float x = cos((angle + angleSpotDelta) * YunuMath::Pi / 180.f);
        float y = 0.5;
        float z = sin((angle + angleSpotDelta) * YunuMath::Pi / 180.f);
        XMVECTOR dir{ XMVector3Normalize({x,y,z}) };
        XMVECTOR posDelta{ 0,10,0 };
        float deltaRot = acos(XMVectorGetX(XMVector3Dot(-dir, { 0,-1,0 })));

        XMVECTOR pos = dir * 10 + posDelta;

        XMStoreFloat4x4(&m_debugObj_Spot->mWorld,
            XMMatrixRotationAxis(XMVector3Cross({ 0,-1,0 }, -dir), deltaRot) *
            XMMatrixTranslationFromVector(pos)
        );

        m_debugObj_Spot2->SetArrow(dir * -20, pos);
        XMStoreFloat3(&mSpotLights[0].Position, pos);
        XMStoreFloat3(&mSpotLights[0].Direction, -dir);
        Effects::BasicTexFX->SetSpotLights(mSpotLights);
        Effects::skinnedBasicTexFX->SetSpotLights(mSpotLights);
    }
}
