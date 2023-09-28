#pragma once
#include "YunuGraphicsInterface.h"
#include "IYunuGIRenderable.h"
#include "YGRenderableAdapter.h"

namespace yunuGIAdapter
{
	class StaticMeshAdapter : public yunuGIAdapter::RenderableAdapter, public yunuGI::IStaticMesh
	{
	public:
		StaticMeshAdapter() :RenderableAdapter() { }
		virtual void SetWorldTM(const yunuGI::Matrix4x4& worldTM) { };
		virtual void SetActive(bool isActive) {};
		virtual void LoadMesh(const char* fileName) {};
		virtual void LoadDiffuseMap(const char* fileName) {}
		virtual void LoadNormalMap(const char* fileName) {}
		virtual void SetPickingMode(bool isPickingModeOn) {}
	private:
	};
}
