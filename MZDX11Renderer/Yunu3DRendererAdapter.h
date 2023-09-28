#pragma once
#include "YunuGraphicsInterface.h"
#include "MZDX11Renderer.h"
#include "DeferredRenderer.h"

namespace yunuGIAdapter
{
	class RendererAdapter : public yunuGI::I3DRenderer
	{
	public:
		RendererAdapter() { MZDX11Renderer::Instance.Get().Initialize(); };
		virtual ~RendererAdapter() {}

		// DX�� �ʱ�ȭ�Ѵ�.
		//virtual bool Initialize() { return true; };
		// �׸��� �׸� ��� �����츦 ����
		virtual void SetOutputWindow(UINT64 hWnd) { MZDX11Renderer::Instance.Get().SetOutputWindow(hWnd); };
		// ������Ʈ
		virtual void Update(float deltaTime) { MZDX11Renderer::Instance.Get().Update(deltaTime); };
		// ������ ����
		//virtual void BeginRender() { mzRenderer.BeginRender(); };
		virtual void Render() { MZDX11Renderer::Instance.Get().Render(); };
		// ������ ������. �׸� �� �� �׸� ���¿��� Present
		//virtual void EndRender() { mzRenderer.EndRender(); };

		// ���콺 �̵� ����
		//virtual void OnMouseDown(int x, int y) { mzRenderer.OnMouseDown(x, y); };
		//virtual void OnMouseUp(int x, int y) { mzRenderer.OnMouseUp(x, y); };
		//virtual void OnMouseMove(int btnState, int x, int y) { mzRenderer.OnMouseMove(btnState, x, y); };

		// â ũ�Ⱑ ������ �� ó���ؾ� �� �͵�
		virtual void ResizeResolution(unsigned int width, unsigned int height) { MZDX11Renderer::Instance.Get().ResizeResolution(width, height); };

		// Ȥ���� ���� �׷��Ƚ� ������ �˾ƾ� �� ��
		virtual ID3D11Device* QueryD3d11Device() { return MZDX11Renderer::Instance.Get().m_d3dDevice.Get(); }
		virtual ID3D11DeviceContext* QueryD3dDeviceContext() { return MZDX11Renderer::Instance.Get().m_d3dImmediateContext.Get(); }
		//virtual ID3D11ShaderResourceView* QueryD3D11RenderOutputSRV() { return mzRenderer.m_deferredRenderer->m_deferredBuffers->GetShaderResourceView(0); }
		virtual ID3D11ShaderResourceView* QueryD3D11RenderOutputSRV() { return nullptr; }
		virtual void* QuerySharedOutputHandle()
		{
			return DeferredRenderer::Instance.Get().GetSharedBackBufferTextureHandle();
		}
	};
}
