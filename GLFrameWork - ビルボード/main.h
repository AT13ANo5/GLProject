//=============================================================================
//メインヘッダー
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS	//警告対策
#define DIRECTINPUT_VERSION (0x0800)// 警告対策用
//=============================================================================
//ライブラリのリンク
//=============================================================================
#pragma comment(lib, "openGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
//=============================================================================
//インクルード
//=============================================================================
#include<windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "dinput.h"
#include "CommonGL.h"
#include "Console.h"
//=============================================================================
//マクロ定義
//=============================================================================
#define SCREEN_WIDTH (800.0f)	//ウィンドウの幅
#define SCREEN_HEIGHT (600.0f)	//ウィンドウの高さ
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