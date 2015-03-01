//=============================================================================
//2D���C�t�N���X
//	Kikushima
//	Masato Masuda
//=============================================================================
#pragma once
#ifndef _TIME_H_
#define _TIME_H_
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "main.h"
#include "Polygon2D.h"
#include "Number2D.h"

//=============================================================================
//�萔
//=============================================================================
static const int MAX_DIGIT = 3;


//=============================================================================
//�N���X��`
//=============================================================================
//class CNumber2D;

class CTime :public CPolygon2D
{
public:
	CTime();
	~CTime(){}
	void Init(void);
	void Uninit(void);
	void Update(void);

	//=============================================================================
	//�쐬
	//�����F
	//	VECTOR3�F���S���W
	//	VECTOR2�F�傫��
	//	VECTOR3�F��]
	//	COLOR�F	���_�J���[
	//=============================================================================
	static CTime* Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot = VECTOR3(0, 0, 0), const COLOR& color = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//=============================================================================
	// color�̐ݒ�
	void SetColor(const COLOR col);

	// �^�C���擾
	int		GetTime(void){ return Timer; }
	void	SetTime(int time)
	{
		Timer = time; TimeCount = 0;
		Nomber[0]->SetNumber(Timer / 100);

		Nomber[1]->SetNumber(Timer / 10);

		Nomber[2]->SetNumber(Timer);
	}
	void	SubTime()
	{
		if (updateFlag == true)
		{
			Timer--;
			TimeCount = 0;
			Nomber[0]->SetNumber(Timer / 100);

			Nomber[1]->SetNumber(Timer / 10);

			Nomber[2]->SetNumber(Timer);
		}
	}

	// 
	void SetUpdateFlag(bool flag){ updateFlag = flag; }
	bool GetUpdateFlag(void){ return updateFlag; }

private:

	CNumber2D* Nomber[MAX_DIGIT];
	int TimeCount;
	int Timer;
	VECTOR3 Pos;
	VECTOR2 Size;
	bool updateFlag;
 bool cntFlag;
};

#endif

// EOF