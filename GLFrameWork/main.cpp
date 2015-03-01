//=============================================================================
//
//
//Creater:Arai Yuhki
//=============================================================================
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "GL\include\GL\glut.h"
#include "resource.h"
#include "main.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "ManagerGL.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
//=============================================================================
//�}�N����`
//=============================================================================
#define GAME_WINDOW (WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU)//�E�B���h�E�^�C�v
//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================================================
//�O���[�o���ϐ�
//=============================================================================
unsigned int CurrentTime = 0;//DWORD dwCurrentTime;
unsigned int PrevTime = 0;	 //DWORD dwExecLastTime;
unsigned int FPSLastTime = 0;//DWORD dwFPSLastTime;
unsigned int FrameCount = 0; //DWORD dwFrameCount;
unsigned int CountFPS = 0;			// FPS�J�E���^

CManager* pManager = nullptr;
//=============================================================================
//���C��
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(714);	//���������m�ۂ����ꏊ��T���Ƃ��́A���̍s�̃R�����g���O���ĉ��ԖڂɊm�ۂ������̔ԍ������ʓ��ɏ���
 int argc = 1;
 char* argv[] = {"�ǂ�����I�悤����"};
 glutInit(&argc,argv);
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

 RECT rect;
 rect.top = 0;
 rect.left = 0;
 rect.bottom = SCREEN_HEIGHT;
 rect.right = SCREEN_WIDTH;

 AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW &~( WS_MAXIMIZEBOX | WS_THICKFRAME ),false);

 wcex.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1)); /* �A�C�R�� */
 RegisterClassEx(&wcex);
	hWnd = CreateWindowEx(0,
		"Test",
		WINDOW_TITLE,
		GAME_WINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
  rect.right - rect.left,
  rect.bottom - rect.top,
  NULL,
		NULL,
		hInstance,
		NULL);
	pManager = new CManager;
	pManager->Init(hInstance,hWnd);
	
	timeBeginPeriod(1);				// ����\��ݒ�
	

	FPSLastTime =
	CurrentTime = timeGetTime();
	
	PrevTime = 0;
	
	FrameCount = 0;

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			CurrentTime = timeGetTime();
			if((CurrentTime - FPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
    CountFPS = FrameCount * 1000 / ( CurrentTime - FPSLastTime );
    Console::SetCursorPos(0,0);
    Console::Print("FPS:%d\n",CountFPS);
    FPSLastTime = CurrentTime;
    FrameCount = 0;

			}
			
			if(CurrentTime - PrevTime >= 1000/60)
			{

				//�X�V
				pManager->Update();
				//�`��
				pManager->Draw();
				
				
				PrevTime = CurrentTime;
    FrameCount++;

			}
		
		}
	}

	pManager->Uninit(hWnd);
	delete pManager;
	pManager = nullptr;
	timeEndPeriod(1);// ����\��߂�

	UnregisterClass("Test",wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//  (CALLBACK : Win32API�֐����Ăяo�����̋K��)
//  hWnd   : �E�B���h�E�̃n���h��
//  uMsg   : ���b�Z�[�W�̎��ʎq
//  wParam : ���b�Z�[�W�̍ŏ��̃p�����[�^
//  lParam : ���b�Z�[�W��2�Ԗڂ̃p�����[�^
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);			// "WM_QUIT"���b�Z�[�W��Ԃ�
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

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		// ����̏������
}
