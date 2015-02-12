#ifndef _SOUND_AL_H_
#define _SOUND_AL_H_
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alut.h"
#include "main.h"
#include<Windows.h>
#pragma comment( lib, "OpenAL32.lib" ) 
#pragma comment( lib, "alut.lib" )

#define MULTI_PLAY_NUM (25)
typedef struct
{
	ALuint* Source;
	ALuint Buffer;
	bool* Using;
	bool loop;
	short MultiPlayNum;
	short PlayingNum;

}SOUND_BUFF;

class CSoundAL
{
public:
	typedef enum
	{
		BGM_TITLE = 0,
		BGM_CONNECT,
		BGM_GAME,
		BGM_RESULT,
		SE_ENTRY,
		SE_ENTER,
		SE_GAME_START,
		SOUND_MAX

	}SOUND;

	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_OUT_AND_STOP,
		FADE_MAX
	}FADE;

private:
	static SOUND_BUFF Buffer[SOUND_MAX];
	static CSoundAL* Top;
	static CSoundAL* Cur;
	static float _DefaultMaxDistance;//最大距離を個別に指定しなければ、自動的にこの変数の値となる
	static float _MasterVolume;//0～1.0f  ボリュームにこの値をかけた数値が実際の音量
	static VECTOR3 _ListenerPos;
	static VECTOR3 _ListenerSpeed;
	static VECTOR3 _ListenerFrontVec;
	static VECTOR3 _ListenerUpVec;
	static ALfloat _ListenerOri[6];
	static bool Device;
	CSoundAL* Next;
	CSoundAL* Prev;

	int Type;
	int id;
	float _Volume;
	float _MaxDistance;
	VECTOR3 _Pos;//音源位置
	VECTOR3 _Speed;//ドップラー効果がかかったりする?
	bool AutoRelease;//止まったら勝手にリリースされるか？
	FADE _Fade;
	float DestVolume;
	float SubVolume;
	static int _Num;
	bool _Loop;

	void LinkList(void);
	void UnlinkList(void);

	void Update(void);

	static void SetListenerOri(void);
	void CheckPlaying(void);

	float GetDuration(ALuint buffer);
	float GetOffset(ALuint source);
public:
	CSoundAL();
	~CSoundAL();
	static void Initialize(void);
	static void Finalize(void);
	static CSoundAL* Play(SOUND id,bool autoRelease = true);
	static CSoundAL* Play(SOUND id,const VECTOR3& pos,float distance = 0,bool autoRelease = true);
	void Init(void);
	void Uninit(void);

	void Stop(void);
	void Release(void);

	static void UpdateAll(void);
	static void ReleaseAll(void);
	static void FadeAll(int frame);
	static void FadeBGM(int frame);


	//------------------------------------アクセサ------------------------------------

	void SetPos(VECTOR3 pos){ _Pos = pos; alSourcefv(Buffer[Type].Source[id],AL_POSITION,_Pos.f); }
	void SetPosX(float x){ _Pos.x = x; alSource3f(Buffer[Type].Source[id],AL_POSITION,_Pos.x,_Pos.y,_Pos.z); }
	void SetPosY(float y){ _Pos.y = y; alSource3f(Buffer[Type].Source[id],AL_POSITION,_Pos.x,_Pos.y,_Pos.z); }
	void SetPosZ(float z){ _Pos.z = z; alSource3f(Buffer[Type].Source[id],AL_POSITION,_Pos.x,_Pos.y,_Pos.z); }

	void SetSpeed(VECTOR3 speed){ _Speed = speed; alSourcefv(Buffer[Type].Source[id],AL_VELOCITY,_Speed.f); }
	void SetSpeedX(float x){ _Speed.x = x; alSource3f(Buffer[Type].Source[id],AL_VELOCITY,_Speed.x,_Speed.y,_Speed.z); }
	void SetSpeedY(float y){ _Speed.y = y; alSource3f(Buffer[Type].Source[id],AL_VELOCITY,_Speed.x,_Speed.y,_Speed.z); }
	void SetSpeedZ(float z){ _Speed.z = z; alSource3f(Buffer[Type].Source[id],AL_VELOCITY,_Speed.x,_Speed.y,_Speed.z); }

	void SetVolume(float volume);
	void SetMaxDistance(float distance){ _MaxDistance = distance; }
	void SetFade(float destVolume,int frame,bool autoStop = false);

	void AddPos(VECTOR3 pos){ _Pos += pos; alSourcefv(Buffer[Type].Source[id],AL_POSITION,_Pos.f); }
	void AddPosX(float x){ _Pos.x += x; alSource3f(Buffer[Type].Source[id],AL_POSITION,_Pos.x,_Pos.y,_Pos.z); }
	void AddPosY(float y){ _Pos.y += y; alSource3f(Buffer[Type].Source[id],AL_POSITION,_Pos.x,_Pos.y,_Pos.z); }
	void AddPosZ(float z){ _Pos.z += z; alSource3f(Buffer[Type].Source[id],AL_POSITION,_Pos.x,_Pos.y,_Pos.z); }

	void AddSpeed(VECTOR3 speed){ _Speed += speed; alSourcefv(Buffer[Type].Source[id],AL_VELOCITY,_Speed.f); }
	void AddSpeedX(float x){ _Speed.x += x; alSource3f(Buffer[Type].Source[id],AL_VELOCITY,_Speed.x,_Speed.y,_Speed.z); }
	void AddSpeedY(float y){ _Speed.y += y; alSource3f(Buffer[Type].Source[id],AL_VELOCITY,_Speed.x,_Speed.y,_Speed.z); }
	void AddSpeedZ(float z){ _Speed.z += z; alSource3f(Buffer[Type].Source[id],AL_VELOCITY,_Speed.x,_Speed.y,_Speed.z); }

	void AddVolume(float volume);
	void AddMaxDistance(float distance){ _MaxDistance += distance; }

	static void SetMasterVolume(float volume);
	static void SetDefaultMaxDistance(float distance){ _DefaultMaxDistance = distance; }

	static void AddMasterVolume(float volume);
	static void AddDefaultMaxDistance(float distance){ _DefaultMaxDistance += distance; }

	static void SetListenerPos(VECTOR3 pos){ _ListenerPos = pos; alListenerfv(AL_POSITION,pos.f); }
	static void SetListenerPosX(float x){ _ListenerPos.x = x; alListenerfv(AL_POSITION,_ListenerPos.f); }
	static void SetListenerPosY(float y){ _ListenerPos.y = y; alListenerfv(AL_POSITION,_ListenerPos.f); }
	static void SetListenerPosZ(float z){ _ListenerPos.x = z; alListenerfv(AL_POSITION,_ListenerPos.f); }

	static void SetListenerVel(VECTOR3 speed){ _ListenerSpeed = speed; alListenerfv(AL_VELOCITY,speed.f); }
	static void SetListenerVelX(float x){ _ListenerSpeed.x = x; alListenerfv(AL_VELOCITY,_ListenerSpeed.f); }
	static void SetListenerVelY(float y){ _ListenerSpeed.y = y; alListenerfv(AL_VELOCITY,_ListenerSpeed.f); }
	static void SetListenerVelZ(float z){ _ListenerSpeed.x = z; alListenerfv(AL_VELOCITY,_ListenerSpeed.f); }

	static void SetListenerFVec(VECTOR3 vec){ _ListenerFrontVec = vec; }
	static void SetListenerUVec(VECTOR3 vec){ _ListenerUpVec = vec; }

	static void AddListenerPos(VECTOR3 pos){ _ListenerPos += pos; alListenerfv(AL_POSITION,pos.f); }
	static void AddListenerPosX(float x){ _ListenerPos.x += x; alListenerfv(AL_POSITION,_ListenerPos.f); }
	static void AddListenerPosY(float y){ _ListenerPos.y += y; alListenerfv(AL_POSITION,_ListenerPos.f); }
	static void AddListenerPosZ(float z){ _ListenerPos.x += z; alListenerfv(AL_POSITION,_ListenerPos.f); }

	static void AddListenerVel(VECTOR3 speed){ _ListenerSpeed += speed; alListenerfv(AL_VELOCITY,speed.f); }
	static void AddListenerVelX(float x){ _ListenerSpeed.x += x; alListenerfv(AL_VELOCITY,_ListenerSpeed.f); }
	static void AddListenerVelY(float y){ _ListenerSpeed.y += y; alListenerfv(AL_VELOCITY,_ListenerSpeed.f); }
	static void AddListenerVelZ(float z){ _ListenerSpeed.x += z; alListenerfv(AL_VELOCITY,_ListenerSpeed.f); }

	//=====================================================================
	//引数
	//FadeOut 消える方の音源
	//FadeIn 現れる方の音源
	//frame 何フレームかけるか？
	//autoStop 消えるほうが勝手にリリースされるか？(デフォルトはtrue)
	//=====================================================================
	static void CrossFade(CSoundAL* FadeOut,CSoundAL* FadeIn,int frame,bool autoStop = true);

	VECTOR3 Pos(void)const{ return _Pos; }
	VECTOR3 Speed(void)const{ return _Speed; }
	float Volume(void)const{ return _Volume; }
	float MaxDistance(void)const{ return _MaxDistance; }
	ALuint Source(void){ return Buffer[Type].Source[id]; }
	bool Loop(void){ return _Loop; }

	static VECTOR3 ListenerPos(void){ return _ListenerPos; }
	static VECTOR3 ListenerSpeed(void){ return _ListenerSpeed; }
	static float DefaultMaxDistance(void){ return _DefaultMaxDistance; }
	static float MasterVolume(void){ return _MasterVolume; }
	static VECTOR3 ListenerFrontVec(void){ return _ListenerFrontVec; }
	static VECTOR3 ListenerUpVec(void){ return _ListenerUpVec; }
	static int Num(void){ return _Num; }


};

inline void CSoundAL::SetVolume(float volume)
{
	if (!Device){ return; }
	if (volume < 0)
	{
		_Volume = 0;
	}
	else
	{
		_Volume = volume;
	}
}

inline void CSoundAL::SetMasterVolume(float volume)
{
	if (!Device){ return; }
	if (volume < 0)
	{
		_MasterVolume = 0;
	}
	else if (volume > 1.0f)
	{
		_MasterVolume = 1.0f;
	}
	else
	{
		_MasterVolume = volume;
	}
}

inline void CSoundAL::SetFade(float destVolume,int frame,bool autoStop)
{
	if (!Device){ return; }
	float Frame = (float)frame;

	SubVolume = (destVolume - _Volume)*(1.0f / Frame);


	if (SubVolume < 0)
	{
		(autoStop) ? _Fade = FADE_OUT_AND_STOP : _Fade = FADE_OUT;
		DestVolume = destVolume;
	}
	else if (SubVolume > 0)
	{
		_Fade = FADE_IN;
		DestVolume = destVolume;
	}
}
inline void CSoundAL::AddVolume(float volume)
{
	if (!Device){ return; }
	_Volume += volume;
	if (_Volume < 0)
	{
		_Volume = 0;
	}
}

inline void CSoundAL::AddMasterVolume(float volume)
{
	if (!Device){ return; }
	_MasterVolume += volume;
	if (_MasterVolume < 0)
	{
		_MasterVolume = 0;
	}
	else if (_MasterVolume > 1.0f)
	{
		_MasterVolume = 1.0f;
	}

}


#endif