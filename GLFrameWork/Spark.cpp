//=============================================================================
// CSpark�N���X [Spark.cpp]
// Author :Mai Tanabe
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "Spark.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define EFFECT_TEX			(TEX_SPARK)
#define EFFECT_SIZE			(10.0f)
#define EFFECT_WIDTH		(0.5f)
#define EFFECT_HEIGHT		(1.0f)
#define EFFECT_1_FRAME		(30)		// 1�̏�Ԃ̃t���[����
#define EFFECT_LIFE_FRAME	(60)	// �����t���[����

//=============================================================================
// ����
//=============================================================================
void CSpark::Create(const VECTOR3& pos)
{
	CSpark* pointer = new CSpark;

	pointer->SetPos(pos + VECTOR3(0.0f, 10.0f, 0.0f));
	pointer->SetRot(0.0f, 0.0f, 0.0f);
	pointer->SetSize(EFFECT_SIZE, EFFECT_SIZE);
	pointer->SetColor(COLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pointer->SetUV(0.0f, 0.0f, EFFECT_WIDTH, EFFECT_HEIGHT);
	pointer->SetTex(CTexture::Texture(EFFECT_TEX));

	pointer->Init();
}

//=============================================================================
// ������
//=============================================================================
void CSpark::Init(void)
{
	// �e�N���X������
	CBillboard::Init();
}

//=============================================================================
// �I��
//=============================================================================
void CSpark::Uninit(void)
{
	// �e�N���X�̏I��
	CBillboard::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CSpark::Update(void)
{
	// �J�E���g�A�b�v
	m_cntAnim++;

	// �e�N�X�`���A�j���[�V����
	VECTOR2	texcord;
	int		nowTex = m_cntAnim / EFFECT_1_FRAME;

	texcord = VECTOR2((int)(nowTex) * EFFECT_WIDTH,
					  (int)((nowTex) * EFFECT_WIDTH) * EFFECT_HEIGHT);

	SetUV_X(texcord.x);
	SetUV_Y(texcord.y);

	if(m_cntAnim >= EFFECT_LIFE_FRAME)
	{
		Release();
	}
}

//=============================================================================
// �`��
//=============================================================================
void CSpark::Draw(void)
{
	// �e�N���X�̕`��
	CBillboard::Draw();
}