/// ���� Main Camera ���������� ȭ���� �����ִ� �г�

import ui.EditorPanel;
export module ui.GameViewPanel;

export class GameViewPanel
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
