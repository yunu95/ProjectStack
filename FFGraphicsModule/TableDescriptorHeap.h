#pragma once

class Device;
class GraphicsCommandQueue;
class ComputeCommandQueue;

/// <summary>
/// ��� Descriptor���� �����ϴ� Table
/// 
/// ����� ���߿� �̸� ������ Rootsignature�� �����Ͽ� �������Ϳ� �Ҵ��Ѵ�.
/// 
/// 2023.06.16 �輺�� ���������
/// </summary>
class GraphicsDescriptorHeap
{
public:
	~GraphicsDescriptorHeap();

public:
	// ��ü���� Descriptor���� ������ DescriptorHeap�� �����. 
	void Init(uint32 p_count, Device* p_device, GraphicsCommandQueue* p_graphicsCommandQueue);

	// �� ������ �Ҹ��鼭 index�� 0���� �ٲ��ش�.
	void Clear();

	// ConstantBufferView�� �����Ѵ�. 
	// ���������� ���� �ε����� �������͸�ŭ ���̺��� �̵��Ͽ� ������ ��ġ�� �������ش�.
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE p_srcHandle, CBV_REGISTER p_reg);

	// ShaderResourceView�� �����Ѵ�.
	// ���������� ���� �ε����� �������͸�ŭ ���̺��� �̵��Ͽ� ������ ��ġ�� �������ش�.
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE p_srcHandle, SRV_REGISTER p_reg);

	// �� �Լ��� �ҷ��� ��� ��Ͽ� ���� �ε����� �ش��ϴ� Handle�� �����Ѵ�.
	void CommitTable(bool p_isFinal = false);

	unsigned __int64 GetGPUHandle() { return m_finalGPUHandle.ptr; }

#pragma region Getter
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_descHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER p_reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER p_reg);
#pragma endregion

private:
	// Ŭ���� ���ο����� ���Ǹ� ���� �ε����� �ش��ϴ� Handle�� ��ȯ�Ѵ�.
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 p_reg);

private:
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	uint64 m_handleSize = 0;
	uint64 m_groupSize = 0;
	uint64 m_groupCount = 0;

	uint32 m_currentGroupIndex = 0;

private:
	D3D12_GPU_DESCRIPTOR_HANDLE m_finalGPUHandle;

private:
	// ������ ������
	Device* m_device;
	GraphicsCommandQueue* m_graphicsCommandQueue;
};

/// Compute�� DescriptorHeap

class ComputeDescriptorHeap
{
public:
	void Init(Device* p_device, ComputeCommandQueue* p_computeCommandQueue);

	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE p_srcHandle, CBV_REGISTER p_reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE p_srcHandle, SRV_REGISTER p_reg);
	void SetUAV(D3D12_CPU_DESCRIPTOR_HANDLE p_srcHandle, UAV_REGISTER p_reg);

	void CommitTable();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER p_reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER p_reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UAV_REGISTER p_reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 p_reg);

private:
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	uint64 m_handleSize = 0;

private:
	// ������ ������
	Device* m_device;
	ComputeCommandQueue* m_computeCommandQueue;
};