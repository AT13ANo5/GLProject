//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Polygon2D.h"
#include <math.h>
#include "Texture.h"
#include "Life.h"

CLife* Life[3] = {NULL};
//=============================================================================
//�R�X�g���N�^
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
//������
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
//�쐬
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
//�I������
//=============================================================================
void CLife::Uninit(void)
{
	delete this;
}
//=============================================================================
//�X�V
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
	//���C�t�c��P
	case 1:
		if (Life[1]->_Color.a != 0)
		{
			Life[1]->_Color.a = 0;
		}
		break;
	//���C�t�c��Q
	case 2:
		if (Life[2]->_Color.a != 0)
		{
			Life[2]->_Color.a = 0;
		}
		break;
	//���C�t�c��3
	case 3:
		break;

	}
}
//=============================================================================
//�`��
//=============================================================================
void CLife::Draw(void)
{
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//2D�p�}�g���N�X�ݒ�

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();//�v���W�F�N�V�����}�g���b�N�X��ޔ�
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//�r���[�}�g���b�N�X��ޔ�
	glLoadIdentity();

	glTranslatef(_Pos.x, _Pos.y, _Pos.z);
	glRotatef(_Rot.z, 0, 0, 1.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, Texture.TexID);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//�|���S���`��
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

	//�r���[�s���߂�
	glPopMatrix();
	//�v���W�F�N�V�����}�g���b�N�X��߂�
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