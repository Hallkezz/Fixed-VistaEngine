#ifndef __INCTERRA_H__
#define __INCTERRA_H__

#include "Unknown.h"

typedef vector<Vect2s> Vect2sVect;

class TerraInterface:public cUnknownClass
{
public:
	virtual int SizeX()=0;
	virtual int SizeY()=0;
	virtual int GetZ(int x,int y)=0;//������ � ����� x,y
	virtual float GetZf(int x,int y)=0;

	virtual void GetZMinMax(int tile_x,int tile_y,int tile_dx,int tile_dy,BYTE& out_zmin,BYTE& out_zmax)=0;
	
	virtual class MultiRegion* GetRegion()=0;

	typedef void (*borderCall)(void* data,Vect2f& p);

	virtual void GetTileColor(char* tile,DWORD line_size,int xstart,int ystart,int xend,int yend,int step)=0;
	virtual void postInit(class cTileMap* tm)=0;

	//0..65536 - 
	virtual void GetTileZ(char* Texture,DWORD pitch,int xstart,int ystart,int xend,int yend,int step)=0;

	//��� ��������������� � �������� GetColumn,GetBorder ����� ����������, 
	//������ ��������� ��� �������
	virtual void LockColumn(){};
	virtual void UnlockColumn(){};
	virtual const Vect2f& GetWindVelocity(const int x, const int y){return Vect2f::ZERO;} //������� � �� ������
};

#endif //__INCTERRA_H__
