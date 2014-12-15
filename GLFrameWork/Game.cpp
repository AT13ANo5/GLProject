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

#include "LoadGauge.h"
#include "LoadString.h"#include "Life.h"
// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 500.0f;   // ��̔��a

// �萔
namespace{
  const VECTOR3   GAUGE_POS      = VECTOR3(20.0f, 20.0f, 0.0f);
  const VECTOR2   GAUGE_SIZE     = VECTOR2(400.0f, 80.0f);
  const VECTOR3   GAUGE_STR_POS  = VECTOR3(80.0f, 20.0f, 0.0f);
  const VECTOR2   GAUGE_STR_SIZE = VECTOR2(150.0f, 60.0f);
}


CGame::CGame()
{
  loadGauge = nullptr;
  loadString = nullptr;
  testTimer = 0;
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
	Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(50.0f, 50.0f), VECTOR2(20.0f, 20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// �󐶐�
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_MIKU));

	// �v���C���[����
	Player = CPlayer::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	Player->SetTex(CTexture::Texture(TEX_MIKU));
	Player->SetScl(20.0f, 20.0f, 20.0f);
	CPlayerCamera::Create(Player,300.0f);

CLife::Create(VECTOR3(50.0f, 40.0f, 0.0f), VECTOR2(100.0f, 100.0f));}// ���U�Q�[�W
  CLoadGauge* load_gauge = nullptr;
  load_gauge = CLoadGauge::Create(GAUGE_POS, GAUGE_SIZE);
  load_gauge->SetTex(CTexture::Texture(TEX_TEAM_LOGO));

  loadGauge = CLoadGauge::Create(GAUGE_POS, GAUGE_SIZE);
  loadGauge->SetTex(CTexture::Texture(TEX_MIKU));

  // ���U������
  loadString = CLoadString::Create(GAUGE_STR_POS, GAUGE_STR_SIZE);
  loadString->SetTex(CTexture::Texture(TEX_CONNECTION));
  loadString->DrawEnable();
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
	AnglePlayerX = VECTOR3::Dot(VectorNormalYZ, VectorUpPlayer);
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglePlayerZ = VECTOR3::Dot(VectorNormalXY, VectorUpPlayer);

	// �v���C���[�ɐݒ肷��
	//Player->SetPosY(HeightGround);
	//Player->SetRot(180.0f / PI * AnglePlayerX, 0.0f, 180.0f / PI * AnglePlayerZ);

  const int max_time = 200;
  testTimer++;
  if (testTimer >= max_time + 50){
    testTimer =  0;
  }
  float rate = (float)testTimer / (float)max_time;
  if (rate > 1.0f){
    rate = 1.0f;
  }
  loadGauge->SetRate(rate);


	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}
}