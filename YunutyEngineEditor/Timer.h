#pragma once
#include <functional>
class Timer
{
public:
    float elapsed = 0;		// Ÿ�̸Ӱ� ���۵� �� ����� �ð�
    bool isActive = false;
    bool isRepeat = false;
    bool useScaledTime = true;
    float duration = 1;		// ��ǥ���ϴ� �ð� ex) 3�� Ÿ�̸Ӹ� �ϰ� �ʹ�
    std::function<void()> onExpiration = []() {};								// elapsed >= duration
    std::function<void(float)> onUpdate{ [](float progress) {} };
    void Update(float deltaTime)
    {
        if (isActive)
        {
            elapsed += deltaTime;

            if (elapsed > duration)
                elapsed = duration;

            if (onUpdate)
                onUpdate(elapsed / duration);

            if (elapsed == duration)
            {
                if (isRepeat == true)
                {
                    elapsed = 0;
                }
                else
                {
                    isActive = false;
                }
                onExpiration();
            }
        }
    }

    void Start()
    {
        elapsed = 0;
        isActive = true;
    }
};

