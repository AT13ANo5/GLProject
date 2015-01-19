//============================================================================
//ミニマップクラス
//============================================================================

//============================================================================
//インクルード
//============================================================================
#include "MiniMap.h"
#include "Polygon2D.h"
#include "MeshGround.h"

//============================================================================
//スタティック変数
//============================================================================
VECTOR3 CMiniMap::FieldSize = VECTOR3(0,0,0);
int CMiniMap::_SelfId = 0;
//============================================================================
//コンスト変数
//============================================================================
const COLOR ArrowColor[PLAYER_MAX] =
{
	BLUE(1.0f),
	GREEN(1.0f),
	COLOR(1.0f,0.7f,0,1.0f),
	RED(1.0f),
	WHITE(1.0f),
	YELLOW(1.0f),
};

//============================================================================
//コンストラクタ
//============================================================================
CMiniMap::CMiniMap()
{
	for (int cnt = 0;cnt < PLAYER_MAX;cnt++)
	{
		Player[cnt] = nullptr;
		PlayerPos[cnt] = VECTOR3(0,0,0);
		PlayerRot[cnt] = VECTOR3(0,0,0);
	}
	Map = nullptr;
}
//============================================================================
//初期化
//============================================================================
void CMiniMap::Init(void)
{
	MiniMapSize = VECTOR2(256.0f,256.0f);
	Map = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH - MiniMapSize.x / 2.0f-5.0f,MiniMapSize.y / 2.0f+5.0f,0),MiniMapSize);
	Map->SetTex(CTexture::Texture(TEX_MINIMAP));
	for (int cnt = 0;cnt < PLAYER_MAX;cnt++)
	{
		PlayerPos[cnt] = Map->Pos();
		Player[cnt] = CPolygon2D::Create(Map->Pos(),VECTOR2(24.0f,24.0f),VECTOR3(0,0,0),ArrowColor[cnt]);
		Player[cnt]->SetTex(CTexture::Texture(TEX_MINIMAP_ARROW));
	}

}
//============================================================================
//更新
//============================================================================
void CMiniMap::Update(void)
{
	for (int cnt = 0;cnt < PLAYER_MAX;cnt++)
	{
		Player[cnt]->SetPos(PlayerPos[cnt]);
		Player[cnt]->SetRot(PlayerRot[cnt]);
	}
	
}

//============================================================================
//プレイヤーの情報をセット
//引数：
//	int	id … id番目のプレイヤー
//	VECTOR3 pos … プレイヤーの位置
//	float rotY … プレイヤーの向き
//============================================================================
void CMiniMap::SetPlayer(int id,const VECTOR3& pos,float rotY)
{
	if (id >= 0 && id < PLAYER_MAX)
	{
		VECTOR2 per = VECTOR2(pos.x / (FieldSize.x),pos.z/(FieldSize.z));
		PlayerPos[id].x = MiniMapSize.x*per.x + Map->Pos().x;
		PlayerPos[id].y = MiniMapSize.y*per.y + Map->Pos().y;
		PlayerRot[id].z = -rotY+180.0f;
	}
}