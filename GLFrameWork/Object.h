//=================================================================================
//�I�u�W�F�N�g�N���X
//
//�啔���̃N���X�̌��ɂȂ�N���X
//
//�Q�b�^�[�����݂��郁���o�ϐ��́A�ϐ����̑O�ɃA���_�[�o�[���t�����Ă���
//�Q�b�^�[��Get�Z�Z�ł͖����A�����o�ϐ����̃A���_�[�o�[���Ƃ��āA����()������
//=================================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
//=================================================================================
//�C���N���[�h
//=================================================================================
#include "main.h"
#include "Texture.h"

//=================================================================================
//�}�N����`
//=================================================================================
#define LAYER_NUM (8)

//�N���X��`
class CObject
{
public:
	CObject(int Prioriy=3);
	virtual ~CObject();
	virtual void Init(void){};
	virtual void Uninit(void){};
	virtual void Update(void)=0;
	virtual void Draw(void)=0;
	void Release(void);

	static void Clear(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);

	//=================================================================================
	//�Z�b�^�[
	//=================================================================================
	//Position
	virtual void SetPos(float x,float y,float z){ _Pos.x = x;_Pos.y = y;_Pos.z = z; }
	virtual void SetPos(const VECTOR3& pos){ _Pos = pos; }
	virtual void SetPosX(float x){ _Pos.x = x; }
	virtual void SetPosY(float y){ _Pos.y = y; }
	virtual void SetPosZ(float z){ _Pos.z = z; }

	//Rotation
	virtual void SetRot(float x,float y,float z){ _Rot.x = x;_Rot.y = y;_Rot.z = z; }
	virtual void SetRot(const VECTOR3& rot){ _Rot = rot; }
	virtual void SetRotX(float x){ _Rot.x = x; }
	virtual void SetRotY(float y){ _Rot.y = y; }
	virtual void SetRotZ(float z){ _Rot.z = z; }

	//Scale
	virtual void SetScl(float x,float y,float z){ _Scl.x = x;_Scl.y = y;_Scl.z = z; }
	virtual void SetScl(const VECTOR3& Scl){ _Scl = Scl; }
	virtual void SetSclX(float x){ _Scl.x = x; }
	virtual void SetSclY(float y){ _Scl.y = y; }
	virtual void SetSclZ(float z){ _Scl.z = z; }

	//Color
	virtual void SetColor(const COLOR& color){ _Color = color; }
	virtual void SetRed(float r){_Color.r = r;}
	virtual void SetBlue(float b){_Color.b = b;}
	virtual void SetGreen(float g){_Color.g = g;}
	virtual void SetAlpha(float a){_Color.a = a;}

	//�e�N�X�`��
	virtual void SetTex(TEX_INFO tex){Texture = tex;}

	//�N�H�[�^�j�I��
	void SetAxisRotation(const VECTOR3& axis)		{ VectorAxisRotation = axis; }		// ��]��
	void SetRotationAxis(float rotation)			{ RotationAxis = rotation; }		// ��]��

	//=================================================================================
	//���Z
	//=================================================================================
	//Position
	virtual void AddPos(float x,float y,float z){ _Pos.x += x;_Pos.y += y;_Pos.z += z; }
	virtual void AddPos(const VECTOR3& pos){ _Pos += pos; }
	virtual void AddPosX(float x){ _Pos.x += x; }
	virtual void AddPosY(float y){ _Pos.y += y; }
	virtual void AddPosZ(float z){ _Pos.z += z; }

	//Rotation
	virtual void AddRot(float x,float y,float z){ _Rot.x += x;_Rot.y += y;_Rot.z += z; }
	virtual void AddRot(const VECTOR3& rot){ _Rot += rot; }
	virtual void AddRotX(float x){ _Rot.x += x; }
	virtual void AddRotY(float y){ _Rot.y += y; }
	virtual void AddRotZ(float z){ _Rot.z += z; }

	//Scale
	virtual void AddScl(float x,float y,float z){ _Scl.x += x;_Scl.y += y;_Scl.z += z; }
	virtual void AddScl(const VECTOR3& Scl){ _Scl = Scl; }
	virtual void AddSclX(float x){ _Scl.x += x; }
	virtual void AddSclY(float y){ _Scl.y += y; }
	virtual void AddSclZ(float z){ _Scl.z += z; }

	//Color
	virtual void AddColor(const COLOR& color){ _Color += color; }
	virtual void AddRed(float r){ _Color.r += r; }
	virtual void AddBlue(float b){ _Color.b += b; }
	virtual void AddGreen(float g){ _Color.g += g; }
	virtual void AddAlpha(float a){ _Color.a += a; }

	//=================================================================================
	//�Q�b�^�[
	//=================================================================================
	virtual VECTOR3 Pos(void)const{ return _Pos; }
	virtual VECTOR3 Rot(void)const{ return _Rot; }
	virtual VECTOR3 Scl(void)const{ return _Scl; }
	virtual COLOR Color(void)const{return _Color;}
	//=================================================================================

protected:

	int Priority;
	VECTOR3 _Pos;
	VECTOR3 _Rot;
	VECTOR3 _Scl;
	COLOR _Color;
	TEX_INFO Texture;

	VECTOR3	VectorAxisRotation;		// �N�H�[�^�j�I����]��
	float	RotationAxis;			// ��]�p�x

private:
	void LinkList();
	void UnlinkList();

	static CObject* Top[LAYER_NUM];
	static CObject* Cur[LAYER_NUM];

	CObject* Next;
	CObject* Prev;

	bool DeathFlag;

};

#endif