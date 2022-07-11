#ifndef __SOUND_H_INCLUDED__
#define __SOUND_H_INCLUDED__
#include <string>

class Channel;
//�������������/��������������� ����������
bool SNDInitSound(HWND g_hWnd,bool bEnable3d,bool soft3d);
void SNDReleaseSound();
void* SNDGetDirectSound();//���������� ��������� �� LPDIRECTSOUND8

void SNDEnableSound(bool enable);
bool SNDIsSoundEnabled();

//������ � ��������
bool SNDEnableErrorLog(LPCSTR file);

void SNDSetVolume(float volume);//volume=0..1
void SNDSetFade(bool fadeIn,int time=0);
void SNDStopAll();
float SNDGetVolume();
void SNDSetGameActive(bool active);

////////////////////////////3D/////////////////////////////////
class SND3DListener
{
protected:
	friend struct SNDOneBuffer;
	friend class VirtualSound3D;
	friend class SoftSound3D;
	friend class HardSound3D;
	friend class SND3DSound;

	Mat3f rotate,invrotate;
	Vect3f position;
	//MatXf mat;
	Vect3f velocity;

	//��������� ��� software
	float s_distance_factor;
	float s_doppler_factor;
	float s_rolloff_factor;

	Vect3f front,top,right;

	float zmultiple;
public:
	SND3DListener();
	~SND3DListener();

	//��������� ���������� ����� (������ ����� �� ������ � ������������� �� ������� �������)
	//� ����-�� ��� �� �������� (�� �� ���� �� ����� �������)
	bool SetDistanceFactor(float);//1 - � ������, 1000 - � ����������
	bool SetDopplerFactor(float);//0..10, �� ��������� 1
	bool SetRolloffFactor(float);

	//SetPos ���� �������� ������ ����
	bool SetPos(const MatXf& mat);

	Vect3f GetPos(){return position;};

	//SetVelocity - ���������� �������� ������ ����
	//����� �� ����� ������ � SetDopplerFactor,SetRolloffFactor
	bool SetVelocity(const Vect3f& velocity);

	//������� ���������� ��� ������
	//���-�� ���������� �� Z ���� ������.
	//� �������� ������� ����� ��������\
	//zmul=0..1
	void SetZMultiple(float zmul){zmultiple=zmul;};
	float GetZMultiple(){return zmultiple;};

	//Update - �������� ����� ��������� ���������� (SetPos,...)
	//(���� ��� �� ����!)
	bool Update();
};

extern SND3DListener snd_listener;
#endif
