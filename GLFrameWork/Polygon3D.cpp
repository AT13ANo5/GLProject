//=============================================================================
//インクルード
//=============================================================================
#include "Polygon3D.h"
#include <math.h>
#include "Texture.h"
//=============================================================================
//コストラクタ
//=============================================================================
CPolygon3D::CPolygon3D():CObject(3)
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
void CPolygon3D::Init(void)
{
	TEX_INFO tex;
	
	Material.ambient = COLOR(0.3f,0.3f,0.3f,1.0f);
	Material.diffuse = _Color;
	Material.specular = COLOR(0,0,0,1.0f);
	Material.emission = COLOR(0,0,0,1.0f);
	Material.shininess = 0.0f;

	Vtx[0] = VECTOR3(-_Size.x/2,0,_Size.y/2);
	Vtx[1] = VECTOR3(_Size.x/2,0,_Size.y/2);
	Vtx[2] = VECTOR3(-_Size.x/2,0,-_Size.y/2);
	Vtx[3] = VECTOR3(_Size.x/2,0,-_Size.y/2);

	for (int cnt = 0;cnt < 4;cnt++)
	{
		Nor[cnt] = VECTOR3(0,1.0f,0);
	}

	DrawFlag = true;
	LightFlag = true;
}
CPolygon3D* CPolygon3D::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CPolygon3D* Scene = new CPolygon3D;
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
void CPolygon3D::Uninit(void)
{
	delete this;
}
//=============================================================================
//更新
//=============================================================================
void CPolygon3D::Update(void)
{

}
//=============================================================================
//描画
//=============================================================================
void CPolygon3D::Draw(void)
{
	// 描画フラグを見て表示 / 非表示を切り替え
	if (DrawFlag == false)
	{
		return;
	}

	// ライト照射フラグでライトオンオフ
	if(LightFlag == true)
	{
		//ライティング有効
		glEnable(GL_LIGHTING);
	}
	else
	{
		// ライティング無効
		glDisable(GL_LIGHTING);
	}
	
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//ビューマトリックスを退避
	//描画設定
	glTranslatef(_Pos.x, _Pos.y, _Pos.z);
	//********************************************************
	// 姿勢制御用処理追加 ここから
	//********************************************************
	glRotatef(RotationAxis, VectorAxisRotation.x, VectorAxisRotation.y, VectorAxisRotation.z);
	//********************************************************
	// 姿勢制御用処理追加 ここまで
	//********************************************************
	glRotatef(_Rot.z,	0,		0,			1.0f);
	glRotatef(_Rot.y,	0,		1.0f,		0);
	glRotatef(_Rot.x,	1.0f,	0,		0);
	glScalef(1.0f,1.0f,1.0f);
	
	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float*)&Material.ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float*)&Material.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float*)&Material.specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(float*)&Material.emission);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material.shininess);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);
	
	for(int cnt=0;cnt<4;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glNormal3f(Nor[cnt].x,Nor[cnt].y,Nor[cnt].z);
		glTexCoord2f(uv.tex[cnt].x,uv.tex[cnt].y);
		glVertex3f(Vtx[cnt].x,Vtx[cnt].y,Vtx[cnt].z);
	}

	glEnd();

	glPopMatrix();//ビューマトリックスを戻す
	glBindTexture(GL_TEXTURE_2D,0);

	if(LightFlag == false)
	{
		//ライティング有効
		glEnable(GL_LIGHTING);
	}
}

void CPolygon3D::SetUV(float x,float y,float width,float height)
{
	uv = UV(x,y,width,height);
	uv.InverseH = Texture.InverseH;
	uv.InverseV = Texture.InverseV;
	uv.Set();

}

void CPolygon3D::Resize(void)
{
	Vtx[0] = VECTOR3(-_Size.x / 2,0,_Size.y / 2);
	Vtx[1] = VECTOR3(_Size.x / 2,0,_Size.y / 2);
	Vtx[2] = VECTOR3(-_Size.x / 2,0,-_Size.y / 2);
	Vtx[3] = VECTOR3(_Size.x / 2,0,-_Size.y / 2);
}