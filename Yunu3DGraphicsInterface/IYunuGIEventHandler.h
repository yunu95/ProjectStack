#pragma once
#include "IUnknown.h"
namespace yunuGI
{
	class IRenderable;
	// EventHandler�� �׷��Ƚ� ���� ���ο����� ó���Ҽ� �ִ�
	// �̺�Ʈ(Ŭ��, �ø� ��)�� ���� �ݹ� �Լ��� �����ϰ�, �̺�Ʈ ��û�� ó���Ѵ�.
    struct IEventHandlerDesc
    {
    };
	class IEventHandler : public IUnknown
	{
	public:
		typedef void(*CallbackPtr)(IRenderable*);
		virtual void AddClickCallback(CallbackPtr callback) {};

		// call ClickCallback function registered by clients request,
		// considering origin point is located at topleft, and x,y coordinates are clamped between 1.0 and 0.0
		virtual yunuGI::IRenderable* Click(float normalizedX, float normalizedY) { return nullptr; }
	};
}