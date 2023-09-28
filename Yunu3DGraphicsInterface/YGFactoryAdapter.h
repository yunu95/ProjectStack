#pragma once
//#include "YunuGICameraAdapter.h"
#include "YunuGraphicsInterface.h"
#include "YGCameraAdapter.h"
#include "YGStaticMeshAdapter.h"
#include "YGSkinnedMeshAdapter.h"
#include "YGDebugCubeAdapter.h"
#include "YGUIImageAdapter.h"
#include "YGUITextAdapter.h"
#include "YGUIImageAdapter.h"
#include "YGUITextAdapter.h"
#include "YGDebugCubeAdapter.h"
#include "IYunuGIDebugCube.h"
#include "IYunuGIUIImage.h"

namespace yunuGIAdapter
{
	class FactoryAdapter : public yunuGI::IFactory
	{
	public:
		virtual yunuGI::ICamera* CreateCamera(const yunuGI::ICameraDesc& desc) {
			return new yunuGIAdapter::CameraAdapter();
		}
		virtual yunuGI::IStaticMesh* CreateStaticMesh(const yunuGI::IStaticMeshDesc& desc)
		{
			return new StaticMeshAdapter{};
		}
		virtual yunuGI::ISkinnedMesh* CreateSkinnedMesh(const yunuGI::ISkinnedMeshDesc& desc)
		{
			return new SkinnedMeshAdapter{};
		}
		virtual yunuGI::IDebugCube* CreateDebugCube(const yunuGI::IDebugCubeDesc& desc)
		{
			return new DebugCubeAdapter{};
		}
		virtual yunuGI::IUIImage* CreateUIImage(const yunuGI::IUIImageDesc& desc)
		{
			return new UIImageAdapter{};
		}
		virtual yunuGI::IUIText* CreateUIText(const yunuGI::IUITextDesc& desc)
		{
			return new UITextAdapter{};
		}
		virtual yunuGI::IDirectionalLight* CreateDirectionalLight(const yunuGI::IDirectionalLightDesc& desc) 
		{ return nullptr; }
	};
}