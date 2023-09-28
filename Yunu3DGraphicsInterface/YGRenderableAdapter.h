#pragma once
#include <unordered_map>
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"

namespace yunuGIAdapter
{
	class RenderableAdapter : virtual public yunuGI::IRenderable
	{
	public:
		RenderableAdapter() { }
		static RenderableAdapter* GetWrapper() { }
	private:
	};
}
