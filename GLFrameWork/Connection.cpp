//=============================================================================
//	Connectionファイル [ Connection.h ]
//	Auther : KOTARO NAGASAKI
//=============================================================================

//*****************************************************************************
//	インクルード
//*****************************************************************************
#include "Connection.h"
#include "Polygon2D.h"
#include "CPushStart.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "netData.h"
#include "ManagerGL.h"

//*****************************************************************************
//	定数定義
//*****************************************************************************
static const int connectionPlayerMax = 6;	//	プレイヤー最大数
static const float windowWidth = 256.0f;	//	画像オブジェクトの横幅
static const float windowHeight = 231.0f;	//	画像オブジェクトの縦幅
static const float waitPlayerWidth = 256.0f;	//	画像オブジェクトの横幅
static const float waitPlayerHeight = 231.0f;	//	画像オブジェクトの縦幅

//	画像オブジェクト配置の基準位置
static const VECTOR3 windowPos[connectionPlayerMax] =
{
	VECTOR3(270.0f, 410.0f, 0),
	VECTOR3(755.0f, 300.0f, 0),
	VECTOR3(1040.0f, 300.0f, 0),
	VECTOR3(635.0f, 505.0f, 0),
	VECTOR3(885.0f, 505.0f, 0),
	VECTOR3(1130.0f, 505.0f, 0),
};

//	画像オブジェクト配置の基準位置
static const VECTOR3 waitPlayerPos[connectionPlayerMax] =
{
	windowPos[0],
	windowPos[1],
	windowPos[2],
	windowPos[3],
	windowPos[4],
	windowPos[5],
};

//*****************************************************************************
//	変数定義
//*****************************************************************************

//=============================================================================
//	コンストラクタ
//=============================================================================
CConnection::CConnection()
{
	Logo = nullptr;
	pushStart = nullptr;
	backGround = nullptr;
}

//=============================================================================
//	デストラクタ
//=============================================================================
CConnection::~CConnection()
{
}

//=============================================================================
//	初期化処理
//=============================================================================
void CConnection::Init(void)
{
	//	背景画像の配置
	//-------------------------------------------------
	backGround = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0),
		VECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	backGround->SetTex(CTexture::Texture(TEX_YOUJO_BG));
	//-------------------------------------------------

	//	文字オブジェクト「ロゴ」の配置
	//-------------------------------------------------
	Logo = CPolygon2D::Create(VECTOR3(670.0f, 90.0f, 0),
		VECTOR2(750.0f, 125.0f));
	Logo->SetTex(CTexture::Texture(TEX_YOUJO_LOGO));
	//-------------------------------------------------

	//	文字オブジェクト「PUSHSTART」の配置
	//-------------------------------------------------
	pushStart = CPushStart::Create(VECTOR3(655.0f, 690.0f, 0),
		VECTOR2(325.0f, 50.0f));
	pushStart->SetTex(CTexture::Texture(TEX_YOUJO_ENTER));
	//-------------------------------------------------

	//	サーバーへエントリー情報を送信
	CManager::SendEntry();

	//	画像オブジェクト群の配置
	//-------------------------------------------------
	{
		waitPlayer = new CPolygon2D*[connectionPlayerMax];

		waitPlayer[0] = CPolygon2D::Create(waitPlayerPos[0], VECTOR2(waitPlayerWidth * 2.0f, waitPlayerHeight * 2.0f));
		waitPlayer[0]->SetTex(CTexture::Texture(TEX_YOUJO_RINCHAN));

		waitPlayer[1] = CPolygon2D::Create(waitPlayerPos[1], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[1]->SetTex(CTexture::Texture(TEX_YOUJO_BLUE));

		waitPlayer[2] = CPolygon2D::Create(waitPlayerPos[2], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[2]->SetTex(CTexture::Texture(TEX_YOUJO_ORANGE));

		waitPlayer[3] = CPolygon2D::Create(waitPlayerPos[3], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[3]->SetTex(CTexture::Texture(TEX_YOUJO_RED));

		waitPlayer[4] = CPolygon2D::Create(waitPlayerPos[4], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[4]->SetTex(CTexture::Texture(TEX_YOUJO_BLUE));

		waitPlayer[5] = CPolygon2D::Create(waitPlayerPos[5], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[5]->SetTex(CTexture::Texture(TEX_YOUJO_WHITE));
	}
	//-------------------------------------------------

	//	画像オブジェクト群の配置
	//-------------------------------------------------
	{
		waitBackGround = new CPolygon2D*[connectionPlayerMax];
		waitBackGround[0] = CPolygon2D::Create(windowPos[0],VECTOR2(windowWidth * 2.0f, windowHeight * 2.0f));
		waitBackGround[0]->SetTex(CTexture::Texture(TEX_YOUJO_CPU));

		waitBackGround[1] = CPolygon2D::Create(windowPos[1], VECTOR2(windowWidth, windowHeight));
		waitBackGround[1]->SetTex(CTexture::Texture(TEX_YOUJO_CPU));

		waitBackGround[2] = CPolygon2D::Create(windowPos[2], VECTOR2(windowWidth, windowHeight));
		waitBackGround[2]->SetTex(CTexture::Texture(TEX_YOUJO_CPU));

		waitBackGround[3] = CPolygon2D::Create(windowPos[3], VECTOR2(windowWidth, windowHeight));
		waitBackGround[3]->SetTex(CTexture::Texture(TEX_YOUJO_CPU));

		waitBackGround[4] = CPolygon2D::Create(windowPos[4], VECTOR2(windowWidth, windowHeight));
		waitBackGround[4]->SetTex(CTexture::Texture(TEX_YOUJO_CPU));

		waitBackGround[5] = CPolygon2D::Create(windowPos[5], VECTOR2(windowWidth, windowHeight));
		waitBackGround[5]->SetTex(CTexture::Texture(TEX_YOUJO_CPU));
	}
	//-------------------------------------------------
}

//=============================================================================
//	終了処理
//=============================================================================
void CConnection::Uninit(void)
{
	if (pushStart != nullptr)
	{
		pushStart->Release();
		pushStart = nullptr;
	}

	if (Logo != nullptr)
	{
		Logo->Release();
		Logo = nullptr;
	}

	for (int count = 0; count < connectionPlayerMax; count++)
	{
		if (waitBackGround[count] != nullptr)
		{
			waitBackGround[count]->Release();
			waitBackGround[count] = nullptr;
		}

		if (waitPlayer[count] != nullptr)
		{
			waitPlayer[count]->Release();
			waitPlayer[count] = nullptr;
		}
	}

	if (waitPlayer != nullptr)
	{
		delete[] waitPlayer;
		waitPlayer = nullptr;
	}

	if (waitBackGround != nullptr)
	{
		delete[] waitBackGround;
		waitBackGround = nullptr;
	}

	if (backGround != nullptr)
	{
		backGround->Release();
		backGround = nullptr;
	}
}
//=============================================================================
//	更新処理
//=============================================================================
void CConnection::Update(void)
{
	recvUpdate();
	keyUpdate();
}
//=============================================================================
//	サーバーからの受信状況による更新処理
//=============================================================================
void CConnection::recvUpdate()
{
	//	ネットワーク関連のデータを取得
	NETWORK_DATA* netWorkData = CManager::getNetWorkData();

	//	部屋がいっぱいなら
	if (netWorkData->emptyFlag == true)
	{
		//	部屋が一杯
	}
}
//=============================================================================
//	キー押下のよる更新処理
//=============================================================================
void CConnection::keyUpdate()
{
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_GAME);
	}
}

//	EOF