#pragma once
#include <iostream>
#include <string>
#include <objbase.h>
#include "Object.h"
#include <guiddef.h>
#include <unordered_map>
#include <rpc.h>
#include "YunutyGenericHash.h"
#include "YunutyGenericEqual.h"
#include "YunutyStringConversion.h"
//#include "GameObject.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunutyEngine
{
    class GameObject;
    class YunutyCycle;
    class Collision;
    class Collision2D;
    class Collider2D;
    class Collider;
    class Transform;

    /// <summary>
    /// Component�� ���� ������Ʈ�鿡�� ��ġ�� �� �ִ� ��ü�Դϴ�.
    /// ���� ������Ʈ�� �ش� ���� ������Ʈ�� � Component���� ��ġ�Ǿ� �ִ��Ŀ� ���� �� ������ ���ǵȴٰ� �� �� �ֽ��ϴ�.
    /// </summary>
    class YUNUTY_API Component :
        public Object
    {
    private:
        struct AddComponentDesc
        {
#if _DEBUG
            bool addAvailable;
#endif
            GameObject* gameObject;
            GUID guid;
        };
        static Component::AddComponentDesc addComponentDesc;
        static std::unordered_map<GUID, Component*, yutility::GenericHash<GUID>, yutility::GenericEqual<GUID>> guidPtrMap;
        GameObject* gameObject = nullptr;
        bool StartCalled = false;
        bool isActive{ true };
        GUID guid;
    protected:
        Component();
    public:
        template<typename ComponentType>
        static ComponentType* LoadReferenceByGUID(const char* guid)
        {
            UUID uuid;
            //CLSIDFromString(guid, &uuid);
            UuidFromStringA((RPC_CSTR)(guid), &uuid);
            return LoadReferenceByGUID<ComponentType>(uuid);
        }
        template<typename ComponentType>
        static ComponentType* LoadReferenceByGUID(const WCHAR* guid)
        {
            //return LoadReferenceByGUID<ComponentType>(yutility::GetString(guid).c_str());
            UUID uuid;
            CLSIDFromString(guid, &uuid);
            /*RPC_WSTR rpcStr = nullptr;
            rpcStr = (RPC_WSTR)RpcSsAllocate((wcslen(guid) + 1) * sizeof(WCHAR));
            if (rpcStr)
                wcscpy_s(reinterpret_cast<WCHAR*>(rpcStr), wcslen(guid) + 1, guid);
             */   //wcscpy_s((WCHAR*)rpcStr, wcslen(guid) + 1, guid);

            //UuidFromStringW((RPC_WSTR)(guid), &uuid);
            //RpcSsFree(rpcStr);
            return LoadReferenceByGUID<ComponentType>(uuid);
        }
        template<typename ComponentType>
        static ComponentType* LoadReferenceByGUID(GUID guid)
        {
            if (auto found = guidPtrMap.find(guid); found != guidPtrMap.end())
                return dynamic_cast<ComponentType*>(found->second);
            else
                return nullptr;
        }

        GUID GetGUID();
        std::wstring GetGUIDWStr();
        // ��Ȱ��ȭ�� ���� ������Ʈ�� Ȱ��ȭ �� �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        virtual void OnEnable() {};
        // Ȱ��ȭ�� ���� ������Ʈ�� ��Ȱ��ȭ �� �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        virtual void OnDisable() {};
        // ���� ���� �� �� �����Ӹ��� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        virtual void Update() {};
        // ������Ʈ�κ��� ���ʷ� Update�� ȣ��� ��, ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        virtual void Start() {};
        // ���� ������Ʈ�� ������ �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        virtual void OnDestroy() {};
        // ���� ���� �� ��Ȯ�� �ð� �������� ȣ��Ǵ� �ݹ� �Լ��Դϴ�. ���� �������� �ʾҽ��ϴ�.
        virtual void FixedUpdate() {};
        // �浹ü ���� �̺�Ʈ�� �Ͼ ��, �ݶ��̴��� ��ġ�� ���� ������Ʈ�� ���� �ִ� ��� Component�鿡��
        // �ش� �̺�Ʈ�� �����Ǵ� �ݹ� �Լ��� ȣ��˴ϴ�.

        // �浹ü���� ���� �浹�� ������ �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        virtual void OnCollisionEnter(const Collision& collision) {};
        virtual void OnCollisionEnter2D(const Collision2D& collision) {};
        // �浹ü���� ���� ������ ���·� ������ �� �����Ӹ��� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        virtual void OnCollisionStay(const Collision& collision) {};
        virtual void OnCollisionStay2D(const Collision2D& collision) {};
        // �浹ü���� �浹�� ��ġ�� ���� ������ ������ ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
        virtual void OnCollisionExit(const Collision& collision) {};
        virtual void OnCollisionExit2D(const Collision2D& collision) {};
        virtual ~Component();
        Transform* GetTransform();
        GameObject* GetGameObject();
        const Transform* GetTransform()const;
        const GameObject* GetGameObject()const;
        // ������Ʈ�� Ȱ��ȭ ���θ� �����ɴϴ�.
        bool GetActive();
        // ������Ʈ�� ��Ȱ��ȭ�մϴ�.
        void SetActive(bool active);

        friend GameObject;
        friend YunutyCycle;
        friend Collider2D;
        friend Collider;
    };
}
