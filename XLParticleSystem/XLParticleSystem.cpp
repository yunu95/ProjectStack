#include <algorithm>

#include "XLParticleSystem.h"
#include "XLParticleEffect.h"


#include "DDSTextureLoader.h"

#include <dxgi1_2.h>
#include <d3d11.h>
#include <d3dcompiler.h>

ID3D11Device* s_pDevice;
ID3D11DeviceContext* s_pDeviceContext;
UINT s_MaxParticles;

XLParticleSystem::XLParticleSystem()
	:mGameTime(0), mTimeStep(0), mAge(0),
	mEyePosW(), mEmitPosW(), mEmitDirW(),
	m_Effects()
{

}

XLParticleSystem::~XLParticleSystem()
{

}


void XLParticleSystem::SetEyePos(const XMFLOAT3& eyePosW)
{
	mEyePosW = eyePosW;
}

void XLParticleSystem::SetEmitPos(const XMFLOAT3& emitPosW)
{
	mEmitPosW = emitPosW;
}

void XLParticleSystem::SetEmitDir(const XMFLOAT3& emitDirW)
{
	mEmitDirW = emitDirW;
}

void XLParticleSystem::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT maxParticles)
{
	s_pDevice = device;
	s_pDeviceContext = deviceContext;
	s_MaxParticles = maxParticles;

	m_pDevice = device;
	m_pDeviceContext = deviceContext;

}

void XLParticleSystem::Reset()
{
	for (auto& e : m_Effects)
	{
		e->mFirstRun = true;
	}

	mAge = 0.0f;
}

UINT XLParticleSystem::GetMaxParticles()
{
	return s_MaxParticles;
}

void XLParticleSystem::Update(float dt, XMMATRIX ViewProj, XMFLOAT3 CameraPosition)
{
	mTimeStep = dt;
	mGameTime += dt;

	for (auto& e : m_Effects)
	{
		e->SetViewProj(ViewProj);
		e->SetEyePosW(CameraPosition);
		e->SetGameTime(mGameTime);
		e->SetTimeStep(mTimeStep);

		e->Update();
	}
}

void XLParticleSystem::Render()
{
	/// ����� ���ؽ� ���� ����

	UINT stride = sizeof(Particle); // Size of One Particle in Byte
	UINT offset = 0;

	for (auto& e : m_Effects)
	{
		m_pDeviceContext->IASetInputLayout(e->ParticleInputLayout);
		m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		// ���� �����̸� �ʱ�ȭ�� VB(Emitter Paricle) ���, �׷��� ������ ���� ��ƼŬ ����� ���� VB ���
		if (e->mFirstRun)
		{
			m_pDeviceContext->IASetVertexBuffers(0, 1, &(e->mInitVB), &stride, &offset);
		}
		else
		{
			m_pDeviceContext->IASetVertexBuffers(0, 1, &(e->mDrawVB), &stride, &offset);
		}

		// ���� ���� ����� 'SO ���� ��ũ��'���� �׷��� ���� ����
		// ���ŵ� ���ڵ��� mStreamOutVB�� ����ȴ�
		m_pDeviceContext->SOSetTargets(1, &(e->mStreamOutVB), &offset);


		e->StreamOutTech->GetPassByIndex(0)->Apply(0, m_pDeviceContext);

		// ���� �����̸� �ʱ�ȭ�� VB(Emitter Paricle) ���, �׷��� ������ ���� ��ƼŬ ����� ���� VB ���
		if (e->mFirstRun)
		{
			m_pDeviceContext->Draw(1, 0);
			e->mFirstRun = false;
		}
		else
		{
			m_pDeviceContext->DrawAuto(); // ��ο����� �ϸ� ��Ʈ�� ��� ���� ��ũ������ ����?
		}


		// SO ���� �н��� ������ ���� ���۸� unbinding
		ID3D11Buffer* bufferArray[1] = { 0 };
		m_pDeviceContext->SOSetTargets(1, bufferArray, &offset);

		// ���� ���͵��� �¹ٲ۴� (����)
		std::swap(e->mDrawVB, e->mStreamOutVB);

		// ��� ��Ʈ�� ��µ�, ���ŵ� ��ƼŬ���� ���� VB�� ȭ�鿡 �׸���.
		m_pDeviceContext->IASetVertexBuffers(0, 1, &(e->mDrawVB), &stride, &offset);
		e->DrawTech->GetPassByIndex(0)->Apply(0, m_pDeviceContext);
		m_pDeviceContext->DrawAuto();


	}
}

void XLParticleSystem::AddParticleEffect(XLParticleEffect* effect)
{
	effect->Initialize();
	effect->BuildVB();
	// TODO: Effect �ȿ� �ִ� ���� �´� �� ����. ��? ��ƼŬ�� �� ������ �޶����� ��ǲ���̾ƿ��� �ٲ��� �ϱ� ����..
	//BuildVB(effect);
	m_Effects.emplace_back(effect);
}

ID3D11ShaderResourceView* XLParticleSystem::LoadTexture(LPCWSTR textureFilePath)
{
	/// Particle �ؽ��� ����
	ID3D11Resource* ParticleTexture = nullptr;
	ID3D11ShaderResourceView* ParticleSRV = nullptr;

	// dds ���Ϸκ��� Shader Resource View ����
	HRESULT hr = CreateDDSTextureFromFile(s_pDevice, textureFilePath, &ParticleTexture, &ParticleSRV);

	return ParticleSRV;
}

ID3DX11Effect* XLParticleSystem::CompileFX(LPCWSTR fxFilePath)
{
	ID3DX11Effect* pFX = nullptr;

	ID3DBlob* compiledShader = nullptr;
	HRESULT hr = D3DCompileFromFile(fxFilePath, NULL, NULL, NULL, "fx_5_0", D3DCOMPILE_DEBUG, NULL, &compiledShader, NULL);

	// Effect ��ü ����
	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, s_pDevice, &(pFX));

	return pFX;
}