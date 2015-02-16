//=============================================================================
//
// Map2D [ Map2D.h]
//
// create : masato masuda
//
//=============================================================================

//=============================================================================
//include
//=============================================================================
#include "Map2D.h"
#include <math.h>
#include "Texture.h"


//=============================================================================
// constructor
//=============================================================================
CMap2D::CMap2D():CPolygon2D()
{
}

//=============================================================================
// init
//=============================================================================
void CMap2D::Init(void)
{
	CPolygon2D::Init();

	MaterialStencil.ambient = COLOR(0.3f, 0.3f, 0.3f, 1.0f);
	MaterialStencil.diffuse = _Color;
	MaterialStencil.specular = COLOR(0, 0, 0, 1.0f);
	MaterialStencil.emission = COLOR(0, 0, 0, 1.0f);
	MaterialStencil.shininess = 0.0f;

	const float scl = 0.49f;
	VtxStencil[0] = VECTOR3(_Size.x * scl, -_Size.y * scl, 0);
	VtxStencil[1] = VECTOR3(-_Size.x * scl, -_Size.y * scl, 0);
	VtxStencil[2] = VECTOR3(_Size.x * scl, _Size.y * scl, 0);
	VtxStencil[3] = VECTOR3(-_Size.x * scl, _Size.y * scl, 0);

	SetTex2(CTexture::Texture(TEX_MINIMAP_BG));

}
//=============================================================================
// create
//=============================================================================
CMap2D* CMap2D::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CMap2D* Scene = new CMap2D;
	Scene->_Pos = pos;
	Scene->_Rot = rot;
	Scene->_Size = size;
	Scene->_Color = color;
	Scene->Init();

	return Scene;
}

//=============================================================================
// uninit
//=============================================================================
void CMap2D::Uninit(void)
{
	delete this;
}

//=============================================================================
// update
//=============================================================================
void CMap2D::Update(void)
{
	
}

//=============================================================================
// draw
//=============================================================================
void CMap2D::Draw(void)
{
	// ステンシルテストを有効化
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	// ステンシルに書き込むよ
	glColorMask(0, 0, 0, 0);
	glDepthMask(0);

	// 値はステンシルテストで検査せずそのまま書き込み
	glStencilFunc(GL_ALWAYS, 1, ~0);

	// テスト合格でステンシル値を glStencilFunc() の第2引数の値に書き換える(GL_REPLACE)
	// ステンシルバッファがない場合はステンシル値はそのまま（第1引数=GL_KEEP）
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	//===========================================================================
	// ステンシルバッファにのみ書き込まれる
	//===========================================================================
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
	glBindTexture(GL_TEXTURE_2D, texBg.TexID);

	// 半透明無視
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_BLEND);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (float*)&Material.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float*)&Material.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float*)&Material.specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (float*)&Material.emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material.shininess);

#if 1
	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);

	uvStencil.Set();
	for (int cnt = 0; cnt<4; cnt++)
	{
		//glColor4f(_Color.r, _Color.g, _Color.b, _Color.a);
		glTexCoord2f(uvStencil.tex[cnt].x, uvStencil.tex[cnt].y);
		glVertex3f(VtxStencil[cnt].x, VtxStencil[cnt].y, VtxStencil[cnt].z);
	}
	glEnd();
#endif


#if 0
	//glTranslatef(1.0f, 1.0f, 1.0f);
	//glRotatef(_Rot.z, 0, 0, 1.0f);
	glScalef(1.0f, 1.0f, 1.0f);

	const int PART = 30;
	int i, n = PART;
	float x, y, r = 100.0f;
	double rate;

	glBegin(GL_POLYGON);
	for (i = 0; i < n; i++) {
		// 座標を計算
		rate = (double)i / n;
		x = r * cos(2.0f * 3.14f * rate) + _Pos.x;// +_Pos.x;
		y = r * sin(2.0f * 3.14f * rate) + _Pos.y;// + _Pos.y;
		glVertex3f(x, y, 0.0); // 頂点座標を指定
	}
	glEnd();
#endif


	//ビュー行列を戻す
	glPopMatrix();
	//プロジェクションマトリックスを戻す
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	//===========================================================================
	// 通常描画
	//===========================================================================
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);

	// 以降でステンシル値は書き換えない
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// ステンシル値が1の所だけを書き込む
	glStencilFunc(GL_EQUAL, 1, ~0);

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
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	Material.diffuse = _Color;
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
		glTexCoord2f(uv.tex[cnt].x,uv.tex[cnt].y);
		glVertex3f(Vtx[cnt].x,Vtx[cnt].y,Vtx[cnt].z);
	}

	glEnd();

	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D,0);

	//ビュー行列を戻す
	glPopMatrix();
	//プロジェクションマトリックスを戻す
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);//ステンシルテストを無効化

}

// end of file