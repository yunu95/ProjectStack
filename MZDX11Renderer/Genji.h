#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "Vertex.h"
#include "IMeshObject.h"

class MZCamera;
//class CASEParser;

/// <summary>
/// 3D������ ������ ���
///
/// 2023.07.07 MJKIM
/// </summary>
class Genji : public IMeshObject
{
public:
	//Genji();
	Genji(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Genji();

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) override;
	virtual void RenderToTexture() override;
	//virtual void Render() override;
	virtual void SetWorldTM(const XMMATRIX& tm) override { m_world = tm; };
	virtual void SetActive(bool isActive) override { m_isActive = isActive; };
	virtual bool Pick(MZCamera* pCamera, float x, float y) override { return false; };
	virtual void SetPickingMode(bool isPickingModeOn) { };
	
	void SetTranslateMatrix(float offsetX, float offsetY, float offsetZ);
	void SetRotateMatrix(float radian);
	void SetScaleMatrix(float scale);

public:
	void SetDevice(ID3D11Device* pDevice) { m_d3dDevice = pDevice; }
	void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) { m_d3dImmediateContext = pDeviceContext; }
	void SetRasterizerState(ID3D11RasterizerState* pRS) { m_pRS = pRS; }

private:
	void BuildGeometryBuffers();		// ���ϱ����κ��� ���ؽ�/�ε������۸� �����.
	//void SetLights();

private:
	ComPtr<ID3D11Device> m_d3dDevice;						// D3D11 ����̽�
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	// Transform Matrix
	DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_proj;

	// ����������Ʈ�� �ٱ����� �޾ƿ´�.
	ComPtr<ID3D11RasterizerState> m_pRS;

	UINT IndexCount;
	Material m_material;

	/// ���� �ܺο��� �޾ƿ����� ����
	//DirectionalLight m_dirLights[3];
	//PointLight m_pointLights[3];
	//SpotLight m_spotLights[3];

	//XMFLOAT3 mEyePosW;

	///----------------------------------------------------------------------------------------------------
	/// �ؽ��Ŀ��� �߰��� �κ�
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11ShaderResourceView* mNormalMapSRV;

	DirectX::XMFLOAT4X4 mTexTransform;
	
	DirectX::XMMATRIX translateTM;
	DirectX::XMMATRIX rotateTM;
	DirectX::XMMATRIX scaleTM;

	bool m_isActive = true;
};
