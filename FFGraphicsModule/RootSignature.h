#pragma once

class Device;
class ComputeCommandQueue;

enum 
{
	DEFAULT,
	LINEAR,
	SHADOW,
	END
};

/// <summary>
/// DX12�� �ִ� ��Ʈ �ñ״�ó�� ���õ� �͵��� ���� Ŭ����
/// 
/// ���÷��� ���Ѱ͵� ���⼭ ������ �־�� �ϴ����� �����
/// 
/// 2023.06.16 �輺�� ���������
/// </summary>
class RootSignature
{
public:
	// ���÷��� ��Ʈ �ñ״�ó�� �����.
	void Init(Device* p_device, ComputeCommandQueue* p_computeCommandQueue);

#pragma region Getter
	ComPtr<ID3D12RootSignature> GetGraphicsRootSignature() { return m_graphicsRootSignature; }
	ComPtr<ID3D12RootSignature> GetComputeRootSignature() { return m_computeRootSignature; }
#pragma endregion

private:
	// ��Ʈ �ñ״�ó�� �����.
	void CreateGraphicsRootSignature();
	void CreateComputeRootSignature();


private:
	D3D12_STATIC_SAMPLER_DESC m_samlplerDesc;
	D3D12_STATIC_SAMPLER_DESC m_samlplerDescArr[END];
	ComPtr<ID3D12RootSignature> m_graphicsRootSignature;
	
	ComPtr<ID3D12RootSignature> m_computeRootSignature;

private:
	// ������ ������
	Device* m_device;
	ComputeCommandQueue* m_computeCommandQueue;
};

