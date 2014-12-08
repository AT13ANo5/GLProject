//=============================================================================
//インクルード
//=============================================================================
#include "Polygon2D.h"
#include <math.h>
#include "Texture.h"
#include "Life.h"

CLife* Life[3] = {NULL};
//=============================================================================
//コストラクタ
//=============================================================================
CLife::CLife() :CObject(LAYER_NUM - 2)
{
	_Pos.x =
		_Pos.y =
		_Pos.z = 0;

	_Rot.x =
		_Rot.y =
		_Rot.z = 0;

	LifePoint = 3;

}
//=============================================================================
//初期化
//=============================================================================
void CLife::Init(void)
{
	Vtx[0] = VECTOR3(_Size.x / 2, -_Size.y / 2.0f, 0);
	Vtx[1] = VECTOR3(-_Size.x / 2, -_Size.y / 2.0f, 0);
	Vtx[2] = VECTOR3(_Size.x / 2, _Size.y / 2.0f, 0);
	Vtx[3] = VECTOR3(-_Size.x / 2, _Size.y / 2.0f, 0);

	Texture = CTexture::Texture(TEX_LIFE);

}
//=============================================================================
//作成
//=============================================================================
CLife* CLife::Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot, const COLOR& color)
{

	for (int i = 0; i < 3; i++)
	{
		Life[i] = new CLife;
		Life[i]->_Pos = pos;
		Life[i]->_Pos.x += (i*10);
		Life[i]->_Rot = rot;
		Life[i]->_Size = size;
		Life[i]->_Color = color;
		Life[i]->Init();
	}

	return *Life;
}

//=============================================================================
//終了処理
//=============================================================================
void CLife::Uninit(void)
{
	delete this;
}
//=============================================================================
//更新
//=============================================================================
void CLife::Update(void)
{
	switch (LifePoint)
	{
	case 0:
		if (Life[0]->_Color.a != 0)
		{
			Life[0]->_Color.a = 0;
		}
		break;
	//ライフ残り１
	case 1:
		if (Life[1]->_Color.a != 0)
		{
			Life[1]->_Color.a = 0;
		}
		break;
	//ライフ残り２
	case 2:
		if (Life[2]->_Color.a != 0)
		{
			Life[2]->_Color.a = 0;
		}
		break;
	//ライフ残り3
	case 3:
		break;

	}
}
//=============================================================================
//描画
//=============================================================================
void CLife::Draw(void)
{
	//ライティング無効
	glDisable(GL_LIGHTING);
	//2D用マトリクス設定

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();//プロジェクションマトリックスを退避
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//ビューマトリックスを退避
	glLoadIdentity();

	glTranslatef(_Pos.x, _Pos.y, _Pos.z);
	glRotatef(_Rot.z, 0, 0, 1.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, Texture.TexID);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);

	uv.Set();
	for (int cnt = 0; cnt<4; cnt++)
	{
		glColor4f(_Color.r, _Color.g, _Color.b, _Color.a);
		glTexCoord2f(uv.tex[cnt].x, uv.tex[cnt].y);
		glVertex3f(Vtx[cnt].x, Vtx[cnt].y, Vtx[cnt].z);
	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	//ビュー行列を戻す
	glPopMatrix();
	//プロジェクションマトリックスを戻す
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
void CLife::SetUV(float x, float y, float width, float height)
{
	uv = UV(x, y, width, height);
	uv.InverseH = Texture.InverseH;
	uv.InverseV = Texture.InverseV;
	uv.Set();

}

void CLife::Resize(void)
{
	Vtx[0] = VECTOR3(_Size.x / 2, -_Size.y / 2.0f, 0);
	Vtx[1] = VECTOR3(-_Size.x / 2, -_Size.y / 2.0f, 0);
	Vtx[2] = VECTOR3(_Size.x / 2, _Size.y / 2.0f, 0);
	Vtx[3] = VECTOR3(-_Size.x / 2, _Size.y / 2.0f, 0);
}