#include "Object.h"
CObject* CObject::Top[] = {NULL};
CObject* CObject::Cur[] = {NULL};

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
	if(Top[Priority] != NULL)//��ڈȍ~�̏���
	{
		CObject* pScene = Cur[Priority];
		pScene->Next = this;
		Prev = pScene;
		Next = NULL;
		Cur[Priority] = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top[Priority] = this;
		Cur[Priority] = this;
		Prev = NULL;
		Next = NULL;
	}
}

//=============================================================================
//���g�����X�g����폜
//=============================================================================
void CObject::UnlinkList(void)
{
	if(Prev == NULL)//�擪
	{
		if(Next != NULL)//��������
		{
			Next->Prev = NULL;
			Top[Priority] = Next;
		}
		else//�Ō�̈������
		{
			Top[Priority] = NULL;
			Cur[Priority] = NULL;
		}
	}
	else if(Next == NULL)//�I�[
	{
		if(Prev != NULL)//�O������
		{
			Prev->Next = NULL;
			Cur[Priority] = Prev;
		}
		else//�Ō�̈������
		{
			Top[Priority] = NULL;
			Cur[Priority] = NULL;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;
}
void CObject::Clear(void)
{
	for(int cnt=0;cnt<LAYER_NUM;cnt++)
	{
		Top[cnt] = NULL;
		Cur[cnt] = NULL;
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
		CObject* Next = NULL;
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
		CObject* Next = NULL;
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
		CObject* Next = NULL;
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