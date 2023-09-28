#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "IDebugObject.h"

/// <summary>
/// ����׿� ť�� ��� ��ü
/// 
/// 2023.08.10 MJKIM
/// </summary>

class MZCamera;

class DebugCube : public IDebugObject
{
public:
	DebugCube(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS, XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 size = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 color = XMFLOAT4((const float*)&Colors::Red));
	~DebugCube();

	void SetPosition(XMFLOAT3 pos) { m_position = pos; }
	void SetColor(XMFLOAT4 color);

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) override;
	void Update(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
	virtual void RenderToTexture() override;
	virtual void SetWorldTM(const XMMATRIX& tm) override 
	{ 
		m_world = tm; 
		m_world.r[3].m128_f32[0] += m_position.x;
		m_world.r[3].m128_f32[1] += m_position.y;
		m_world.r[3].m128_f32[2] += m_position.z;
	};
	virtual void SetActive(bool isActive) override { 
		s_IsActive = isActive;
		m_isActive = isActive; 
	};
	//virtual bool Pick(MZCamera* pCamera, float x, float y) override { return false; }

public:
	void SetFillModeSolid() { m_pRenderstate = RasterizerState::SolidRS.Get(); };
	void SetFillModeWireframe() { m_pRenderstate = RasterizerState::WireframeRS.Get(); };

private:
	void BuildGeometryBuffers();

private:
	ComPtr<ID3D11Device> m_d3dDevice;						/// D3D11 ����̽�
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		/// ����̽� ���ؽ�Ʈ

	ComPtr<ID3D11Buffer> m_VB;	// ���ؽ�����
	ComPtr<ID3D11Buffer> m_IB;	// �ε�������

	// ����������Ʈ. �������� ��� �� ���ΰ��� ���� ��.
	ComPtr<ID3D11RasterizerState> m_pRenderstate;
	
	// ��ȯ ����
	DirectX::XMMATRIX m_world;	// ���� ��ȯ ��� (����->����)
	DirectX::XMFLOAT4X4 m_view;	// �þ� ��ȯ ��� (ī�޶� ��)
	DirectX::XMFLOAT4X4 m_proj;	// ���� ��ȯ ��� (����/����)

	XMFLOAT3 m_position;
	XMFLOAT3 m_size;
	XMFLOAT4 m_color;

	static bool s_IsActive;
	bool m_isActive = true;
};

