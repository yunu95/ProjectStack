#pragma once
// �̱��� Ŭ������ �ڽ� Ŭ����Ÿ���� ���ø� �Ķ���ͷ� �޴� ���ø� Ŭ������ �����Ѵ�.

namespace yunutyEngine
{
    template<typename T>
    class SingletonClass
    {
    public:
        static T& SingleInstance();
    private:
        static T* instance;
    };
}
template<typename T>
T& yunutyEngine::SingletonClass<T>::SingleInstance()
{
    if (instance == nullptr)
        instance = new T();
    return *instance;
}
template<typename T>
T* yunutyEngine::SingletonClass<T>::instance = nullptr;
