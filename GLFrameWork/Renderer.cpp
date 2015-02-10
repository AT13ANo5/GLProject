#include "Renderer.h"
#include "Object.h"
#include "Fade.h"

HDC		CRenderer::OpenGLDevice	=NULL;
HGLRC	CRenderer::OpenGLContext=NULL;

HRESULT CRenderer::Init(HWND hWnd)
{
	// OpenGL初期化
	// ピクセルフォーマット初期化
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
		PFD_TYPE_RGBA, //ピクセルフォーマットの種類
		32, //フレームバッファの深度(?)
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24, //深度バッファに使用するビット数
		8, //ステンシルバッファに使用するビット数
		0, //補助バッファの数
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	OpenGLDevice = GetDC(hWnd);

	int format = ChoosePixelFormat(OpenGLDevice, &pfd);

	if (format == 0)
	{
		return E_FAIL; // 該当するピクセルフォーマットが無い
	}

	// OpenGLが使うデバイスコンテキストに指定のピクセルフォーマットをセット
	if (!SetPixelFormat(OpenGLDevice, format, &pfd))
	{
		return E_FAIL; // DCへフォーマットを設定するのに失敗
	}

	// OpenGLのコンテキストを作成
	OpenGLContext = wglCreateContext(OpenGLDevice);
	// 作成されたコンテキストが現在使用中のコンテキストか？
	if (!wglMakeCurrent(OpenGLDevice, OpenGLContext))
	{
		return E_FAIL; // 何か正しくない
	}
	//OpenGL描画設定
	glEnable(GL_CULL_FACE);//カリングする
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glAlphaFunc(GL_GREATER,GLclampf(0.1));
	

	CFade::Initialize();

	return S_OK;
}

void CRenderer::Uninit(HWND hWnd)
{
	CObject::ReleaseAll();
	CFade::Finalize();

	// カレントコンテキストを無効にする
	wglMakeCurrent(NULL, NULL);
	// カレントコンテキストを削除
	wglDeleteContext(OpenGLContext);
	// デバイスコンテキスト解放
	ReleaseDC(hWnd, OpenGLDevice);
}

void CRenderer::Update(void)
{
	CObject::UpdateAll();
	CFade::Instance().Update();
}
void CRenderer::Draw(void)
{
	//画面のクリア
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//RGBA
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	CObject::DrawAll();
	CFade::Instance().Draw();

	SwapBuffers(OpenGLDevice);//バッファの入れ替え

}