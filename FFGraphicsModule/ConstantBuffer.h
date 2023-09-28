#pragma once

class Device;
class GraphicsDescriptorHeap;
class ComputeDescriptorHeap;
class GraphicsCommandQueue;

/// <summary>
/// ��� ���� Ŭ���� 
/// ���⼭�� �������� �����Ͱ� �� Buffer�� �� Buffer�� ����Ű�� DrecriptorHeap�� �����Ѵ�.
/// 
/// �������� �����ʹ� ID3D12Resource�� Map�Լ��� �����ϰ� ID3D12Resource�� View��
/// Device�� CreateConstantBufferView�Լ��� �����Ѵ�.
/// 
/// 2023.06.16 �輺�� ���������
/// </summary>
class ConstantBuffer
{
public:
	ConstantBuffer();
	// Map���� ����� ���� Unmap�Ѵ�.
	~ConstantBuffer();

	// Buffer�� View�� �����.
	void Init(CBV_REGISTER p_reg, uint32 p_size, uint32 p_count, Device* p_device, GraphicsDescriptorHeap* p_graphicsDescriptorHeap, ComputeDescriptorHeap* p_computeDescriptorHeap, GraphicsCommandQueue* p_graphicsCommandQueue);

	// �� ������ ȣ��Ǹ� �ε����� �ʱ�ȭ �Ѵ�.
	void Clear();

	// �������� �����͸� �ִ´�. memcpy�̿�
	void PushGraphicsData(void* p_buffer, uint32 p_size);
	void PushComputeSRVData(void* p_buffer, uint32 p_size);

	void SetGlobalData(void* p_buffer, uint32 p_size);

#pragma region Getter
	// ���ϴ� ��ġ�� GPU�ּҸ� �����´�.
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(uint32 p_index);

	// ���ϴ� ��ġ�� CPU Handle�� �����´�.
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 p_index);
#pragma endregion

private:
	void CreateBuffer();
	void CreateView();

private:
	ComPtr<ID3D12Resource> m_cbvBuffer;
	BYTE* m_mappedBuffer = nullptr;
	uint32 m_elementSize = 0;
	uint32 m_elementCount = 0;

	ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandleBegin = {};
	uint32 m_handleIncrementSize = 0;

	uint32 m_currentIndex = 0;

	CBV_REGISTER m_reg = {};

private:
	// ������ ������
	Device* m_device;
	GraphicsDescriptorHeap* m_graphicsDescriptorHeap;
	ComputeDescriptorHeap* m_computeDescriptorHeap;
	GraphicsCommandQueue* m_graphicsCommandQueue;
};

