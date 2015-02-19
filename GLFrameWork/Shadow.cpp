//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Shadow.h"
#include <math.h>
#include "Texture.h"
//=============================================================================
//�R�X�g���N�^
//=============================================================================
CShadow::CShadow():CPolygon3D()
{
}

CShadow* CShadow::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CShadow* Scene = new CShadow;
	Scene->_Pos = pos;
	Scene->_Rot = rot;
	Scene->_Size = size;
	Scene->_Color = color;
	Scene->Init();

	return Scene;
}

//=============================================================================
//�`��
//=============================================================================
void CShadow::Draw(void)
{
	//���C�e�B���O����
	glEnable(GL_LIGHTING);
	
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//�r���[�}�g���b�N�X��ޔ�
	//�`��ݒ�
	glTranslatef(_Pos.x,_Pos.y,_Pos.z);
	glScalef(_Scl.x, _Scl.y, _Scl.z);
	glRotatef(RotationAxis, VectorAxisRotation.x, VectorAxisRotation.y, VectorAxisRotation.z);
//	glRotatef(_Rot.z,	0,		0,			1.0f);
//	glRotatef(_Rot.y,	0,		1.0f,		0);
//	glRotatef(_Rot.x,	1.0f,	0,		0);
	
	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float*)&Material.ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float*)&Material.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float*)&Material.specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(float*)&Material.emission);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material.shininess);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//�|���S���`��
	glBegin(GL_TRIANGLE_STRIP);
	
	for(int cnt=0;cnt<4;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glNormal3f(Nor[cnt].x,Nor[cnt].y,Nor[cnt].z);
		glTexCoord2f(uv.tex[cnt].x,uv.tex[cnt].y);
		glVertex3f(Vtx[cnt].x,Vtx[cnt].y,Vtx[cnt].z);
	}

	glEnd();

	glPopMatrix();//�r���[�}�g���b�N�X��߂�
	glBindTexture(GL_TEXTURE_2D,0);

}