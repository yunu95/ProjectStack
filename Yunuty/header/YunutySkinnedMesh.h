#pragma once
#include "Object.h"
#include "Component.h"
#include "Rect.h"
#include "YunuGraphicsInterface.h"
#include "IYunuGISkinnedMesh.h"
#include "YunutyRenderable.h"

// ī�޶�� ȭ�鿡 ���ӻ� �����ϴ� ��� �׷��� ��ҵ��� ����ϴ� ���� ���̴� ��ü�Դϴ�. 
// ī�޶��� ������ ������ ����ȯ��,Ȥ�� ī�޶��� Ư���� ���� �޶��� �� �ֱ� ������,
// ȭ���� �׷����� ���� ���̴� �Լ� Render�� �߻� �޼ҵ�� ���ǵ˴ϴ�.
#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace yunutyEngine::graphics
{
    class YunutyCycle;
    class YUNUTY_API SkinnedMesh : public Renderable<yunuGI::ISkinnedMesh>
    {
    protected:
    public:
        // GI�� Graphics Interface��� ����.
        yunuGI::ISkinnedMesh& GetGI() { return Renderable<yunuGI::ISkinnedMesh>::GetGI(); }
        SkinnedMesh();
    };
}
