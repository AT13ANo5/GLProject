#include <math.h>

#include "main.h"
#include "Game.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Polygon3D.h"
#include "Texture.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "Model.h"
#include "CPlayer.h"
#include "PlayerCamera.h"
#include "Report.h"

#include "LoadGauge.h"
#include "LoadString.h"
#include "Life.h"
#include "NumberManager.h"
#include "MiniMap.h"
#include "UI.h"

// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 1500.0f;   // ��̔��a
CPlayer* Player = nullptr;//�v���C���[

// �萔
namespace{
  const float     ICON_SIZE = 50.0f;
  const VECTOR3   REPORT_BG_POS = VECTOR3(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0.0f);
  const COLOR     REPORT_BG_COLOR = COLOR(0.0f, 0.05f, 0.0f, 0.6f);
}


CGame::CGame()
{

	reportBg = nullptr;
	report = nullptr;
	numberManager = nullptr;
	UI = nullptr;
}
CGame::~CGame()
{

}

void CGame::Init(void)
{
	//CPolygon3D* polygon = CPolygon3D::Create(VECTOR3(-200.0f,0,0),VECTOR2(250.0f,250.0f),VECTOR3(0,0,90.0f));
	//polygon->SetTex(CTexture::Texture(TEX_LIGHT));
	//polygon->SetColor(GREEN(1.0f));
	CPolygon3D::Create(VECTOR3(0,-100.0f,0),VECTOR2(500.0f,500.0f),VECTOR3(0.0f,0,0));	// �n�`����

	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(100.0f,100.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// �󐶐�
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));
#ifdef _DEBUG
	// �f�o�b�O���C���[�t���[��
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// �v���C���[����
	Player = CPlayer::Create(CModel::RINCHAN, VECTOR3(0.0f, 30.0f, 0.0f));	Player->SetTex(CTexture::Texture(TEX_YOUJO_BLUE));
	Player->SetRot(0.0f,180.0f,0.0f);

	//�v���C���[�J��������
	CPlayerCamera::Create(Player,300.0f);

	// ���ѕ\�̔w�i
	reportBg = CReport::Create(REPORT_BG_POS,VECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));
	reportBg->SetColor(REPORT_BG_COLOR);

	// ���ѕ\
	const float scl = 0.8f;
	report = CReport::Create(REPORT_BG_POS,VECTOR2(SCREEN_WIDTH * scl,SCREEN_HEIGHT * scl));
	report->SetTex(CTexture::Texture(TEX_REPORT));// ���ѕ\�̐��l
	numberManager = CNumberManager::Create();

	//UI������
	//UI->SetGround(Ground);
	UI->Init();

}
void CGame::Uninit(void)
{
	// �v���C���[�j��
	if (Player != nullptr)
	{
		Player->Release();
		Player = nullptr;
	}

	// ��j��
	if (Sky != nullptr)
	{
		Sky->Release();
		Sky = nullptr;
	}

	// �n�`�j��
	if (Ground != nullptr)
	{
		Ground->Release();
		Ground = nullptr;
	}

	//UI�j��
	UI->Uninit();

	CCamera::ReleaseAll();	CObject::ReleaseAll();
}

void CGame::Update(void)
{
	// �n�`�Ƃ̂����蔻��
	VECTOR3	NormalGround;		// �n�`�̖@��
	float	HeightGround;		// �n�`�̍���
	HeightGround = Ground->GetHeight(Player->Pos(),&NormalGround);

	// ��]�����߂�
	VECTOR3	VectorUpPlayer;		// ������x�N�g��
	VECTOR3	VectorNormalYZ;		// YZ���ʏ�̖@���x�N�g��
	VECTOR3	VectorNormalXY;		// XY���ʏ�̖@���x�N�g��
	float	AnglePlayerX;		// �v���C���[��]X��
	float	AnglePlayerZ;		// �v���C���[��]Z��
	VectorUpPlayer.x = VectorUpPlayer.z = 0.0f;
	VectorUpPlayer.y = 1.0f;
	VectorNormalYZ.x = 0.0f;
	VectorNormalYZ.y = NormalGround.y;
	VectorNormalYZ.z = NormalGround.z;
	VectorNormalYZ.Normalize();
	AnglePlayerX = -acosf(VECTOR3::Dot(VectorNormalYZ,VectorUpPlayer));
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglePlayerZ = -acosf(VECTOR3::Dot(VectorNormalXY,VectorUpPlayer));

	// �v���C���[���̃f�o�b�O�\��
	VECTOR3	positionPlayer = Player->Pos();
	VECTOR3	rotaionPlayer = Player->Rot();
	Console::SetCursorPos(1,1);
	Console::Print("Pos : (%9.3f, %9.3f, %9.3f)",positionPlayer.x,HeightGround,positionPlayer.z);
	Console::Print("Rot : (%9.3f, %9.3f, %9.3f)",180.0f / PI * AnglePlayerX,rotaionPlayer.y,180.0f / PI * AnglePlayerZ);

	// �v���C���[�ɐݒ肷��
	Player->SetPosY(HeightGround);
	Player->SetRotX(AnglePlayerX * 180.0f / PI);
	Player->SetRotZ(AnglePlayerZ * 180.0f / PI);


	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}

	if (CKeyboard::GetPress(DIK_P)){
		reportBg->SetDrawFlag(true);
		report->SetDrawFlag(true);
		numberManager->SetDrawFlag(true);
	}
	else {
		reportBg->SetDrawFlag(false);
		report->SetDrawFlag(false);
		numberManager->SetDrawFlag(false);
	}
}