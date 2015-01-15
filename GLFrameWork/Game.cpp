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
// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 500.0f;   // ��̔��a

// �萔
namespace{
  const float     GAUGE_POS_Y = 70.0f;
  const float     GAUGE_POS_X = 80.0f;
  const VECTOR3   GAUGE_POS = VECTOR3(GAUGE_POS_X, GAUGE_POS_Y, 0.0f);
	const VECTOR2   GAUGE_SIZE = VECTOR2(300.0f,50.0f);
  const float     GAUGE_STR_OFFSET = 50.0f;
  const VECTOR3   GAUGE_STR_POS = VECTOR3(GAUGE_POS_X + GAUGE_STR_OFFSET, GAUGE_POS_Y, 0.0f);
	const VECTOR2   GAUGE_STR_SIZE = VECTOR2(180.0f,50.0f);
  const COLOR     GAUGE_COLOR = COLOR(0.4f, 0.5f, 1.0f, 1.0f);
  const float     ICON_SIZE = 50.0f;
  const VECTOR3   ICON_POS = VECTOR3(20.0f + ICON_SIZE / 2, GAUGE_POS_Y + ICON_SIZE / 2, 0.0f);
  const VECTOR3   REPORT_BG_POS = VECTOR3(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0.0f);
  const COLOR     REPORT_BG_COLOR = COLOR(0.0f, 0.05f, 0.0f, 0.6f);

}


CGame::CGame()
{
	loadGauge = nullptr;
	loadString = nullptr;
  reportBg = nullptr;
  report = nullptr;
}

CGame::~CGame()
{

}

void CGame::Init(void)
{

	//CPolygon3D* polygon = CPolygon3D::Create(VECTOR3(-200.0f,0,0),VECTOR2(250.0f,250.0f),VECTOR3(0,0,90.0f));
	//polygon->SetTex(CTexture::Texture(TEX_LIGHT));
	//polygon->SetColor(GREEN(1.0f));
	CPolygon3D::Create(VECTOR3(0,-100.0f,0),VECTOR2(500.0f,500.0f),VECTOR3(0.0f,0,0));


	// �n�`����
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(50.0f,50.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// �󐶐�
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_MIKU));

	// �v���C���[����
	Player = CPlayer::Create(CModel::MIKU,VECTOR3(0.0f,0.0f,0.0f));
	Player->SetTex(CTexture::Texture(TEX_MIKU));
	Player->SetScl(20.0f,20.0f,20.0f);
	CPlayerCamera::Create(Player,300.0f);
	CLife::Create(VECTOR3(50.0f,40.0f,0.0f),VECTOR2(100.0f,100.0f));

  // ���U�Q�[�W
	CLoadGauge* load_gauge = nullptr;
	load_gauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
	load_gauge->SetTex(CTexture::Texture(TEX_TEAM_LOGO));

	loadGauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
  loadGauge->SetDefaultColor(GAUGE_COLOR);
//	loadGauge->SetTex(CTexture::Texture(TEX_MIKU));

	// ���U������
	loadString = CLoadString::Create(GAUGE_STR_POS,GAUGE_STR_SIZE);
	loadString->SetTex(CTexture::Texture(TEX_CONNECTION));
	loadString->DrawEnable();

  // �e�A�C�R��
  CPolygon2D* canonIcon = nullptr;
  canonIcon = CPolygon2D::Create(ICON_POS, VECTOR2(ICON_SIZE,ICON_SIZE));
  canonIcon->SetTex(CTexture::Texture(TEX_MIKU));

  // ���ѕ\�̔w�i
  reportBg = CReport::Create(REPORT_BG_POS, VECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
  reportBg->SetColor(REPORT_BG_COLOR);

  // ���ѕ\
  const float scl = 0.7f;
  report = CReport::Create(REPORT_BG_POS, VECTOR2(SCREEN_WIDTH * scl, SCREEN_HEIGHT * scl));
  report->SetTex(CTexture::Texture(TEX_REPORT));

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
	CCamera::Release();
	CObject::ReleaseAll();
}

void CGame::Update(void)
{
	// TODO�F�ȉ���s�R�����g�A�E�g�͍�Ƃ��ɂ�������B�I������������܂�
	// �n�`�Ƃ̂����蔻��
	VECTOR3 NormalGround;   // �n�`�̖@��
	//float   HeightGround;   // �n�`�̍���
	//HeightGround = Ground->GetHeight(Player->Pos(), &NormalGround);
	NormalGround.Normalize();

	// ��]�����߂�
	VECTOR3 VectorUpPlayer; // ������x�N�g��
	VECTOR3 VectorNormalYZ; // YZ���ʏ�̖@���x�N�g��
	VECTOR3 VectorNormalXY; // XY���ʏ�̖@���x�N�g��
	float   AnglePlayerX;   // �v���C���[��]X��
	float   AnglePlayerZ;   // �v���C���[��]Z��
	VectorUpPlayer.x = VectorUpPlayer.z = 0.0f;
	VectorUpPlayer.y = 1.0f;
	VectorNormalYZ.x = 0.0f;
	VectorNormalYZ.y = NormalGround.y;
	VectorNormalYZ.z = NormalGround.z;
	VectorNormalYZ.Normalize();
	AnglePlayerX = VECTOR3::Dot(VectorNormalYZ,VectorUpPlayer);
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglePlayerZ = VECTOR3::Dot(VectorNormalXY,VectorUpPlayer);

	// �v���C���[�ɐݒ肷��
	//Player->SetPosY(HeightGround);
	//Player->SetRot(180.0f / PI * AnglePlayerX, 0.0f, 180.0f / PI * AnglePlayerZ);

  // ���U�Q�[�W
  const float currentTimer = (float)Player->ReloadTimer();
  const float maxTimer = (float)PLAYER_RELOAD_TIME;
  const float rate = currentTimer / maxTimer;

  loadGauge->SetRate(rate);
  if (rate >= 1.0f){
    loadString->DrawDisable();
  }
  if (rate <= 0.0f){
    loadString->DrawEnable();
  }

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}

  if (CKeyboard::GetPress(DIK_P)){
    reportBg->SetDrawFlag(true);
    report->SetDrawFlag(true);
  }
  else {
    reportBg->SetDrawFlag(false);
    report->SetDrawFlag(false);
  }

}