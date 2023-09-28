#pragma once
#include <d3d11.h>

/// <summary>
/// Factory �������̽�.
/// ������Ʈ�� ���� ����� ���� ������ �Ѵ�.
/// 2023.07.04 MJKIM
/// </summary>

class IBuilder;
class Grid;

class IFactory
{
public:
	IFactory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~IFactory();


	/// ������Ʈ���� �Ű������� �ٸ��� ������ ���� ��쿡��
	/// ��� ó���ؾ� �ϴ°�?	
	template <class T>
	T* CreateObject()
	{
		return new T(m_d3dDevice, m_d3dImmediateContext, m_pRS);
	}
	/// �ذ��� : ���ø� Ư��ȭ
	/// �׷����� �� ����� ���ø� Ư��ȭ�� �Ź� Ư���� ���̽��� �´� �Լ��� 
	/// �ϳ��� ������ �Ǵ� �� ������...


private:
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dImmediateContext;
	ID3D11RasterizerState* m_pRS;
};