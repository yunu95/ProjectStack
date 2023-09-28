#include "Axis.h"
#include "MZCamera.h"
#include "Vertex.h"
#include "Effects.h"

Axis::Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: //IRenderableObject(pDevice, pDeviceContext, pRS),
	m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS),
	m_VB(nullptr), m_IB(nullptr), 
	m_world(XMMatrixIdentity()), m_view(), m_proj()
{
	BuildGeometryBuffers();
}

Axis::Axis()
	: //IRenderableObject(),
	m_VB(nullptr), m_IB(nullptr), 
	m_world(), m_view(), m_proj()
{

}

Axis::~Axis()
{
	
}

void Axis::Update(MZCamera* pCamera, float deltaTime)
{
	if (!m_isActive)
		return;
	//m_world = XMMatrixIdentity();
	m_view = pCamera->View();
	m_proj = pCamera->Proj();
}

void Axis::RenderToTexture()
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
		m_d3dImmediateContext->DrawIndexed(6, 0, 0);
	}
#endif
}

void Axis::BuildGeometryBuffers()
{
	// ���� ���۸� �����.
	VertexStruct::PosColor vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x�� (����)
		{ XMFLOAT3(10.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y�� (�ʷ�)
		{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z�� (�Ķ�)
		{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT4((const float*)&Colors::Blue) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::PosColor) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, m_VB.GetAddressOf()));

	// �ε��� ���۸� �����.
	UINT indices[] = {
		// x��
		0, 1,

		// y��
		2, 3,

		// z��
		4, 5,
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
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, m_IB.GetAddressOf()));
}

