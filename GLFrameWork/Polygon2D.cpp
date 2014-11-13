//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Polygon2D.h"
#include <math.h>
#include "Texture.h"
//=============================================================================
//�R�X�g���N�^
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
//������
//=============================================================================
void CPolygon2D::Init(void)
{

	Vtx[0] = VECTOR3(_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2,_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2,_Size.y / 2.0f,0);
	

}
//=============================================================================
//�쐬
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
//�I������
//=============================================================================
void CPolygon2D::Uninit(void)
{
	delete this;
}
//=============================================================================
//�X�V
//=============================================================================
void CPolygon2D::Update(void)
{
	
}
//=============================================================================
//�`��
//=============================================================================
void CPolygon2D::Draw(void)
{
	
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//2D�p�}�g���N�X�ݒ�
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();//�v���W�F�N�V�����}�g���b�N�X��ޔ�
	glLoadIdentity();
	glOrtho(0,SCREEN_WIDTH,SCREEN_HEIGHT,0,0,1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//�r���[�}�g���b�N�X��ޔ�
	glLoadIdentity();

	glTranslatef(_Pos.x,_Pos.y,_Pos.z);
	glRotatef(_Rot.z,0,0,1.0f);
	glScalef(1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D,Texture.TexID);
		
	//�|���S���`��
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

	//�r���[�s���߂�
	glPopMatrix();
	//�v���W�F�N�V�����}�g���b�N�X��߂�
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