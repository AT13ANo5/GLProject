//=============================================================================
// CSandCloudクラス [SandCloud.cpp]
// Author :Mai Tanabe
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "SandCloud.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define EFFECT_TEX			(TEX_SAND_CLOUD)
#define EFFECT_SIZE			(30.0f)
#define EFFECT_WIDTH		(0.25f)
#define EFFECT_HEIGHT		(1.0f)
#define EFFECT_1_FRAME		(4)	// 1つの状態のフレーム数
#define EFFECT_LIFE_FRAME	(16)	// 寿命フレーム数

//=============================================================================
// 生成
//=============================================================================
void CSandCloud::Create(const VECTOR3& pos)
{
	CSandCloud* pointer = new CSandCloud;

	pointer->SetPos(pos + VECTOR3(0.0f, 10.0f, 0.0f));
	pointer->SetRot(0.0f, 0.0f, 0.0f);
	pointer->SetSize(EFFECT_SIZE, EFFECT_SIZE);
	pointer->SetColor(COLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pointer->SetUV(0.0f, 0.0f, EFFECT_WIDTH, EFFECT_HEIGHT);
	pointer->SetTex(CTexture::Texture(EFFECT_TEX));

	pointer->Init();
}

//=============================================================================
// 初期化
//=============================================================================
void CSandCloud::Init(void)
{
	// 親クラス初期化
	CBillboard::Init();
}

//=============================================================================
// 終了
//=============================================================================
void CSandCloud::Uninit(void)
{
	// 親クラスの終了
	CBillboard::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CSandCloud::Update(void)
{
	// カウントアップ
	m_cntAnim++;

	// テクスチャアニメーション
	VECTOR2	texcord;
	int		nowTex = m_cntAnim / EFFECT_1_FRAME;

	texcord = VECTOR2((int)(nowTex)* EFFECT_WIDTH,
					  (int)((nowTex)* EFFECT_WIDTH) * EFFECT_HEIGHT);

	SetUV_X(texcord.x);
	SetUV_Y(texcord.y);

	if(m_cntAnim >= EFFECT_LIFE_FRAME)
	{
		Release();
	}
}

//=============================================================================
// 描画
//=============================================================================
void CSandCloud::Draw(void)
{
	// 親クラスの描画
	CBillboard::Draw();
}