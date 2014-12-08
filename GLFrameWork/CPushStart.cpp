//=============================================================================
//インクルード
//=============================================================================
#include "CPushStart.h"
#include <math.h>
#include "Texture.h"
//=============================================================================
//コストラクタ
//=============================================================================
CPushStart::CPushStart():CObject(LAYER_NUM - 2)
{
	_Pos.x =
	_Pos.y =
	_Pos.z = 0;

	_Rot.x =
	_Rot.y =
	_Rot.z = 0;

	moveAlpha = -0.05f;

}
//=============================================================================
//初期化
//=============================================================================
void CPushStart::Init(void)
{
	TEX_INFO tex;
	Length = sqrtf((_Size.x / 2)*(_Size.x / 2) + (_Size.y / 2)*(_Size.y / 2));
	Angle = atan2f(_Size.x / 2,_Size.y / 2);

	Vtx[0] = VECTOR3(_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2,_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2,_Size.y / 2.0f,0);

	uv.tex[0] = VECTOR2(1.0f, 0.0f);
	uv.tex[1] = VECTOR2(0.0f, 0.0f);
	uv.tex[2] = VECTOR2(1.0f, 1.0f);
	uv.tex[3] = VECTOR2(0.0f, 1.0f);
}
//=============================================================================
//作成
//=============================================================================
CPushStart* CPushStart::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CPushStart* Scene = new CPushStart;
	Scene->_Pos = pos;
	Scene->_Rot = rot;
	Scene->_Size = size;
	Scene->_Color = color;
	Scene->Init();

	return Scene;
}

//=============================================================================
//終了処理
//=============================================================================
void CPushStart::Uninit(void)
{
	delete this;
}
//=============================================================================
//更新
//=============================================================================
void CPushStart::Update(void)
{
	SetAlpha(Color().a + moveAlpha);

	if (Color().a < 0.0f)
	{
		SetAlpha(0.0f);
		moveAlpha *= -1;
	}

	if (Color().a > 1.0f)
	{
		SetAlpha(1.0f);
		moveAlpha *= -1;
	}
}
//=============================================================================
//描画
//=============================================================================
void CPushStart::Draw(void)
{

	//ライティング無効
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();//プロジェクションマトリックスを退避
	glLoadIdentity();
	glOrtho(0,SCREEN_WIDTH,SCREEN_HEIGHT,0,0,1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//ビューマトリックスを退避
	glLoadIdentity();

	glTranslatef(_Pos.x,_Pos.y,_Pos.z);
	glRotatef(_Rot.z,0,0,1.0f);
	glScalef(1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);

	uv.Set();
	for (int cnt = 0;cnt<4;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glTexCoord2f(uv.tex[cnt].x,uv.tex[cnt].y);
		glVertex3f(Vtx[cnt].x,Vtx[cnt].y,Vtx[cnt].z);
	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D,0);

	//ビュー行列を戻す
	glPopMatrix();
	//プロジェクションマトリックスを戻す
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void CPushStart::Resize(void)
{
	Vtx[0] = VECTOR3(_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2,_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2,_Size.y / 2.0f,0);
}