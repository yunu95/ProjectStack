#pragma once
#include <windows.h>

/// <summary>
/// ������ ���� ���� ����
/// ���� ���� ������ ����Ǵ� ��
/// </summary>

namespace MZ3DAPI
{
	class I3DRenderer;
}

class GameTimer;

class GameProcess
{
private:
	HWND m_hWnd;
	MSG m_msg;

	// ������ �ʱ� ��ġ
	const int m_windowPosX = 0;
	const int m_windowPosY = 0;

	// ������ â ũ��
	const int m_screenWidth = 1920;
	const int m_screenHeight = 1080;

	// �׷��Ƚ� ����
	MZ3DAPI::I3DRenderer* m_pRenderer;
	static MZ3DAPI::I3DRenderer* m_pRendererInstance;

	// �ð� ����
	GameTimer* m_pGameTimer;

private:
	void RecalcWindowSize();

	void UpdateAll();
	void RenderAll();

public:
	// �ʱ�ȭ
	HRESULT Initialize(HINSTANCE hInstance);

	// ���� ������ ����Ǵ� �ݺ�����
	void Loop();

	// ���� ����
	void Finalize();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

