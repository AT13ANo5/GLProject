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

// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 500.0f;   // ��̔��a
#include "CPlayer.h"

CPlayer* g_player;
CModel* g_model;


CGame::CGame()
{

}
CGame::~CGame()
{

}

void CGame::Init(void)
{
<<<<<<< HEAD
	
	CPolygon3D* polygon = CPolygon3D::Create(VECTOR3(-200.0f,0,0),VECTOR2(250.0f,250.0f),VECTOR3(0,0,90.0f));
	polygon->SetTex(CTexture::Texture(TEX_LIGHT));
	polygon->SetColor(GREEN(1.0f));
=======
  // �n�`����
  Ground = nullptr;
  Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(50.0f, 50.0f), VECTOR2(20.0f, 20.0f));
  Ground->SetTex(CTexture::Texture(TEX_FIELD));

  // �󐶐�
  Sky = nullptr;
  Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
  Sky->SetTex(CTexture::Texture(TEX_MIKU));

  // �v���C���[����
  Player = CModel::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
  Player->SetTex(CTexture::Texture(TEX_MIKU));
  Player->SetScl(20.0f, 20.0f, 20.0f);
	
	//CPolygon3D* polygon = CPolygon3D::Create(VECTOR3(-200.0f,0,0),VECTOR2(250.0f,250.0f),VECTOR3(0,0,90.0f));
	//polygon->SetTex(CTexture::Texture(TEX_LIGHT));
	//polygon->SetColor(GREEN(1.0f));
>>>>>>> 24b6ce008e35316dd002e63a3f998df33727a3d6
	CPolygon3D::Create(VECTOR3(0,-100.0f,0),VECTOR2(500.0f,500.0f),VECTOR3(0.0f,0,0));

	// 2014-12-01
	// �@���F�e�X�g
	g_player = CPlayer::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	g_player->SetTex(CTexture::Texture(TEX_MIKU));
	g_player->SetScl(VECTOR3(60.0f, 60.0f, 60.0f));
	//g_player->SetRotY(180.0f);
	
	/*
	g_model = CModel::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	//g_model->SetTex(CTexture::Texture(TEX_MIKU));
	g_model->SetScl(VECTOR3(60.0f, 60.0f, 60.0f));
	g_model->SetRotY(180.0f);
	*/

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

	CObject::ReleaseAll();
}

void CGame::Update(void)
{
  // �v���C���[�ړ�
  if (CKeyboard::GetPress(DIK_W))
  {
    Player->AddPosZ(1.0f);
  }
  else if (CKeyboard::GetPress(DIK_S))
  {
    Player->AddPosZ(-1.0f);
  }
  if (CKeyboard::GetPress(DIK_A))
  {
    Player->AddPosX(1.0f);
  }
  else if (CKeyboard::GetPress(DIK_D))
  {
    Player->AddPosX(-1.0f);
  }

  // �n�`�Ƃ̂����蔻��
  VECTOR3 NormalGround;   // �n�`�̖@��
  float   HeightGround;   // �n�`�̍���
  HeightGround = Ground->GetHeight(Player->Pos(), &NormalGround);
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
  Player->SetPosY(HeightGround);
  Player->SetRot(180.0f / PI * AnglePlayerX, 0.0f, 180.0f / PI * AnglePlayerZ);

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}
}