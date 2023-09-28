#pragma once
#include "IBuilder.h"
#include "Grid.h"
#include "IRenderableObject.h"

/// <summary>
/// IBuilder�� ��ӹ޴� GridBuilder Ŭ����.
/// �̷��� ������Ʈ���� Builder�� �ϳ��� ������ �ϴ� ����
/// ���ո��� �� ������...
/// �ϴ� ������.
/// 2023.07.04 MJKIM
/// </summary>

class GridBuilder : public IBuilder
{
public:
	virtual IBuilder* SetDevice(ID3D11Device* pDevice) override;
	virtual IBuilder* SetDeviceContext(ID3D11DeviceContext* pDeviceContext) override;
	virtual IBuilder* SetRasterizerState(ID3D11RasterizerState* pRS) override;

	virtual IRenderableObject* Build() override;
};