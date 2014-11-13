//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Effect2D.h"
#include <math.h>
#include "Texture.h"
//=============================================================================
//�R�X�g���N�^
//=============================================================================
CEffect2D::CEffect2D():CObject(LAYER_NUM - 1)
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
void CEffect2D::Init(void)
{
	TEX_INFO tex;
	Length = sqrtf((_Size.x / 2)*(_Size.x / 2) + (_Size.y / 2)*(_Size.y / 2));
	Angle = atan2f(_Size.x / 2,_Size.y / 2);

	Vtx[0] = VECTOR3(_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2,_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2,_Size.y / 2.0f,0);


}
//=============================================================================
//�쐬
//=============================================================================
CEffect2D* CEffect2D::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CEffect2D* Scene = new CEffect2D;
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
void CEffect2D::Uninit(void)
{
	delete this;
}
//=============================================================================
//�X�V
//=============================================================================
void CEffect2D::Update(void)
{

}
//=============================================================================
//�`��
//=============================================================================
void CEffect2D::Draw(void)
{

	//���C�e�B���O����
	glDisable(GL_LIGHTING);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
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

	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);

	//�r���[�s���߂�
	glPopMatrix();
	//�v���W�F�N�V�����}�g���b�N�X��߂�
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
void CEffect2D::SetUV(float x,float y,float width,float height)
{
	uv = UV(x,y,width,height);
	uv.InverseH = Texture.InverseH;
	uv.InverseV = Texture.InverseV;
	uv.Set();

}

void CEffect2D::Resize(void)
{
	Vtx[0] = VECTOR3(_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2,_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2,_Size.y / 2.0f,0);
}