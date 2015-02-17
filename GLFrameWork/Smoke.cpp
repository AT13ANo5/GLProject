//=============================================================================
// CSmoke�N���X [Smoke.cpp]
// Author :Mai Tanabe
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "Smoke.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define EFFECT_TEX			(TEX_SMOKE)
#define EFFECT_SIZE			(25.0f)
#define EFFECT_WIDTH		(0.25f)
#define EFFECT_HEIGHT		(1.0f)
#define EFFECT_1_FRAME		(5)		// 1�̏�Ԃ̃t���[����
#define EFFECT_LIFE_FRAME	(20)	// �����t���[����

//=============================================================================
// ����
//=============================================================================
void CSmoke::Create(const VECTOR3& pos)
{
	CSmoke* pointer = new CSmoke;

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
void CSmoke::Init(void)
{
	// �e�N���X������
	CBillboard::Init();
}

//=============================================================================
// �I��
//=============================================================================
void CSmoke::Uninit(void)
{
	// �e�N���X�̏I��
	CBillboard::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CSmoke::Update(void)
{
	// �J�E���g�A�b�v
	m_cntAnim++;

	// �e�N�X�`���A�j���[�V����
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
// �`��
//=============================================================================
void CSmoke::Draw(void)
{
	// �e�N���X�̕`��
	CBillboard::Draw();
}