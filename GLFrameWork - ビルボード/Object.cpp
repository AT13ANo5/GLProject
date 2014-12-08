#include "Object.h"
CObject* CObject::Top[] = {nullptr};
CObject* CObject::Cur[] = {nullptr};

CObject::CObject(int priority)
{
	Priority = priority;
	DeathFlag = false;
	LinkList();
}
CObject::~CObject()
{
	UnlinkList();
}
//=============================================================================
//���g�����X�g�ɒǉ�
//=============================================================================
void CObject::LinkList(void)
{
	if(Top[Priority] != nullptr)//��ڈȍ~�̏���
	{
		CObject* pScene = Cur[Priority];
		pScene->Next = this;
		Prev = pScene;
		Next = nullptr;
		Cur[Priority] = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top[Priority] = this;
		Cur[Priority] = this;
		Prev = nullptr;
		Next = nullptr;
	}
}

//=============================================================================
//���g�����X�g����폜
//=============================================================================
void CObject::UnlinkList(void)
{
	if(Prev == nullptr)//�擪
	{
		if(Next != nullptr)//��������
		{
			Next->Prev = nullptr;
			Top[Priority] = Next;
		}
		else//�Ō�̈������
		{
			Top[Priority] = nullptr;
			Cur[Priority] = nullptr;
		}
	}
	else if(Next == nullptr)//�I�[
	{
		if(Prev != nullptr)//�O������
		{
			Prev->Next = nullptr;
			Cur[Priority] = Prev;
		}
		else//�Ō�̈������
		{
			Top[Priority] = nullptr;
			Cur[Priority] = nullptr;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = nullptr;
	Next = nullptr;
}
void CObject::Clear(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		Top[cnt] = nullptr;
		Cur[cnt] = nullptr;
	}
}

void CObject::Release(void)
{
	DeathFlag = true;
	
}

void CObject::UpdateAll(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		CObject* Scene = Top[cnt];
		CObject* Next = nullptr;
		while(Scene)
		{
			Next = Scene->Next;
			Scene->Update();
			if (Scene->DeathFlag)
			{
				Scene->Uninit();
			}
			Scene = Next;
		}
	}
}

void CObject::DrawAll(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		CObject* Scene = Top[cnt];
		CObject* Next = nullptr;
		while(Scene)
		{
			Next = Scene->Next;
			Scene->Draw();
			Scene = Next;
		}
	}
}

void CObject::ReleaseAll(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		CObject* Scene = Top[cnt];
		CObject* Next = nullptr;
		while(Scene)
		{
			Next = Scene->Next;
			Scene->Release();
			if(Scene->DeathFlag)
			{
				Scene->Uninit();
			}
			Scene = Next;
		}
	}
}