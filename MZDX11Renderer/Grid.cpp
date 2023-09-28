#include "Grid.h"
#include "MZCamera.h"
#include "Effects.h"
#include "Vertex.h"

Grid::Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: //IRenderableObject(pDevice, pDeviceContext, pRS),
	m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS),
	m_VB(nullptr), m_IB(nullptr), 
	m_world(XMMatrixIdentity()), m_view(), m_proj()
{
	BuildGeometryBuffers();
}

Grid::Grid()
	: //IRenderableObject(),
	m_VB(nullptr), m_IB(nullptr), 
	m_world(), m_view(), m_proj()
{

}

Grid::~Grid()
{
	
}

void Grid::Update(MZCamera* pCamera, float deltaTime)
{
	if (!m_isActive)
		return;
	//m_world = XMMatrixIdentity();
	m_view = pCamera->View();
	m_proj = pCamera->Proj();
}

void Grid::RenderToTexture()
{
#ifdef _DEBUG
	if (!m_isActive)
		return;
	// �Է� ��ġ ��ü ����
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::PosColor);
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// ���� ������Ʈ
	m_d3dImmediateContext->RSSetState(m_pRS.Get());

	// �ε������ۿ� ���ؽ����� ����
	UINT stride = sizeof(VertexStruct::PosColor);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	// world view proj TM ����
	XMMATRIX worldViewProj = m_world * m_view * m_proj;

	ID3DX11EffectTechnique* mTech = Effects::ColorFX->ColorTech;

	// ��ũ��
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	Effects::ColorFX->SetWorld(m_world);
	Effects::ColorFX->SetWorldViewProj(worldViewProj);

	// �����н�
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, m_d3dImmediateContext.Get());
		// 20���� �ε����� �׸��带 �׸���.
		m_d3dImmediateContext->DrawIndexed(40, 0, 0);
	}
#endif
}

void Grid::BuildGeometryBuffers()
{
	// ���� ���۸� �����.
	VertexStruct::PosColor vertices[100];
	for (int i = 0; i < 100; ++i)
	{
		vertices[i].Pos = XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
		vertices[i].Color = XMFLOAT4((const float*)&Colors::DarkGray);
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::PosColor) * 100;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, m_VB.GetAddressOf()));

	// �ε��� ���۸� �����.
	UINT indices[40];
	for (int i = 0; i < 10; i++)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = i + 90;
	}

	for (int i = 0; i < 10; i++)
	{
		indices[20 + (i * 2)] = i * 10;
		indices[20 + (i * 2) + 1] = i * 10 + 9;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 40;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, m_IB.GetAddressOf()));
}

