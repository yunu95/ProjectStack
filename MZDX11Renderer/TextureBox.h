#pragma once
#include "DX11Define.h"

/// <summary>
/// GBuffer�� �� �������� �׸� �ڽ�
/// �ϴ� �ִ� 5�������� �׷����� �������.
/// 2023.07.13 MJKIM
/// </summary>

class MZCamera;

class TextureBox
{
public:
	//TextureBox();
	TextureBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~TextureBox();

public:
	void SetTexture(ID3D11ShaderResourceView* tex);
	void Update(MZCamera* pCamera);
	void Render();

private:
	void BuildGeometryBuffers();
	

private:
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dImmediateContext;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	//ID3DX11Effect* m_FX;
	//ID3DX11EffectTechnique* m_effectTech;
	//ID3DX11EffectMatrixVariable* m_fxWorldViewProj;

	//ID3D11InputLayout* m_inputLayout;

	XMMATRIX m_world;
	XMMATRIX m_view;
	XMMATRIX m_proj;

	ID3D11RasterizerState* m_pRS;

	ID3D11ShaderResourceView* mDiffuseMapSRV;
	XMFLOAT4X4 mTexTransform;

	static UINT currentIndex;	// �ڽ��� ������ ������ �ϳ��� ����
	UINT uniqueIndex;	// �ڽ����� ������ �ε���
};



