//=============================================================================
//インクルード
//=============================================================================
#include "Billboard.h"
#include <math.h>
#include "Texture.h"
//=============================================================================
//コストラクタ
//=============================================================================
CBillboard::CBillboard() :CObject(3)
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
void CBillboard::Init(void)
{

	Material.ambient = COLOR(0.3f,0.3f,0.3f,1.0f);
	Material.diffuse = _Color;
	Material.specular = COLOR(0,0,0,1.0f);
	Material.emission = COLOR(0,0,0,1.0f);
	Material.shininess = 0.0f;

	Vtx[0] = VECTOR3(_Size.x / 2.0f,_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2.0f,_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2.0f,-_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2.0f,-_Size.y / 2.0f,0);

}
CBillboard* CBillboard::Create(VECTOR3 pos,VECTOR3 size,VECTOR3 rot,COLOR color)
{
	CBillboard* Scene = new CBillboard;
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
void CBillboard::Uninit(void)
{
	delete this;
}
//=============================================================================
//更新
//=============================================================================
void CBillboard::Update(void)
{
	
}
//=============================================================================
//描画
//=============================================================================
void CBillboard::Draw(void)
{
	//ライティング無効
	glDisable(GL_LIGHTING);


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//ビューマトリックスを退避

	MATRIX4x4 matrix,invMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX,(float*)&matrix);
	
	//描画設定
	glTranslatef(_Pos.x,_Pos.y,_Pos.z);
	glMatrixInverse(&invMatrix,matrix);
	glMultMatrixf((float*)&invMatrix);
	
	glRotatef(_Rot.z,0,0,1.0f);
	glScalef(1.0f,1.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float*)&Material.ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float*)&Material.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float*)&Material.specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(float*)&Material.emission);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material.shininess);

	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);

	uv.Set();
	for (int cnt = 0;cnt<4;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glNormal3f(Nor[cnt].x,Nor[cnt].y,Nor[cnt].z);
		glTexCoord2f(uv.tex[cnt].x,uv.tex[cnt].y);
		glVertex3f(Vtx[cnt].x,Vtx[cnt].y,Vtx[cnt].z);
	}

	glEnd();

	glPopMatrix();//ビューマトリックスを戻す
	glBindTexture(GL_TEXTURE_2D,0);


}

void CBillboard::SetUV(float x,float y,float width,float height)
{
	uv = UV(x,y,width,height);
	uv.InverseH = Texture.InverseH;
	uv.InverseV = Texture.InverseV;
	uv.Set();

}