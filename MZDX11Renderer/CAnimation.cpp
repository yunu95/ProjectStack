#include "CAnimation.h"
using namespace DirectX::SimpleMath;

CAnimation::CAnimation()
{

}

void CAnimation::Update(float timeElapsed)
{
	if (timeElapsed < 0.0f)
		return;

	if (!m_isRotAnimFinished || !m_isPosAnimFinished)
	{
		m_currentFrame = timeElapsed;
	}

	// ���� �������� �̵�/ȸ�� �����Ӱ����� Ŀ���� �ִϸ��̼��� ����� ����
	if (rotFrame.size() != 0)
	{
		if (m_currentFrame >= rotFrame[rotFrame.size() - 1])
		{
			m_currentFrame = 0;
			m_isRotAnimFinished = true;
			//m_isFinished = true;
		}
	}

	if (posFrame.size() != 0)
	{
		if (m_currentFrame >= posFrame[posFrame.size() - 1])
		{
			m_currentFrame = 0;
			m_isPosAnimFinished = true;
			//m_isFinished = true;
		}
	}

}

void CAnimation::AccumulateQuaternion()
{
	Vector4 quaternion;
	float angle = 0.0f;

	// 0 ������ ���� �����ʹ� MeshObject Ŭ�������� decompose�� �����ͷ� pushback �Ǿ���.
	for (int i = 1; i < rotInfo.size(); ++i)
	{
		Vector3 tempAxis;
		tempAxis.x = rotInfo[i].x;
		tempAxis.y = rotInfo[i].y;
		tempAxis.z = rotInfo[i].z;
		angle = rotInfo[i].w;
		quaternion = XMQuaternionRotationAxis(tempAxis, angle);
		quaternionList.push_back(quaternion);
	}

	for (int i = 1; i < quaternionList.size(); ++i)
	{
		quaternionList[i] = XMQuaternionMultiply(quaternionList[i - 1], quaternionList[i]);
	}
}
