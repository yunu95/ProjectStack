#pragma once
#include "DX11Define.h"
#include "d3dx11Effect.h"
#include "Vertex.h"
#include "IUIObject.h"
#include "xnacollision.h"

/// <summary>
/// �׸� �ڽ� ��翡 Image�� ����ϴ� UI ������Ʈ
/// </summary>

class MZCamera;
class DebugCube;

class UIImage : public IUIObject
{
public:
	UIImage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS, XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2 size = XMFLOAT2(1.0f, 1.0f), XMFLOAT4 color = XMFLOAT4((const float*)&Colors::White));
	~UIImage();

public:
	virtual void Update(MZCamera* pCamera, float deltaTime);
	virtual void Render();
	virtual bool Pick(MZCamera* pCamera, float x, float y);
	virtual void SetPickingMode(bool isPickingModeOn) { m_isPickingOn = isPickingModeOn; };

	virtual void SetWorldTM(const XMMATRIX& tm);
	virtual void SetActive(bool isActive) { m_isActive = isActive; };

	virtual void SetScreenSpace() { m_isSetScreenSpace = true; }
	virtual void SetWorldSpace() { m_isSetScreenSpace = false; }
	void SetImage(LPCSTR fileName);

	virtual float GetDepth() { return m_depth; }

private:
	void BuildGeometryBuffers();
	void SetBoundingVolume();

private:
	ComPtr<ID3D11Device> m_d3dDevice;						/// D3D11 ����̽�
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		/// ����̽� ���ؽ�Ʈ

	ComPtr<ID3D11Buffer> m_VB;	// ���ؽ�����
	ComPtr<ID3D11Buffer> m_IB;	// �ε�������

	// ����������Ʈ. �������� ��� �� ���ΰ��� ���� ��.
	ComPtr<ID3D11RasterizerState> m_pRenderstate;

	std::vector<VertexStruct::PosTex> vertices;

	// ��ȯ ����
	DirectX::XMMATRIX m_world;	// ���� ��ȯ ��� (����->����)
	DirectX::XMMATRIX m_view;	// �þ� ��ȯ ��� (ī�޶� ��)
	DirectX::XMMATRIX m_proj;	// ���� ��ȯ ��� (����/����)

	ComPtr<ID3D11ShaderResourceView> m_diffuseMapSRV;
	XMFLOAT4X4 m_texTransform;

	XMFLOAT3 m_position;
	XMFLOAT2 m_size;
	XMFLOAT4 m_color;

	/// Picking�� ���� bouding box
	XNA::AxisAlignedBox m_meshBox;
	std::string m_meshName;
	DirectX::XMFLOAT3 m_boundingVolumeInitPos;
	DirectX::XMFLOAT3 m_boundingVolumeInitExtents;
	bool m_isPickingOn = true;

	//std::unique_ptr<DebugCube> m_debugCube;

	bool m_isSetScreenSpace;
	float m_depth = 1.0f;

	DirectX::XMFLOAT3 m_scale{ 1.0f, 1.0f, 1.0f };

	bool m_isActive = true;
};

