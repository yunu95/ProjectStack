#pragma once

/// <summary>
/// DX12�� �ִ� Device�� ���õ� �͵��� ���� Ŭ����
/// 
/// 2023.06.16 �輺�� ���������
/// </summary>

class Device
{
public:
	void Init();

#pragma region Getter
	ComPtr<IDXGIFactory> GetDXGI() { return m_dxgi; }
	ComPtr<ID3D12Device> GetDevice() 
	{
		return m_device; 
	}
#pragma endregion

private:
	ComPtr<ID3D12Debug>		m_debugController;
	ComPtr<IDXGIFactory>	m_dxgi;		// ȭ�� ���� ��ɵ�
	ComPtr<ID3D12Device>	m_device;	// ���� ��ü ����
};

