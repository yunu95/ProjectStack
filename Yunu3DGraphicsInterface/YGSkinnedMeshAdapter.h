#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"
#include "YGRenderableAdapter.h"

namespace yunuGIAdapter
{
	class SkinnedMeshAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::ISkinnedMesh
	{
	public:
		SkinnedMeshAdapter() :RenderableAdapter() { }
		~SkinnedMeshAdapter() { }
		virtual void SetWorldTM(const yunuGI::Matrix4x4& worldTM) { };
		virtual void SetActive(bool isActive) {};
		virtual void LoadMesh(const char* fileName) {};
		virtual void LoadDiffuseMap(const char* fileName) {}
		virtual void LoadNormalMap(const char* fileName) {}
		virtual void SetAnimation(const char* animID) {}
		virtual void SetAnimationRepeat(bool repeat) {}
		virtual void SetPickingMode(bool isPickingModeOn) {}
		virtual void PlayAnimation() {}
	private:
	};
}
