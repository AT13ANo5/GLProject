#define _CRT_SECURE_NO_WARNINGS
#include "SoundAL.h"
#include<stdio.h>

CSoundAL* CSoundAL::Top = NULL;
CSoundAL* CSoundAL::Cur = NULL;
float CSoundAL::_DefaultMaxDistance = 1500.0f;
float CSoundAL::_MasterVolume = 1.0f;
int CSoundAL::_Num = 0;
VECTOR3 CSoundAL::_ListenerPos = VECTOR3(0,0,0);
VECTOR3 CSoundAL::_ListenerSpeed = VECTOR3(0,0,0);

VECTOR3 CSoundAL::_ListenerFrontVec = VECTOR3(0,0,0);
VECTOR3 CSoundAL::_ListenerUpVec = VECTOR3(0,0,0);
ALfloat CSoundAL::_ListenerOri[6] = { 0 };

typedef struct
{
	char* file;
	bool loop;
}PARAM;

const PARAM File[CSoundAL::SOUND_MAX] =
{
	{ "data/sound/CosmicMind.wav",true },
	{ "data/sound/Alice_in_Wonderland.wav",true },
	{ "data/sound/ChineseTea.wav",true },
	{ "data/sound/Greenwich_of_Sky.wav",true },
	{ "data/sound/Lullaby.wav",true },
	{ "data/sound/OldWorld.wav",true },
	{ "data/sound/Port_of_Spring.wav",true },
	{ "data/sound/ENCOUNTER.wav",true },
	{ "data/sound/LockOn.wav",false }
};

SOUND_BUFF CSoundAL::Buffer[SOUND_MAX];

CSoundAL::CSoundAL()
{
	_Num++;
	LinkList();
}
CSoundAL::~CSoundAL()
{
	_Num--;
	UnlinkList();
}

//=============================================================================
//自身をリストに追加
//=============================================================================
void CSoundAL::LinkList(void)
{
	if (Top != NULL)//二つ目以降の処理
	{
		CSoundAL* sound = Cur;
		sound->Next = this;
		Prev = sound;
		Next = NULL;
		Cur = this;
	}
	else//最初の一つの時の処理
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
}
//=============================================================================
//自身をリストから削除
//=============================================================================
void CSoundAL::UnlinkList(void)
{
	if (Prev == NULL)//先頭
	{
		if (Next != NULL)//次がある
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if (Next == NULL)//終端
	{
		if (Prev != NULL)//前がある
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//最後の一つだった
		{
			Top = NULL;
			Cur = NULL;
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
void CSoundAL::Initialize(void)
{
	int argc = 0;
	char* argv[] = { NULL };
	ALboolean  succece = false;
	succece = alutInit(&argc,argv);
	alDistanceModel(AL_LINEAR_DISTANCE);

	for(int cnt = 0;cnt < SOUND_MAX;cnt++)
	{
		
		alGenBuffers(1,&Buffer[cnt].Buffer);
		Buffer[cnt].Buffer = alutCreateBufferFromFile(File[cnt].file);
		if (Buffer[cnt].Buffer == 0)
		{
			Buffer[cnt].Buffer = alutCreateBufferFromFile(File[cnt].file);
		}
		Buffer[cnt].loop = File[cnt].loop;
		alGenSources(MULTI_PLAY_NUM,Buffer[cnt].Source);

		for (int num = 0;num < MULTI_PLAY_NUM;num++)
		{
			
			Buffer[cnt].Using[num] = false;
			alSourcei(Buffer[cnt].Source[num],AL_BUFFER,Buffer[cnt].Buffer);
			alSourcei(Buffer[cnt].Source[num],AL_LOOPING,Buffer[cnt].loop);
		}

	}
	ALfloat ori[] = { 0,0,1.0f,0,1.0f,0 };
	
	memcpy(_ListenerOri,ori,sizeof(ori));
	_ListenerFrontVec.z = 1.0f;
	_ListenerUpVec.y = 1.0f;

	alListenerfv(AL_ORIENTATION,ori);
	alListener3f(AL_VELOCITY,0,0,0);
	alListenerf(AL_ROLLOFF_FACTOR,1.0f);

	_ListenerPos = VECTOR3(0,0,0);
	_ListenerSpeed = VECTOR3(0,0,0);

}
void CSoundAL::Finalize(void)
{
	for (int cnt = 0;cnt < SOUND_MAX;cnt++)
	{
		alDeleteSources(MULTI_PLAY_NUM,Buffer[cnt].Source);
		alDeleteBuffers(MULTI_PLAY_NUM,&Buffer[cnt].Buffer);
	}
	ReleaseAll();
	alutExit();
}

CSoundAL* CSoundAL::Play(SOUND id,bool autoRelease)
{
	CSoundAL* sound = new CSoundAL;
	if (sound == NULL){return NULL;}
	sound->Type = id;
	sound->CheckPlaying();
	if (sound->id < 0)
	{
		sound->Release();
		return NULL;
	}
	sound->AutoRelease = autoRelease;
	sound->Init();

	return sound;
}
void CSoundAL::Init(void)
{
	_MaxDistance = _DefaultMaxDistance;
	_Volume = _MasterVolume;

	alSourcef(Buffer[Type].Source[id],AL_GAIN,_Volume);
	alSourcef(Buffer[Type].Source[id],AL_MIN_GAIN,0.0f);
	alSourcef(Buffer[Type].Source[id],AL_MAX_DISTANCE,_MaxDistance);
	
	alSourcePlay(Buffer[Type].Source[id]);
	alutSleep(1);
}
void CSoundAL::Uninit(void)
{
	
}

void CSoundAL::Stop(void)
{
	int state = 0;
	alGetSourcei(Buffer[Type].Source[id],AL_SOURCE_STATE,&state);
	if (state==AL_PLAYING)
	{
		alSourceStop(Buffer[Type].Source[id]);
	}
}
void CSoundAL::Update(void)
{

	switch (_Fade)
	{
	case FADE_IN:
		
		_Volume += SubVolume;
		if (_Volume >= DestVolume)
		{
			_Volume = DestVolume;
			SubVolume = 0;
			_Fade = FADE_NONE;
		}
		break;

	case FADE_OUT:
		
		_Volume += SubVolume;
		if (_Volume <= DestVolume)
		{
			_Volume = DestVolume;
			SubVolume = 0;
			_Fade = FADE_NONE;
		}
		break;

	case FADE_OUT_AND_STOP:
		
		_Volume += SubVolume;
		if (_Volume <= DestVolume)
		{
			_Fade = FADE_NONE;
			SubVolume = 0;
			alSourceStop(Buffer[Type].Source[id]);
		}
		break;

	default:
		break;
	}

	alSourcef(Buffer[Type].Source[id],AL_GAIN,_Volume*_MasterVolume);

	if (AutoRelease)
	{
		int state = 0;
		alGetSourcei(Buffer[Type].Source[id],AL_SOURCE_STATE,&state);
		if (state == AL_STOPPED)
		{
			Release();
		}
	}
}
void CSoundAL::Release(void)
{
	int state = 0;
	alGetSourcei(Buffer[Type].Source[id],AL_SOURCE_STATE,&state);
	if (state == AL_PLAYING)
	{
		alSourceStop(Buffer[Type].Source[id]);
	}
	Buffer[Type].Using[id] = false;

	delete this;
}

void CSoundAL::UpdateAll(void)
{
	CSoundAL* sound = Top;
	CSoundAL* next = NULL;
	SetListenerOri();
	while (sound)
	{
		next = sound->Next;
		sound->Update();
		sound = next;
	}
}

void CSoundAL::ReleaseAll(void)
{
	CSoundAL* sound = Top;
	CSoundAL* next = NULL;
	while (sound)
	{
		next = sound->Next;
		sound->Release();
		sound = next;
	}
}

void CSoundAL::CrossFade(CSoundAL* FadeOut,CSoundAL* FadeIn,int frame,bool autoStop)
{
	if (FadeOut != NULL,FadeIn != NULL)
	{
		FadeOut->SetFade(0,(int)(frame*0.8f),autoStop);
		FadeIn->SetFade(1.0f,frame);
	}
}

void CSoundAL::SetListenerOri(void)
{
	_ListenerOri[0] = _ListenerFrontVec.x;
	_ListenerOri[1] = _ListenerFrontVec.y;
	_ListenerOri[2] = _ListenerFrontVec.z;

	_ListenerOri[3] = _ListenerUpVec.x;
	_ListenerOri[4] = _ListenerUpVec.y;
	_ListenerOri[5] = _ListenerUpVec.z;

	alListenerfv(AL_ORIENTATION,_ListenerOri);
}

void CSoundAL::CheckPlaying(void)
{
	id = -1;
	for (int cnt = 0;cnt < MULTI_PLAY_NUM;cnt++)
	{
		if (!Buffer[Type].Using[cnt])
		{
			id = cnt;
			Buffer[Type].Using[cnt] = true;
			break;
		}
	}
}