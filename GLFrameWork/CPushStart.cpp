//=============================================================================
//�C���N���[�h
//=============================================================================
#include "CPushStart.h"
#include <math.h>
#include "Texture.h"
//=============================================================================
//�R�X�g���N�^
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
//������
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
//�쐬
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
//�I������
//=============================================================================
void CPushStart::Uninit(void)
{
	delete this;
}
//=============================================================================
//�X�V
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
//�`��
//=============================================================================
void CPushStart::Draw(void)
{

	//���C�e�B���O����
	glDisable(GL_LIGHTING);

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

void CPushStart::Resize(void)
{
	Vtx[0] = VECTOR3(_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[1] = VECTOR3(-_Size.x / 2,-_Size.y / 2.0f,0);
	Vtx[2] = VECTOR3(_Size.x / 2,_Size.y / 2.0f,0);
	Vtx[3] = VECTOR3(-_Size.x / 2,_Size.y / 2.0f,0);
}