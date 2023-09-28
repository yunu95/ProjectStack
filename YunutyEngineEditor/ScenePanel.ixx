/// 2023. 07. 09. �����
/// ���Ӱ� ������ ������ ������ ���� ����ϴ� ī�޶� ���������� ȭ���� �����ִ� �г�
/// �̱����̹Ƿ� EditorCamera �� ���� ������
 
import ui.EditorPanel;
export module ui.ScenePanel;

export class ScenePanel
	: public EditorPanel
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnUpdate(float ts) override;
	virtual void DrawPanel() override;

private:
	void ReturnMousePos();

private:
	float m_gameImageSizeX = 0.0f;
	float m_gameImageSizeY = 0.0f;
};
