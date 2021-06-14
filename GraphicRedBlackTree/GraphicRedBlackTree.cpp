// GraphicRedBlackTree.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GraphicRedBlackTree.h"
#include "RedBlackTree.h"
#include "LinkedList.h"
#include <time.h>

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
RedBlackTree g_cTree;
RECT g_rect;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)

{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICREDBLACKTREE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Graphic";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);


	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(L"Graphic", L"Graphic", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	GetClientRect(hWnd, &g_rect);
	srand((unsigned)time(NULL));

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICREDBLACKTREE));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

CList<int> g_List;

int insertData = -1;
int DeleteData = -1;

BOOL bLClick = false;
BOOL bRClick = false;

HWND hCreate;
HWND hDelete;

TCHAR createMsg[20];
TCHAR deleteMsg[20];

void CreateNode(int number = -1)
{
	if (number != -1)
	{
		insertData = number;
		bLClick = true;
	}

	if (bLClick == true)
	{
		CList<int>::iterator iter;
		for (iter = g_List.begin(); iter != g_List.end(); ++iter)
		{
			if ((*iter) == insertData)
			{
				bLClick = false;
				insertData = -1;
				return;
			}
		}

		g_List.push_back(insertData);
		g_cTree.InsertNode(insertData);
		insertData = -1;
		bLClick = false;
	}
	else
	{
		insertData = rand() % 1000;
		bLClick = true;
	}
}


void DeleteNode(int number = -1)
{
	if (number != -1)
	{
		DeleteData = number;
		bRClick = true;
	}

	if (bRClick == true)
	{
		CList<int>::iterator iter = g_List.begin();
		for (iter = g_List.begin(); iter != g_List.end(); ++iter)
		{
			if ((*iter) == DeleteData)
			{
				g_cTree.DeleteNode(DeleteData);
				g_List.remove(DeleteData);
				break;
			}
		}

		DeleteData = -1;
		bRClick = false;
	}
	else
	{
		if (g_List.size() != 0)
		{
			int key = rand() % g_List.size();
			CList<int>::iterator iter = g_List.begin();
			for (int i = 0; i < key; i++)
			{
				++iter;
			}

			DeleteData = (*iter);
			bRClick = true;
		}
		else
		{
			DeleteData = -1;
		}
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		CreateWindow(L"button", L"생성", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			230, 600, 100, 25, hWnd, (HMENU)0, hInst, NULL);
		CreateWindow(L"button", L"삭제", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			230, 630, 100, 25, hWnd, (HMENU)1, hInst, NULL);

		hCreate = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 10, 600, 200, 25, hWnd, (HMENU)100, hInst, NULL);
		hDelete = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 10, 630, 200, 25, hWnd, (HMENU)101, hInst, NULL);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0:
			GetWindowText(hCreate, createMsg, 20);
			CreateNode(_ttoi(createMsg));
			SetWindowText(hCreate, L"");
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case 1:
			GetWindowText(hDelete, deleteMsg, 20);
			DeleteNode(_ttoi(deleteMsg));
			SetWindowText(hDelete, L"");
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		CreateNode();
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_RBUTTONDOWN:
		DeleteNode();
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

		g_cTree.PreorderPrint(hdc, g_rect);

		if (bLClick == true)
		{
			TCHAR data[20];
			wsprintf(data, TEXT("New Node : %d"), insertData);
			TextOut(hdc, 0, 0, data, wcslen(data));
		}

		if (bRClick == true)
		{
			TCHAR data[20];
			wsprintf(data, TEXT("Delete Node : %d"), DeleteData);
			TextOut(hdc, 0, 20, data, wcslen(data));
		}

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



