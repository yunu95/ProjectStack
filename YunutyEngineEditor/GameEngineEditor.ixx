/// 2023. 07. 23. ������
/// ���� ������ �۾����� �޾� ó������ �������� �׾� ������� ��ȹ
/// 
/// 2023. 07. 09. �����
/// Walnut API �� ����Ͽ� ó������ �������� �׾� ������� ����
/// Walnut �� EntryPoint �� �ְ�, Application �� �������Ͽ� �����
module;
#include "Walnut/Application.h"
//#include "Walnut/EntryPoint.h"
#include <d3d11.h>
#include "Walnut/Image.h"
export module editor.GameEngineEditor;

export class GameEngineEditor
	: public Walnut::Application
{
public:
	GameEngineEditor(const Walnut::ApplicationSpecification& applicationSpecification = Walnut::ApplicationSpecification());
	virtual ~GameEngineEditor();

	void ApplyDefaultLayout();
	void SaveLayout();
	void LoadLayout();
	void GetSharedHandleTest(void* handle);
	void InitTestDX();
	ID3D11Device* testD3DDevice{nullptr};
	ID3D11DeviceContext* testD3DDeviceContext{nullptr};
    ID3D11Texture2D* testD3DTexture{ nullptr };
    ID3D11ShaderResourceView* testD3DSrv{ nullptr };
	ID3D11RenderTargetView* testRTV{ nullptr };
	static GameEngineEditor* testInstance;
	static VkDescriptorSet testDescriptorSet;
    static VkImageView testImageView;
private:
	bool storability = false;
	bool objectSelection = false;
	bool objectArchived = false;
	std::vector<int> selectedObjectList;
	std::vector<int> archivedObjectList;
	std::vector<int> undoStack;
	std::vector<int> redoStack;

	friend Walnut::Application* Walnut::CreateApplication(int argc, char** argv);
};
