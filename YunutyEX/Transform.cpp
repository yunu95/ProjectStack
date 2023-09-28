#include "YunutyEngine.h"
#include "Transform.h"
#include "GameObject.h"
#include "_XMMath.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace yunutyEngine;
using namespace DirectX;

// ������ ����. ĳ�̿� �Ű��� ���� �ʾ� ���� ������ ���ݸ� ���������� ���ϰ� �ſ� Ŀ����.
Vector3d yunutyEngine::Transform::GetWorldPosition()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return position;

    auto deltaPosition = (GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * position);
    auto parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
    deltaPosition.x *= parentScale.x;
    deltaPosition.y *= parentScale.y;
    deltaPosition.z *= parentScale.z;
    return  deltaPosition +
        GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
}
//XMMATRIX
// ������ ����2
Quaternion yunutyEngine::Transform::GetWorldRotation()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return rotation;
    return  GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * rotation;
}
Vector3d yunutyEngine::Transform::GetWorldScale()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return scale;
    Vector3d ret;
    auto parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
    ret.x = scale.x * parentScale.x;
    ret.y = scale.y * parentScale.y;
    ret.z = scale.z * parentScale.z;
    return ret;
}
yunuGI::Matrix4x4 yunutyEngine::Transform::GetLocalTM()const
{
    DirectX::XMMATRIX ret = math::GetTransformMatrix(position, scale, rotation);
    return reinterpret_cast<yunuGI::Matrix4x4&>(ret);
}
yunuGI::Matrix4x4 yunutyEngine::Transform::GetWorldTM()const
{
    const GameObject* parent = GetGameObject()->GetParentGameObject();
    if (!parent)
    {
        auto answer = GetLocalTM();
        return GetLocalTM();
    }

    yunuGI::Matrix4x4&& parentTM = parent->GetTransform()->GetWorldTM();
    yunuGI::Matrix4x4&& localTM = GetLocalTM();
    //DirectX::XMMATRIX&& xmOutput = reinterpret_cast<DirectX::XMMATRIX&&>(parentTM) * reinterpret_cast<DirectX::XMMATRIX&&>(localTM);
    DirectX::XMMATRIX&& xmOutput = reinterpret_cast<DirectX::XMMATRIX&&>(localTM) * reinterpret_cast<DirectX::XMMATRIX&&>(parentTM);
    yunuGI::Matrix4x4&& output = reinterpret_cast<yunuGI::Matrix4x4&&>(xmOutput);

    return output;
}
void yunutyEngine::Transform::SetWorldPosition(const Vector3d& position)
{
    auto parent = GetGameObject()->GetParentGameObject();

    if (!parent)
        this->position = position;
    else
        this->position = parent->GetTransform()->GetWorldRotation() * (position - parent->GetTransform()->GetWorldPosition());
}
// ������ ����4
void yunutyEngine::Transform::SetWorldRotation(const Quaternion& rotation)
{
    this->rotation = rotation;
}
void yunutyEngine::Transform::SetCacheDirty()
{

}