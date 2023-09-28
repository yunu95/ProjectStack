#pragma once
#include "imgui.h"

/// ��� â������ debug ��带 �����ؼ� �ؽ�Ʈ�� ����� �� �ֵ���
/// 2023.08.23 ����ȭ

class DebugManager
{
private:
	DebugManager();
	~DebugManager();

	static DebugManager* instance;


public:
	static DebugManager* GetInstance();
	static void Delete();

	void DrawDebugText(const char* text, ...);

	const bool GetDebugMode();
	void SetDebugMode(bool tf);

	bool m_isPlayMode = false;

private:
	bool m_isDebugMode;
};
