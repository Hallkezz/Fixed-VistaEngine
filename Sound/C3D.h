#ifndef __C3_D_H_INCLUDED__
#define __C3_D_H_INCLUDED__

#include "SoftwareBuffer.h"
#include "..\Util\StaticMap.h"

struct ScriptParam;

struct SNDOneBuffer
{
	ScriptParam* script;

	Vect3f pos,velocity;
	float volume;

	LPDIRECTSOUNDBUFFER buffer;
	DWORD nSamplesPerSec;//������� ������ ��. WAVEFORMATEX
	DWORD nAvgBytesPerSec;//���������� ������ � �������

	VirtualSound3D* p3DBuffer;

	void* pSound;//SND2DSound ��� SND3DSound
	bool used,played_cycled;

	int pause_level;//���� �� 0, �� ���� ���������� �������� SNDPausePush();

	double begin_play_time;

	SNDOneBuffer();
	~SNDOneBuffer();

	inline HRESULT RecalculatePos();
	inline void RecalculateVolume();

	//�������������� ������� ������������� �������
	HRESULT PlayPreprocessing();

	bool SetFrequency(float frequency);
};

struct ScriptParam
{
	string sound_name;

	float radius;//
	float max_radius;
	int max_num_sound;
	float def_volume;
	float min_volume;

	bool delta_random;
	float delta_up,delta_down;

	ScriptParam():
		def_volume(1.0f)
	{};

	void LoadSound(const char* name,bool b2d);

	void Release();

	//������������, ����� ������ ���� ClipDistance, ����� ������� �� ����� max_num_sound
	void RecalculateClipDistance();

	MTSection* GetLock(){return &mtlock;};
	vector<LPDIRECTSOUNDBUFFER>& GetSounds(){ASSERT(mtlock.is_lock());return sounds;}
	vector<SNDOneBuffer>& GetBuffer(){ASSERT(mtlock.is_lock());return soundbuffer;}
protected:
	MTSection mtlock;
	//�����, ������� ���������� ��� �������� �� RND
	vector<LPDIRECTSOUNDBUFFER> sounds;
	vector<SNDOneBuffer> soundbuffer;
};

struct SoundScriptPrm;

class SNDScript
{
	struct ltstr
	{
	  bool operator()(LPCSTR s1,LPCSTR s2) const
	  {
		return strcmp(s1, s2) < 0;
	  }
	};

public:
	typedef StaticMap<LPCSTR, ScriptParam*, ltstr> MapScript;

	bool map_script_updated;
	MapScript map_script;
	typedef list<ScriptParam> SoundArray;
	SoundArray sounds;

	bool b2d;
public:
	SNDScript(bool b2d);
	~SNDScript();

	bool AddSound(struct SNDSoundParam& snd);
	void RemoveAll();

	ScriptParam* Find(LPCSTR name);
	bool FindFree(LPCSTR name,ScriptParam*& script,int& nfree);

	void PauseAllPlayed(int pause_level);
	void PlayByLevel(int pause_level);

	void StopAll();
protected:

	void RebuildMapScript();
	bool RemoveScriptInternal(LPCSTR name);
};

extern SNDScript script3d;
extern SNDScript script2d;
/////////////////////////inline//////////////////////////
#endif
