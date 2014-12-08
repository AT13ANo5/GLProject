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
//自身をリストに追加
//=============================================================================
void CObject::LinkList(void)
{
	if(Top[Priority] != NULL)//二つ目以降の処理
	{
		CObject* pScene = Cur[Priority];
		pScene->Next = this;
		Prev = pScene;
		Next = NULL;
		Cur[Priority] = this;
	}
	else//最初の一つの時の処理
	{
		Top[Priority] = this;
		Cur[Priority] = this;
		Prev = NULL;
		Next = NULL;
	}
}

//=============================================================================
//自身をリストから削除
//=============================================================================
void CObject::UnlinkList(void)
{
	if(Prev == NULL)//先頭
	{
		if(Next != NULL)//次がある
		{
			Next->Prev = NULL;
			Top[Priority] = Next;
		}
		else//最後の一つだった
		{
			Top[Priority] = NULL;
			Cur[Priority] = NULL;
		}
	}
	else if(Next == NULL)//終端
	{
		if(Prev != NULL)//前がある
		{
			Prev->Next = NULL;
			Cur[Priority] = Prev;
		}
		else//最後の一つだった
		{
			Top[Priority] = NULL;
			Cur[Priority] = NULL;
		}
	}
	else//前後にデータがあるとき
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