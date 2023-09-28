#pragma once
#include "Component.h"
#include "YunutyRenderable.h"
#include "YunuGraphicsInterface.h"

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
	class YUNUTY_API UIText : public Renderable<yunuGI::IUIText>
	{
	private:
	protected:
		virtual void Update() override;
	public:
		// GI�� Graphics Interface��� ����.
		yunuGI::IUIText& GetGI() { return Renderable<yunuGI::IUIText>::GetGI(); }
		UIText();
	};
}
