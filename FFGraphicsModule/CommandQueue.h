#pragma once

class Device;
class ConstantBuffer;
class GraphicsDescriptorHeap;
class SwapChain;
class RenderTargetGroup;
class RootSignature;

/// <summary>
/// GPU���� ���������� ����� ��� ����� ��� Queue
/// �������� Excute�ؼ� CommandList�� �ִ� ��� ����� GPU�� �ѱ��.
/// �׷��⿡ CommandQueue Class�� CommandList�� ������ �ִ´�
/// 
/// 2023.06.16 �輺�� ���������
/// </summary>

class GraphicsCommandQueue
{
public:
	~GraphicsCommandQueue();

	// �ʱ�ȭ�� �����Ѵ�.
	void Init(Device* p_device, vector<ConstantBuffer*>* p_constantBuffers, GraphicsDescriptorHeap* p_graphicsDescriptorHeap, SwapChain* p_swapChain, array<RenderTargetGroup*, RENDER_TARGET_TYPE_COUNT>* p_renderTargets, RootSignature* p_rootSignature);

	// GPU�� CPU�� ����ȭ ���ش�.
	void WaitSync();

	// �׸��� ����
	void RenderBegin(const D3D12_VIEWPORT* p_vp, const D3D12_RECT* p_rect);

	// �׸��� ��
	void RenderEnd();

	// ���ҽ��� CommandList�� �ٽ� 
	void FlushResourceCommandQueue();

#pragma region Getter
	ComPtr<ID3D12CommandQueue> GetGraphicsCmdQueue() { return m_graphicsCmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCmdList() { return m_cmdList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return m_resCmdList; }
#pragma endregion

	
private:
	// �������� CommandQueue
	ComPtr<ID3D12CommandQueue> m_graphicsCmdQueue;

	// ��ü���� ��� ��� (���ҽ� �ε� ����)�� ����� List
	ComPtr<ID3D12CommandAllocator> m_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> m_cmdList;

	// ���ҽ��� �ε��ϴ� �뵵�� ���� Ŀ�ǵ� ����Ʈ
	ComPtr<ID3D12CommandAllocator> m_resCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> m_resCmdList;

	// CPU / GPU ����ȭ�� ���� ������ ����
	ComPtr<ID3D12Fence> m_fence;
	uint32 m_fenceValue = 0;
	HANDLE m_fenceEvent = INVALID_HANDLE_VALUE;

private:
	// ������ ������
	Device* m_device;
	vector<ConstantBuffer*>* m_constantBuffers;
	GraphicsDescriptorHeap* m_graphicsDescriptorHeap;
	SwapChain* m_swapChain;
	array<RenderTargetGroup*, RENDER_TARGET_TYPE_COUNT>* m_renderTargets;
	RootSignature* m_rootSignature;
};

/// Compute�� CommandQueue

class ComputeCommandQueue
{
public:
	~ComputeCommandQueue();

	void Init(Device* p_device, RootSignature* p_rootSignature);
	void WaitSync();
	void FlushComputeCommandQueue();

	ComPtr<ID3D12CommandQueue> GetComputeCmdQueue() { return m_computeCmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetComputeCmdList() { return m_cmdList; }

private:
	ComPtr<ID3D12CommandQueue> m_computeCmdQueue;
	ComPtr<ID3D12CommandAllocator> m_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> m_cmdList;

	ComPtr<ID3D12Fence> m_fence;
	uint32 m_fenceValue = 0;
	HANDLE m_fenceEvent = INVALID_HANDLE_VALUE;

private:
	// ������ ������
	Device* m_device;
	RootSignature* m_rootSignature;
};