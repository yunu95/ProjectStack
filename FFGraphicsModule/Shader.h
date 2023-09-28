#pragma once

#include "IResource.h"

class Device;
class GraphicsCommandQueue;
class ComputeCommandQueue;
class RootSignature;

/// <summary>
/// ���̴����� �ε�� ���������� ���¸� ����� Ŭ����
/// 
/// 2023.06.16 �輺�� ���������
/// </summary>
/// 
 
class Shader : public IResource
{
public:
	Shader();
	virtual ~Shader();

	// �־��� ��η� VS, PS ���� / ���������� ���� ����
	void CreateGraphicsShader(Device* p_device, RootSignature* p_rootSignature, GraphicsCommandQueue* p_graphicsCommandQueue, ComputeCommandQueue* p_computeCommandQueue, const wstring& p_path, ShaderInfo p_info = ShaderInfo());
	void CreateComputeShader(Device* p_device, RootSignature* p_rootSignature, GraphicsCommandQueue* p_graphicsCommandQueue, ComputeCommandQueue* p_computeCommandQueue, const wstring& p_path, const string& p_name, const string& p_version);

	// �� ������Ʈ���� ���������� ���� ����
	void Update();

	void SetShaderInfo(ShaderInfo p_info) { m_shaderInfo = p_info; };

	ShaderInfo GetShaderInfo() { return m_shaderInfo; }

	static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopology(D3D_PRIMITIVE_TOPOLOGY p_topology);

private:
	// ���̴� ����
	void CreateShader(const wstring& p_path, const string& p_name, const string& p_version, ComPtr<ID3DBlob>& p_blob, D3D12_SHADER_BYTECODE& p_shaderByteCode);

	// ���ؽ� ���̴� ����
	void CreateVertexShader(const wstring& p_path, const string& p_name, const string& p_version);

	// �ȼ� ���̴� ����
	void CreatePixelShader(const wstring& p_path, const string& p_name, const string& p_version);

	// ������Ʈ�� ���̴� ����
	void CreateGeometryShader(const wstring& p_path, const string& p_name, const string& p_version);

	void SetPipelineState();

private:
	ShaderInfo m_shaderInfo = ShaderInfo();
	ComPtr<ID3D12PipelineState>			m_pipelineState;
	ComPtr<ID3D12PipelineState>			m_shadowPipelineState;

	// Graphics Shader
	ComPtr<ID3DBlob>					m_vsBlob;
	ComPtr<ID3DBlob>					m_psBlob;
	ComPtr<ID3DBlob>					m_gsBlob;
	ComPtr<ID3DBlob>					m_errBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_pipelineDesc = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_shadowPipelineDesc = {};

	// Compute Shader
	ComPtr<ID3DBlob> m_csBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC m_computePipelineDesc = {};

private:
	// ������ ������
	Device* m_device;
	GraphicsCommandQueue* m_graphicsCommandQueue;
	ComputeCommandQueue* m_computeCommandQueue;
	RootSignature* m_rootSignature;
};

