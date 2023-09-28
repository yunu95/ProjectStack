#pragma once
#include "YunuAnimation.h"
class SkinnedMesh;
class YunuAnimation;
class YunuAnimInstance
{
public:
    void SetActive(bool isActive) { this->isActive = isActive; }
    void SetAnimation(LPCSTR animID);
    void SetAnimation(YunuAnimation* targetAnim)
    {
        assert(targetAnim->GetTargetSkeleton() == targetSkeleton && "�ִϸ��̼ǰ� �޽��� ���̷����� ��ġ���� �ʽ��ϴ�!");
        if (targetAnim == nullptr)
        {
            SetDefaultPose();
            return;
        }
        targetAnimation = targetAnim;
        for (int i = 0; i < targetAnim->GetBonesKeyFrames().size(); i++)
        {
            auto& firstKeyFrame = targetAnimation->GetBonesKeyFrames()[i][0];
            boneTMs[i] = XMMatrixTransformation(
                XMVectorZero(),
                XMQuaternionIdentity(),
                firstKeyFrame.localScale,
                XMVectorZero(),
                firstKeyFrame.localRotation,
                firstKeyFrame.localPosition
            );
        }
    }
    void SetSkeleton(Skeleton* skeleton)
    {
        targetSkeleton = skeleton;
        frameIndices.resize(skeleton->GetBones().size());
        boneTMs.resize(skeleton->GetBones().size());
        for (auto i = 0; i < boneTMs.size(); i++)
            boneTMs[i] = skeleton->GetBones()[i].initialTM;
    }
    void SetDefaultPose()
    {
        targetAnimation = nullptr;
        for (int i = 0; i < boneTMs.size(); i++)
            boneTMs[i] = targetSkeleton->GetBones()[0].initialTM;
    }
    void Update(float deltaTime)
    {
		if (!isActive)
			return;
        currentTime += deltaTime;

        if (targetAnimation == nullptr)
            return;

        // ��庰 �ε��� ����
        for (auto nodeIndex = 0; nodeIndex < frameIndices.size(); nodeIndex++)
        {
            auto& keyIndex = frameIndices[nodeIndex];
            auto& keyFrames = targetAnimation->GetBonesKeyFrames()[nodeIndex];
            while (keyIndex < keyFrames.size() - 1 && keyFrames[keyIndex].time < currentTime)
                keyIndex++;
            // ��庰 TM ������Ʈ
            XMVECTOR lerpTranslation{ 0 };
            XMVECTOR lerpRotation{ 0 };
            XMVECTOR lerpScale{ 0 };
            // �ִϸ��̼��� ���� ��쿡 ���� ����ó��
            if (keyIndex == keyFrames.size() - 1)
            {
                lerpTranslation = keyFrames[keyIndex].localPosition;
                lerpRotation = keyFrames[keyIndex].localRotation;
                lerpScale = keyFrames[keyIndex].localScale;
            }
            else
            {
                float t = (currentTime - keyFrames[keyIndex].time) / (keyFrames[keyIndex + 1].time - keyFrames[keyIndex].time);

                lerpTranslation = XMVectorLerp(keyFrames[keyIndex].localPosition, keyFrames[keyIndex + 1].localPosition, t);
                lerpRotation = XMQuaternionSlerp(keyFrames[keyIndex].localRotation, keyFrames[keyIndex + 1].localRotation, t);
                lerpScale = XMVectorLerp(keyFrames[keyIndex].localScale, keyFrames[keyIndex + 1].localScale, t);
            }
            // ����
            boneTMs[nodeIndex] = XMMatrixTransformation(
                XMVectorZero(),
                XMQuaternionIdentity(),
                lerpScale,
                XMVectorZero(),
                lerpRotation,
                lerpTranslation
            );
        }
        if (currentTime > targetAnimation->GetDuration() && isLoop)
            Play();
    }
    void Play()
    {
		if (!isActive)
			return;
        currentTime = 0;
        for (auto& each : frameIndices)
            each = 0;
    }
    void SetLoopMode(bool loop)
    {
        isLoop = loop;
    }
    const vector<XMMATRIX>& GetBoneTMs()
    {
        return boneTMs;
    }
private:
    YunuAnimation* targetAnimation{ nullptr };
    Skeleton* targetSkeleton{ nullptr };
    float currentTime{ 0 };
    bool isPlaying{ false };
    bool isLoop{ false };
    bool isActive{ true };
    vector<unsigned int> frameIndices;
    vector<XMMATRIX> boneTMs;
    friend SkinnedMesh;
};
