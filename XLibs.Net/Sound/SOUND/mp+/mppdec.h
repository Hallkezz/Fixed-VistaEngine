#pragma once
/*
	���� ��������������� ������ � 
	������ 44 KHz 16 bit Stereo
*/

bool MpegOpen(const char* file);
bool MpegGetNextSample(short*& buffer,int& len);
void MpegClose();
