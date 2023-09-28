#include "TextureBox.h"
#include "MZCamera.h"
#include "Vertex.h"
#include "Effects.h"
#include "DDSTextureLoader.h"

UINT TextureBox::currentIndex = 0;

TextureBox::TextureBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: //IRenderableObject(pDevice, pDeviceContext, pRS),
	m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS),
	m_VB(nullptr), m_IB(nullptr), //m_FX(nullptr), m_effectTech(nullptr), //m_fxWorldViewProj(nullptr), m_inputLayout(nullptr),
	m_world(), m_view(), m_proj()
{
	uniqueIndex = currentIndex;
	currentIndex++;
	if (currentIndex > 4)
		currentIndex = 4;

	XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity());
	BuildGeometryBuffers();
	//ID3D11Resource* texResource = nullptr;
	//HR(CreateDDSTextureFromFile(m_d3dDevice,
	//	//L"Textures/000000002405.dds", &texResource, &mDiffuseMapSRV));
	//	L"Textures/000000002405_reverse.dds", &texResource, &mDiffuseMapSRV));
}

//TextureBox::TextureBox()
//	://IRenderableObject(),
//	m_VB(nullptr), m_IB(nullptr), //m_FX(nullptr), m_effectTech(nullptr), //m_fxWorldViewProj(nullptr), m_inputLayout(nullptr),
//	m_world(), m_view(), m_proj()
//{
//
//}

TextureBox::~TextureBox()
{
	//m_FX->Release();
	//m_inputLayout->Release();
	m_VB->Release();
	m_IB->Release();
}

void TextureBox::SetTexture(ID3D11ShaderResourceView* tex)
{
	mDiffuseMapSRV = tex;
}

void TextureBox::Update(MZCamera* pCamera)
{
	m_world = XMMatrixIdentity();
	m_view = pCamera->View();
	m_proj = pCamera->Proj();
}

void TextureBox::Render()
{
#ifdef _DEBUG
	// �Է� ��ġ ��ü ����
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::TexInputLayout);
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �ε������ۿ� ���ؽ����� ����
	UINT stride = sizeof(VertexStruct::PosTex);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	// ���� ������Ʈ
	m_d3dImmediateContext->RSSetState(m_pRS);

	// world view proj TM ����
	//XMMATRIX worldViewProj = m_world * m_view * m_proj;
	//m_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));


	// ������ �����
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(m_world);

	ID3DX11EffectTechnique* mTech = Effects::UIObjectFX->DrawUITech;

	// ��ũ��
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	// �����н�
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		Effects::UIObjectFX->SetWorld(XMMatrixIdentity());
		Effects::UIObjectFX->SetDiffuseMap(mDiffuseMapSRV);

		mTech->GetPassByIndex(p)->Apply(0, m_d3dImmediateContext);
		m_d3dImmediateContext->DrawIndexed(6, 0, 0);
	}
#endif
}

void TextureBox::BuildGeometryBuffers()
{
	// ���� ���۸� �����.
	VertexStruct::PosTex vertices[] =
	{
		{ XMFLOAT3(0.6f, 0.6f - 0.4f * uniqueIndex, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.6f, 1.0f - 0.4f * uniqueIndex, 0.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, 1.0f - 0.4f * uniqueIndex, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 0.6f - 0.4f * uniqueIndex, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::PosTex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));

	// �ε��� ���۸� �����.
	UINT indices[] = {
		0,1,2,
		0,2,3,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));
}

