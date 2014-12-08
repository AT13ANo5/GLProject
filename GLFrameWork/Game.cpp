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

CGame::CGame()
{

}
CGame::~CGame()
{

}

void CGame::Init(void)
{
	// �n�`����
	Ground = nullptr;
	/*
	Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(50.0f, 50.0f), VECTOR2(20.0f, 20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));
	*/

	// �󐶐�
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_MIKU));

	// �v���C���[����
	Player = CPlayer::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	Player->SetTex(CTexture::Texture(TEX_MIKU));
	Player->SetScl(20.0f, 20.0f, 20.0f);
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

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}
}