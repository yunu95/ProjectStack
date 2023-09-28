#pragma once

#include <Windows.h>
#include "IUnknown.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct D3D12_GPU_DESCRIPTOR_HANDLE;

// �������� ���ȭ���� �����ϰ�, �̹����� ������ ���ִ� ������ �Ѵ�.
// �������� �ƹ��� ������ ��ü�� ���������� �ʰ�, ��ŷ�� ���� �̺�Ʈ ó���� �������� �ʴ´�.
namespace yunuGI
{
    struct I3DRendererDesc
    {
        unsigned int width = 1024;
        unsigned int height = 768;
    };
    class I3DRenderer : public yunuGI::IUnknown
    {
    public:
        // �������� �ʱ�ȭ�ϴ� �κ��� ��� ���⼭ ó���Ѵ�.
        I3DRenderer() {};
        virtual ~I3DRenderer() {}

        // DX�� �ʱ�ȭ�Ѵ�.
        //virtual bool Initialize() = 0;

        // �׸��� �׸� ��� �����츦 ����, �� �Լ��� ����ϸ�
        // ���� �׷��Ƚ��������� �ش� �����쿡 �̹����� �׸��� ������ �����Ѵ�.
        virtual void SetOutputWindow(UINT64 hWnd) = 0;
        // �ð��� ���� ���°� ��ȭ�Ǿ�� �ϴ� �ִϸ��̼�, ��ƼŬ�� ��쿡 ����ȴ�.
        virtual void Update(float deltaTime) = 0;
        // ������ ����
        //virtual void BeginRender() = 0;
        virtual void Render() = 0;

        // ������ ������. �׸� �� �� �׸� ���¿��� Present
        //virtual void EndRender() = 0;

        // â ũ�Ⱑ ������ �� ó���ؾ� �� �͵�
        //virtual void OnResize() = 0;
        // �ػ� ������ ��û�ϴ� �Լ�
        virtual void ResizeResolution(unsigned int width, unsigned int height) = 0;

        // Ȥ���� ���� �׷��Ƚ� ������ �˾ƾ� �� ��
        //virtual ID3D11Device* QueryD3d11Device() { return nullptr; }
        //virtual ID3D11DeviceContext* QueryD3dDeviceContext() { return nullptr; }
        // void �����ͷ� ���� ��¹��� �������ֱ� ���� ����
        virtual void* QuerySharedOutputHandle() = 0;
        //virtual ID3D11ShaderResourceView* QueryD3D11RenderOutputSRV(ID3D11Device* requesting) { return nullptr; }
        // imgui�� ���� ��¹��� �������ֱ� ���� ����
        //virtual D3D12_GPU_DESCRIPTOR_HANDLE* QueryD3D12RenderOutputGDH() { return nullptr; }
    };
}

