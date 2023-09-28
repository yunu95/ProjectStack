#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"
#include "Vertex.h"
#include "IRenderableObject.h"

/// <summary>
/// PBXLoader �׽�Ʈ�� ������Ʈ
/// PBXLoader�κ��� �����Ͱ� �� ���Դ��� �׽�Ʈ�Ѵ�.
/// 2023.08.01 MJKIM
/// </summary>

class MZCamera;

class Box
{
public:
	Box(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Box();

public:
	void Initialize();
	void Update(MZCamera* pCamera);
	void Render();

private:
	void BuildGeometryBuffers();		// ���ϱ����κ��� ���ؽ�/�ε������۸� �����.

private:
	ComPtr<ID3D11Device> m_d3dDevice;						// D3D11 ����̽�
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	// Transform Matrix
	XMFLOAT4X4 m_world;	// Define transformations from local spaces to world space.
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;

	XMFLOAT3 mEyePosW;

	// ����������Ʈ�� �ٱ����� �޾ƿ´�.
	ComPtr<ID3D11RasterizerState> m_pRS;

	UINT IndexCount;
	Material m_material;

	///----------------------------------------------------------------------------------------------------
	/// �ؽ��Ŀ��� �߰��� �κ�
	XMMATRIX mWorldPos;
	XMMATRIX translateTM;
	XMMATRIX rotateTM;
	XMMATRIX scaleTM;

	DirectionalLight m_dirLights[3];
};
