#include "GraphicsPch.h"
#include "SwapChain.h"

//#include "FFGraphicsEngine.h"
#include "Device.h"
#include "CommandQueue.h"

void SwapChain::Init(Device* p_device, GraphicsCommandQueue* p_graphicsCommandQueue, const WindowInfo& p_info)
{
	m_device = p_device;
	m_graphicsCommandQueue = p_graphicsCommandQueue;
	CreateSwapChain(p_info);
}

void SwapChain::Present()
{
	m_swapChain->Present(1, 0);
}

void SwapChain::SwapIndex()
{
	m_backBufferIndex = (m_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void SwapChain::CreateSwapChain(const WindowInfo& p_info)
{
	// ������ ���� ������ ����
	m_swapChain.Reset();

	// DXGI_SWAP_CHAIN_DESC ����ü �ȿ� DXGI_MODE_DESC����ü�� BufferDesc�� �� ����
	DXGI_SWAP_CHAIN_DESC _sd;
	_sd.BufferDesc.Width = static_cast<uint32>(p_info.width);	// ������ �ػ� �ʺ�
	_sd.BufferDesc.Height = static_cast<uint32>(p_info.height);	// ������ �ػ� ����
	_sd.BufferDesc.RefreshRate.Numerator = 60;	// ȭ�� ���� ����
	_sd.BufferDesc.RefreshRate.Denominator = 1;	// ȭ�� ���� ����
	_sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ������ ���÷��� ����
	_sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	_sd.SampleDesc.Count = 1; // ��Ƽ ���ø� OFF
	_sd.SampleDesc.Quality = 0;
	_sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���ۿ� ������ �� ��
	_sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;	// ���� + �ĸ�
	_sd.OutputWindow = p_info.hwnd;
	_sd.Windowed = p_info.windowed;
	_sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
	_sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	m_device->GetDXGI()->CreateSwapChain(
		m_graphicsCommandQueue->GetGraphicsCmdQueue().Get(), &_sd, &m_swapChain);
}
