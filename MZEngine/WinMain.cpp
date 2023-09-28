/// 2023.06.16
/// ������� ���� MZEngine
/// 
// WinMain.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.

#include "GameProcess.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ���� ���� Ŭ����
	// ���ο��� ���� ������ �޽��� �ݹ��� ó���Ѵ�.
	// ���� �׷��Ƚ� ���� ���� �����Ѵ�.
	GameProcess* pGameProcess = new GameProcess();
	pGameProcess->Initialize(hInstance);
	pGameProcess->Loop();
	pGameProcess->Finalize();

	delete pGameProcess;
}