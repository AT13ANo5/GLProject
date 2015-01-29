//=============================================================================
//
//
//Creater:Arai Yuhki
//=============================================================================
//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#include "ManagerGL.h"
#include "Mouse.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
//=============================================================================
//マクロ定義
//=============================================================================
#define GAME_WINDOW (WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU)//ウィンドウタイプ
//=============================================================================
// プロトタイプ宣言
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================================================
//グローバル変数
//=============================================================================
unsigned int CurrentTime = 0;//DWORD dwCurrentTime;
unsigned int PrevTime = 0;	 //DWORD dwExecLastTime;
unsigned int FPSLastTime = 0;//DWORD dwFPSLastTime;
unsigned int FrameCount = 0; //DWORD dwFrameCount;
CManager* pManager = nullptr;
//=============================================================================
//メイン
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(487);	//メモリを確保した場所を探すときは、この行のコメントを外して何番目に確保したかの番号を括弧内に書く

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+0),
		NULL,
		"Test",
		NULL
	};

	HWND hWnd;
	MSG msg;

	RegisterClassEx(&wcex);
	hWnd = CreateWindowEx(0,
		"Test",
		WINDOW_TITLE,
		GAME_WINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(int)SCREEN_WIDTH,
		(int)SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);
	pManager = new CManager;
	pManager->Init(hInstance,hWnd);
	
	timeBeginPeriod(1);				// 分解能を設定
	

	FPSLastTime =
	CurrentTime = timeGetTime();
	
	PrevTime = 0;
	
	FrameCount = 0;

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	CMouse::SetHWND(hWnd);
	
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			CurrentTime = timeGetTime();
			if((CurrentTime - FPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
				FPSLastTime = CurrentTime;
				FrameCount = 0;
			}
			
			if(CurrentTime - PrevTime >= 1000/60)
			{
				

				//更新
				pManager->Update();
				//描画
				pManager->Draw();
				
				
				PrevTime = CurrentTime;
			}
		
		}
	}

	pManager->Uninit(hWnd);
	delete pManager;
	pManager = nullptr;
	timeEndPeriod(1);// 分解能を戻す

	UnregisterClass("Test",wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//  (CALLBACK : Win32API関数を呼び出す時の規約)
//  hWnd   : ウィンドウのハンドル
//  uMsg   : メッセージの識別子
//  wParam : メッセージの最初のパラメータ
//  lParam : メッセージの2番目のパラメータ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);			// "WM_QUIT"メッセージを返す
		break;
	
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
	break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		// 既定の処理を提供
}
