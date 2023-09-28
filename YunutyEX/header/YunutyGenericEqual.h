#pragma once
#include <compare>
#include <memory>

namespace yunutyEngine
{
    // yunutyUtility���� �Լ� ����, Ȥ�� functor�� ����.
    namespace yutility
    {
        template<typename T>
        struct GenericEqual {
            bool operator()(const T& lhs, const T& rhs) const {
                // Compare Ts for equality
                return (!memcmp(&(lhs), &(rhs), sizeof(T)));
            }
        };
    }
}
