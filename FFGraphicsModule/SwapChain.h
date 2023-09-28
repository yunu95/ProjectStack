#pragma once

class Device;
class GraphicsCommandQueue;

/// <summary>
/// DX12�� �ִ� SwapChain�� ���õ� �͵��� ���� Ŭ����
/// 
/// ������ ���� ���۸��� �� ���ۿ� ����Ʈ ���۸� ����
/// ���߿� ���۵��� �� � ������ ������ ��
/// 
/// 2023.06.16 �輺�� ���������
/// </summary>
class SwapChain
{
public:
	// ����ü�ΰ� ����Ÿ�ٺ並 �����.
	void Init(Device* p_device, GraphicsCommandQueue* p_graphicsCommandQueue,const WindowInfo& p_info);

	// ������ ����۸� ����Ʈ���۷� �ٲ��ش�.
	void Present();

	// Present�� �Ǹ� ����ۿ� ����Ʈ���۰� �ٲ�� ������ Index�� �ٲ��ش�.
	void SwapIndex();

#pragma region Getter
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_swapChain; }

	uint32 GetBackBufferIndex() { return m_backBufferIndex; }
#pragma endregion

private:
	// ȭ�� ������ �޾� SwapChain�� �����.
	void CreateSwapChain(const WindowInfo& p_info);

private:
	ComPtr<IDXGISwapChain> m_swapChain;

	uint32 m_backBufferIndex = 0;

private:
	Device* m_device;
	GraphicsCommandQueue* m_graphicsCommandQueue;
};

