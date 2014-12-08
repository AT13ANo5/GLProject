//============================================================================
//�t�F�[�h�N���X
//============================================================================
#ifndef _FADE_H_
#define _FADE_H_
//============================================================================
//�C���N���[�h
//============================================================================
#include "main.h"
#include "Texture.h"

//�N���X��`
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX,
	}FADE_STATE;

	//============================================================================
	//�t�F�[�h�J�n
	//�����F
	//	float�F�ω���̃A���t�@�l
	//	int�F���t���[�������ĕω����邩
	//	bool�Ftrue���ƕω����I���܂Ŏ��̃t�F�[�h���ł��Ȃ�
	//============================================================================
	static bool Set(float destAlpha,int frame,bool inifinit = true);

	static void Initialize(void);
	static void Finalize(void);
	void Update(void);
	void Draw(void);

	void SetColor(const COLOR& color){ _Color = color; }
	void SetInfinit(bool infinit){ _Infinit = infinit; }
	void SetTexture(TEX_INFO tex){ Texture = tex; }

	bool Infinit(void){ return _Infinit; }
	FADE_STATE State(void){ return _State; }
	COLOR Color(void){ return _Color; }

	static CFade& Instance(void){ return *Self; }

private:
	CFade(){}
	CFade(const CFade& obj){}
	~CFade(){}
	CFade &operator=(const CFade* obj){}
	
	static CFade* Self;
	float DestAlpha;
	float SubAlpha;
	TEX_INFO Texture;
	
	bool _Infinit;
	FADE_STATE _State;
	COLOR _Color;
	
	VECTOR3 Vtx[4];
	

};


#endif