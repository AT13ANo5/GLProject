#define _CRT_SECURE_NO_WARNINGS
#include "SoundAL.h"
#include<stdio.h>

CSoundAL* CSoundAL::Top = nullptr;
CSoundAL* CSoundAL::Cur = nullptr;
float CSoundAL::_DefaultMaxDistance = 25000.0f;
float CSoundAL::_MasterVolume = 1.0f;
int CSoundAL::_Num = 0;
bool CSoundAL::Device = false;
VECTOR3 CSoundAL::_ListenerPos = VECTOR3(0,0,0);
VECTOR3 CSoundAL::_ListenerSpeed = VECTOR3(0,0,0);

VECTOR3 CSoundAL::_ListenerFrontVec = VECTOR3(0,0,0);
VECTOR3 CSoundAL::_ListenerUpVec = VECTOR3(0,0,0);
ALfloat CSoundAL::_ListenerOri[6] = { 0 };

typedef struct
{
	char* file;
	bool loop;
	short MultiPlayNum;
}PARAM;
const PARAM File[CSoundAL::SOUND_MAX] =
{
	{ "data/sound/BGM/Title.wav",true,2 },
	{ "data/sound/BGM/Connect.wav",true,2 },
	{ "data/sound/BGM/Game.wav",true,2 },
	{ "data/sound/BGM/Result.wav",true,2 },
	{ "data/sound/SE/Entry.wav",false,2 },
	{ "data/sound/SE/PushEnter.wav",false,2 },
	{ "data/sound/SE/ToGame.wav",false,2 },
	{ "data/sound/SE/Cannon.wav",false,7 },

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
	char* argv[] = { nullptr };
	ALboolean  succece = false;
	succece = alutInit(&argc,argv);
	if (succece == false)
	{
		char error[512] = { 0 };
		strcpy(error,alutGetErrorString(alutGetError()));
		MessageBox(nullptr,error,"ERROR",MB_OK);
		return;
	}
	Device = true;
	alDistanceModel(AL_LINEAR_DISTANCE);

	for (int cnt = 0;cnt < SOUND_MAX;cnt++)
	{
		Buffer[cnt].MultiPlayNum = File[cnt].MultiPlayNum;
		Buffer[cnt].PlayingNum = 0;
		Buffer[cnt].Source = new ALuint[Buffer[cnt].MultiPlayNum];
		Buffer[cnt].Using = new bool[Buffer[cnt].MultiPlayNum];

		alGenBuffers(1,&Buffer[cnt].Buffer);
		Buffer[cnt].Buffer = alutCreateBufferFromFile(File[cnt].file);
		if (Buffer[cnt].Buffer == 0)
		{
			Buffer[cnt].Buffer = alutCreateBufferFromFile(File[cnt].file);
			if (Buffer[cnt].Buffer == 0)
			{
				//MessageBox(nullptr,File[cnt].file,"バッファの作成に失敗しました",MB_OK);
			}
		}
		Buffer[cnt].loop = File[cnt].loop;
		alGenSources(Buffer[cnt].MultiPlayNum,Buffer[cnt].Source);

		for (int num = 0;num < Buffer[cnt].MultiPlayNum;num++)
		{
			Buffer[cnt].Using[num] = false;
			alSourcei(Buffer[cnt].Source[num],AL_BUFFER,Buffer[cnt].Buffer);
			if (Buffer[cnt].Source[num] == 0)
			{
				MessageBox(nullptr,"ソースの生成に失敗しました","ERROR",MB_OK);
			}
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
	ReleaseAll();
	if (!Device){ return; }
	for (int cnt = 0;cnt < SOUND_MAX;cnt++)
	{
		alDeleteSources(Buffer[cnt].MultiPlayNum,Buffer[cnt].Source);
		alDeleteBuffers(Buffer[cnt].MultiPlayNum,&Buffer[cnt].Buffer);
		SafeDeletes(Buffer[cnt].Using);
		SafeDeletes(Buffer[cnt].Source);
		
	}
	alutExit();
}

CSoundAL* CSoundAL::Play(SOUND id,bool autoRelease)
{
	if (!Device){ return nullptr; }
	if (Buffer[id].MultiPlayNum < Buffer[id].PlayingNum){ return nullptr; }
	CSoundAL* sound = new CSoundAL;
	if (sound == nullptr){ return nullptr; }
	sound->Type = id;
	sound->CheckPlaying();
	if (sound->id < 0)
	{
		SafeDelete(sound);
		return nullptr;
	}
	sound->AutoRelease = autoRelease;
	sound->Init();

	return sound;
}
CSoundAL* CSoundAL::Play(SOUND id,const VECTOR3& pos,float distance,bool autoRelease)
{
	if (!Device){ return nullptr; }
	if (Buffer[id].MultiPlayNum < Buffer[id].PlayingNum){ return nullptr; }
	
	VECTOR3 dis = pos - _ListenerPos;
	
	if (distance > 0)
	{
		if (((dis.x*dis.x) + (dis.z*dis.z)) > ((distance)*(distance)))
		{
			return nullptr;
		}
	}
	else
	{
		if (((dis.x*dis.x) + (dis.z*dis.z)) > ((_DefaultMaxDistance)*(_DefaultMaxDistance)))
		{
			return nullptr;
		}
	}
	
	CSoundAL* sound = new CSoundAL;
	if (sound == nullptr){ return nullptr; }
	sound->Type = id;
	sound->CheckPlaying();
	if (sound->id < 0)
	{
		SafeDelete(sound);
		return nullptr;
	}
	sound->AutoRelease = autoRelease;
	sound->Init();
	sound->SetPos(pos);

	return sound;
}
void CSoundAL::Init(void)
{
	_MaxDistance = _DefaultMaxDistance;
	_Volume = _MasterVolume;
	if (Buffer[Type].loop == false)
	{
		_Volume *= 0.5f;
	}
	_Loop = Buffer[Type].loop;
	alSourcef(Buffer[Type].Source[id],AL_GAIN,_Volume);
	alSourcef(Buffer[Type].Source[id],AL_MIN_GAIN,0.0f);
	alSourcef(Buffer[Type].Source[id],AL_MAX_GAIN,_Volume);
	alSourcef(Buffer[Type].Source[id],AL_MAX_DISTANCE,_MaxDistance);

	alSourcePlay(Buffer[Type].Source[id]);

	Buffer[Type].PlayingNum++;
	//alutSleep(1);
}
void CSoundAL::Uninit(void)
{

}

void CSoundAL::Stop(void)
{
	if (!Device){ return; }
	int state = 0;
	alGetSourcei(Buffer[Type].Source[id],AL_SOURCE_STATE,&state);
	if (state == AL_PLAYING)
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
	if (!Device){ delete this;return; }
	int state = 0;
	alGetSourcei(Buffer[Type].Source[id],AL_SOURCE_STATE,&state);
	if (state == AL_PLAYING)
	{
		alSourceStop(Buffer[Type].Source[id]);
	}
	Buffer[Type].Using[id] = false;
	Buffer[Type].PlayingNum--;

	delete this;
}

void CSoundAL::UpdateAll(void)
{
	if (!Device){ return; }
	CSoundAL* sound = Top;
	CSoundAL* next = nullptr;
	SetListenerOri();
	while (sound)
	{
		next = sound->Next;
		sound->Update();
		sound = next;
	}

#ifdef _DEBUG
#endif
}

void CSoundAL::ReleaseAll(void)
{
	CSoundAL* sound = Top;
	CSoundAL* next = nullptr;
	while (sound)
	{
		next = sound->Next;
		sound->Release();
		sound = next;
	}
}

void CSoundAL::CrossFade(CSoundAL* FadeOut,CSoundAL* FadeIn,int frame,bool autoStop)
{
	if (!Device){ return; }
	if (FadeOut != nullptr&&FadeIn != nullptr)
	{
		FadeOut->SetFade(0,(int)(frame*0.8f),autoStop);
		FadeIn->SetFade(1.0f,frame);
	}
}

void CSoundAL::SetListenerOri(void)
{
	if (!Device){ return; }
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
	if (!Device){ return; }
	for (int cnt = 0;cnt < Buffer[Type].MultiPlayNum;cnt++)
	{
		if (!Buffer[Type].Using[cnt])
		{
			id = cnt;
			Buffer[Type].Using[cnt] = true;
			break;
		}
	}
}
void CSoundAL::FadeAll(int frame)
{
	CSoundAL* sound = Top;

	while (sound)
	{
		sound->SetFade(0,frame,true);

		sound = sound->Next;
	}
}

void CSoundAL::FadeBGM(int frame)
{
	CSoundAL* sound = Top;

	while (sound)
	{
		if (sound->Loop())
		{
			sound->SetFade(0,frame,true);
		}

		sound = sound->Next;
	}
}


float CSoundAL::GetDuration(ALuint buffer)
{
	ALint size,frequency,channels,bits;

	alGetBufferi(buffer,AL_SIZE,&size);
	alGetBufferi(buffer,AL_FREQUENCY,&frequency);
	alGetBufferi(buffer,AL_CHANNELS,&channels);
	alGetBufferi(buffer,AL_BITS,&bits);

	return (float)size / (float)(frequency*channels*(bits / 8));

}

float CSoundAL::GetOffset(ALuint source) 
{

	float offset;

	alGetSourcef(source,AL_SEC_OFFSET,&offset);

	return offset;

}