/// 2023. 07. 09. �����
/// ���� �������� ���� ������ ���� ���ȭ�� �г�

import ui.EditorPanel;
export module ui.ProjectPanel;

export class ProjectPanel
	: public EditorPanel
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnUpdate(float ts) override;
	virtual void DrawPanel() override;
};