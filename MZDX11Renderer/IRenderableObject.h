#pragma once
#include "DX11Define.h"
#include "d3dx11effect.h"

/// <summary>
/// Renderable Object�� �������̽�.
/// 2023.07.04 MJKIM
/// 
/// Deferred Rendering�� ���� ������ �����Ͽ���.
/// Render �κ��� DeferredRenderer���� RenderAll()�� �׷��� ����.
/// 2023.07.19 MJKIM
/// 
/// IRenderableObject�� ��� Object���� �θ�� �����,
/// IMeshObject, IUIObject � ������� ���̴�.
/// �׷��� ���� �ּ����� ��ɸ� ���ܵΰ� ��� ������.
/// 2023.08.16 MJKIM
/// </summary>

class MZCamera;

class IRenderableObject
{
public:
	virtual ~IRenderableObject() {};

	virtual void SetWorldTM(const XMMATRIX& tm) abstract;
	virtual void SetActive(bool isActive) abstract;

protected:
	//bool m_isActive = true;
};