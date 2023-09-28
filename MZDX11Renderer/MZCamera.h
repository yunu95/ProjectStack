#pragma once
#include "DX11Define.h"

/// <summary>
/// DX11�� �̿��� 3D ���� ���α׷��� �Թ� å�� 
/// �����Ͽ� ���� ī�޶� Ŭ����.
/// 2023.07.02 MJKIM
/// </summary>

class MZCamera
{
public:
    static MZCamera* GetMainCamera();
    MZCamera();
    ~MZCamera();

    // ī�޶� ���� ������ getter/setter
    XMVECTOR GetPositionXM() const;
    XMFLOAT3 GetPosition() const;
    XMMATRIX GetWorld() const;
    void SetMain();
    void SetWorldTM(const DirectX::XMMATRIX& tm);
    void SetViewMatrix(const XMFLOAT4X4& view) { m_view = view; }
    void SetViewMatrix(const XMMATRIX& matrix) { SetViewMatrix(reinterpret_cast<const XMFLOAT4X4&>(matrix)); }
    void SetPosition(float x, float y, float z);
    void SetPosition(const XMFLOAT3& p);

    // ī�޶� ���� ���� getter
    XMVECTOR GetRightXM() const;
    XMFLOAT3 GetRight() const;
    XMVECTOR GetUpXM() const;
    XMFLOAT3 GetUp() const;
    XMVECTOR GetLookXM() const;
    XMFLOAT3 GetLook() const;

    // �������� ���� getter

    // near plane�� far plane�� �ʺ�, ���� getter
    float GetNearWindowWidth() const;
    float GetNearWindowHeight() const;
    float GetFarWindowWidth() const;
    float GetFarWindowHeight() const;

    // Get view/proj matrices
    XMMATRIX View() const;
    XMMATRIX Proj() const;
    XMMATRIX ViewProj() const;

public:
    // �������� ����
    void SetFrustum(float fovY, float aspect, float zn, float zf);
    // ī�޶� ���� ����
    void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
    void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

    // ī�޶� �̵�
    void Strafe(float dt);
    void Walk(float dt);
    void WorldUpDown(float dt);

    // ī�޶� ȸ��
    void Pitch(float angle);
    void RotateY(float angle);

    // �þ���� ������Ʈ
    void UpdateViewMatrix();

private:
    static MZCamera* mainCamera;
    // ī�޶� ��ǥ��
    XMFLOAT3 m_position;	// ī�޶� ��ġ
    XMFLOAT3 m_right;		// ī�޶��� ������ ����
    XMFLOAT3 m_up;			// ī�޶��� ���� ����
    XMFLOAT3 m_look;		// ī�޶� �ٶ󺸴� ����

    // �� �������� ����
    float m_nearZ;
    float m_farZ;
    float m_aspect;
    float m_fovY;
    float m_nearWindowHeight;
    float m_farWindowHeight;

    // �þ� ���, ���� ���
    XMFLOAT4X4 m_view;
    XMFLOAT4X4 m_proj;
};
