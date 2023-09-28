#include "GraphicsPch.h"
#include "Device.h"

#include <filesystem>
#include <shlobj.h>

/// GPU ĸó��
std::wstring GetLatestWinPixGpuCapturerPath_Cpp17()
{
	LPWSTR programFilesPath = nullptr;
	SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &programFilesPath);

	std::filesystem::path pixInstallationPath = programFilesPath;
	pixInstallationPath /= "Microsoft PIX";

	std::wstring newestVersionFound;

	for (auto const& directory_entry : std::filesystem::directory_iterator(pixInstallationPath))
	{
		if (directory_entry.is_directory())
		{
			if (newestVersionFound.empty() || newestVersionFound < directory_entry.path().filename().c_str())
			{
				newestVersionFound = directory_entry.path().filename().c_str();
			}
		}
	}

	if (newestVersionFound.empty())
	{
		// TODO: Error, no PIX installation found
	}

	return pixInstallationPath / newestVersionFound / L"WinPixGpuCapturer.dll";
}


void Device::Init()
{
	///if (GetModuleHandle(L"WinPixGpuCapturer.dll") == 0)
	///{
	///	LoadLibrary(GetLatestWinPixGpuCapturerPath_Cpp17().c_str());
	///}

	// D3D12 ������� Ȱ��ȭ
// VC++ ���â�� ���� ����� �޼��� ���
// riid : ����̽��� COM ID
// ppDevice : ������ ��ġ�� �Ű������� ����
#ifdef _DEBUG
	::D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController));
	m_debugController->EnableDebugLayer();
#endif

	// DXGI(DirectX Graphics Infrastructure)
	// Direct3D�� �Բ� ���̴� API
	// ��üȭ�� ��� ��ȯ
	// �����Ǵ� ���÷��� ��� ����
	// CreateDXGIFactory
	// riid : ����̽��� COM ID
	// ppDevice : ������ ��ġ�� �Ű������� ����
	::CreateDXGIFactory(IID_PPV_ARGS(&m_dxgi));

	// CreateDevice
	// ���÷��� ����� (�׷��� ī��)�� ��Ÿ���� ��ü
	// pAdapter : nullptr�� �����ϸ� �ý��� �⺻ ���÷��� �����
	// MinimumFeatureLevel : ���� ���α׷��� �䱸�ϴ� �ּ� ��� ����
	// riid : ����̽��� COM ID
	// ppDevice : ������ ��ġ�� �Ű������� ����
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
}
