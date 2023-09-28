#pragma once
#include <concepts>

// PairedŬ������ �����Ǹ鼭 �ٸ� ��ü�� �Բ� �����ϰ�, Release �� �� �� ��ü�� �Ҹ��ŵ�ϴ�.
template<typename T>
class PairedClass
{
public:
    template<typename... Args> requires std::is_constructible_v<T, Args...>
    PairedClass(Args... args) : pairedObject{ args... } {  }
protected:
    T pairedObject;
};
