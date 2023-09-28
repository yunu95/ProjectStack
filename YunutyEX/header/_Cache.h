#pragma once
#include <stdexcept>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunutyEngine
{
    template<typename T>
    class _Cache
    {
    private:
        T data;
        bool dirty = true;
    public:
        _Cache() :data(T()) {}
        _Cache(const T& data) :data(data), dirty(false) {  }
        operator T() const
        {
#if _DEBUG
            if (dirty)
                throw std::runtime_error(std::string("trying to read cahced data while it's dirty!"));
#endif
            return data;
        }
        T& operator=(const T& data) { dirty = false; return this->data = data; }
        T& operator=(const _Cache& cachedData) { dirty = false; return this->data = cachedData->data; }
        void SetDirty() { dirty = true; }
        bool IsDirty() { return dirty; }
    };
}
