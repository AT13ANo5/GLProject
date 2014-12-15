//=============================================================================
//2D�G�t�F�N�g�N���X
//=============================================================================
#ifndef _CPUSH_START_H_
#define _CPUSH_START_H_
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "main.h"
#include "Object.h"

//�N���X��`
class CPushStart :public CObject
{
public:
	CPushStart();
	virtual ~CPushStart(){}
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//=============================================================================
	//����
	//�����F
	//	VECTOR3�F���S���W
	//	VECTOR2�F�傫��
	//	VECTOR3�F��]
	//	COLOR�F���_�J���[
	//
	//XY���ʂŐ��������
	//=============================================================================
	static CPushStart* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot = VECTOR3(0,0,0),const COLOR& color = COLOR(1.0f,1.0f,1.0f,1.0f));

	//=============================================================================
	//�Z�b�^�[
	//=============================================================================
	void SetSize(float x,float y){ _Size.x = x;_Size.y = y; Resize();}
	void SetSize(const VECTOR2& pos){ _Size = pos; Resize(); }
	void SetSizeX(float x){ _Size.x = x; Resize();}
	void SetSizeY(float y){ _Size.y = y; Resize();}

	void SetTex(TEX_INFO tex){ Texture = tex;uv.InverseH = tex.InverseH;uv.InverseV = tex.InverseV; }

	//=============================================================================
	//���Z
	//=============================================================================
	void AddSize(float x,float y){ _Size.x += x;_Size.y += y; Resize(); }
	void AddSize(const VECTOR2& pos){ _Size += pos; Resize(); }
	void AddSizeX(float x){ _Size.x += x; Resize();}
	void AddSizeY(float y){ _Size.y += y; Resize();}

	//=============================================================================
	//�Q�b�^�[
	//=============================================================================
	VECTOR2 Size(void)const{ return _Size; }

private:

	VECTOR2 _Size;
	VECTOR3 Vtx[4];
	UV uv;
	float Length;
	float Angle;

	float moveAlpha;

	void Resize(void);

};

#endif