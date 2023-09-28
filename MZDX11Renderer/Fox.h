#pragma once
#include "DX11Define.h"
#include "IMeshObject.h"

class MZCamera;

class Fox : public IMeshObject
{
public:
	Fox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Fox();

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) override;
	virtual void RenderToTexture() override;
	virtual void SetWorldTM(const XMMATRIX& tm) override { m_world = tm; };
	virtual void SetActive(bool isActive) override {};
	virtual bool Pick(MZCamera* pCamera, float x, float y) override { return false; };

	void SetTranslateMatrix(float offsetX, float offsetY, float offsetZ);
	void SetRotateMatrix(float radian);
	void SetScaleMatrix(float scale);

public:
	void SetDevice(ID3D11Device* pDevice) { m_d3dDevice = pDevice; }
	void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) { m_d3dImmediateContext = pDeviceContext; }
	void SetRasterizerState(ID3D11RasterizerState* pRS) { m_pRS = pRS; }

private:
	void BuildGeometryBuffers();		// ���ϱ����κ��� ���ؽ�/�ε������۸� �����.

private:
	ComPtr<ID3D11Device> m_d3dDevice;						// D3D11 ����̽�
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	// Transform Matrix
	XMMATRIX m_world;	// Define transformations from local spaces to world space.
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;

	// ����������Ʈ�� �ٱ����� �޾ƿ´�.
	ComPtr<ID3D11RasterizerState> m_pRS;

	UINT IndexCount;
	Material m_material;

	///----------------------------------------------------------------------------------------------------
	/// �ؽ��Ŀ��� �߰��� �κ�
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11ShaderResourceView* mNormalMapSRV;

	XMFLOAT4X4 mTexTransform;

	XMMATRIX translateTM;
	XMMATRIX rotateTM;
	XMMATRIX scaleTM;
};

