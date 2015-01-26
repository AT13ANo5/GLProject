//=============================================================================
//メインヘッダー
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS	//警告対策
#define DIRECTINPUT_VERSION (0x0800)// 警告対策用
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//=============================================================================
//ライブラリのリンク
//=============================================================================
#pragma comment(lib, "openGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment ( lib , "ws2_32.lib" )

//=============================================================================
//インクルード
//=============================================================================
#include <winsock2.h>
#include <ws2tcpip.h>
#include<windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "dinput.h"
#include "CommonGL.h"
#include "Console.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define SCREEN_WIDTH (1280.0f)	//ウィンドウの幅
#define SCREEN_HEIGHT (720.0f)	//ウィンドウの高さ
#define WINDOW_TITLE ("OpenGLテスト")

//=============================================================================
//構造体宣言
//=============================================================================
struct MATERIAL//マテリアル
{
	COLOR ambient,diffuse,specular,emission;
	float shininess;
};

#endif