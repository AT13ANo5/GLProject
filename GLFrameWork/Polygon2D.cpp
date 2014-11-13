//=============================================================================
//インクルード
//=============================================================================
#include "Polygon2D.h"
#include <math.h>
#include "Texture.h"
//=============================================================================
//コストラクタ
//=============================================================================
CPolygon2D::CPolygon2D():CObject(LAYER_NUM-2)
{
	_Pos.x = 
	_Pos.y = 
	_Pos.z = 0;

	_Rot.x = 
	_Rot.y = 
	_Rot.z = 0;
	
}
//=============================================================================
//初期化
//=============================================================================
void CPolygon2D::Init(void)
{

	Vtx[0] = VECTOR3(_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2,_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2,_Size.y / 2.0f,0);
	

}
//=============================================================================
//作成
//=============================================================================
CPolygon2D* CPolygon2D::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CPolygon2D* Scene = new CPolygon2D;
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
void CPolygon2D::Uninit(void)
{
	delete this;
}
//=============================================================================
//更新
//=============================================================================
void CPolygon2D::Update(void)
{
	
}
//=============================================================================
//描画
//=============================================================================
void CPolygon2D::Draw(void)
{
	
	//ライティング無効
	glDisable(GL_LIGHTING);
	//2D用マトリクス設定
	
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
void CPolygon2D::SetUV(float x,float y,float width,float height)
{
	uv = UV(x,y,width,height);
	uv.InverseH = Texture.InverseH;
	uv.InverseV = Texture.InverseV;
	uv.Set();

}

void CPolygon2D::Resize(void)
{
	Vtx[0] = VECTOR3(_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2,_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2,_Size.y / 2.0f,0);
}