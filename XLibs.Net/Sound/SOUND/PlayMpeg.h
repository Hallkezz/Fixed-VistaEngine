#pragma once
//
bool MpegInitLibrary(void* LPDIRECTSOUND_pDS);

//�������� �� ��������������� DirectSound
void MpegDeinitLibrary();

bool MpegOpenToPlay(const char* fname,bool cycled);
void MpegStop();
void MpegPause();
void MpegResume();

enum MpegState
{
	MPEG_STOP=0,
	MPEG_PLAY=1,
	MPEG_PAUSE=2,
};

MpegState MpegIsPlay();


void MpegSetVolume(int volume);//0..255
int MpegGetVolume();


//��������� �������, ������ ����������� ����������� ����� ��������� ����������.
//���������� true ���� ������� ��������� � �������.
//� ������� ����� ���� �� ����� 3x ������

bool MpegSteamIsEmpty();//��� ��������� ������

//��������� ��������� c �������� �������� �� volume  �� time ����������
bool MpegStreamVolumeTo(int volume,DWORD time);
bool MpegStreamSetVolume(int volume);
bool MpegStreamOpenToPlay(const char* fname,bool cycled);
