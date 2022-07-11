#ifndef _TEXTURE_ATLAS_H_
#define _TEXTURE_ATLAS_H_

#include "XMath\Rectangle4f.h"

/*
	��������� ���������� ��������� � �����.
	���� ������ �������� �� ������ ������� ������
*/

class cTextureAtlas
{
public:
	cTextureAtlas();
	~cTextureAtlas();
	bool Init(vector<Vect2i>& texture_size,bool line=false);
	void FillTexture(int index,DWORD* data,int dx,int dy);

	int GetNumTextures(){ return texture_data.size();}
	sRectangle4f GetUV(int index);
	Vect2i GetSize(int index);
	int GetDX(){return texture_size.x;};
	int GetDY(){return texture_size.y;};

	DWORD* GetTexture(){return texture;}
	int GetMaxMip();//���������� ���������� ��� �������, ���������� ��� �������, ������� ����� ������� � ��������.
protected:
	struct Tex
	{
		Vect2i size;
		Vect2i pow2_size;
		Vect2i pos;
	};
	vector<Tex> texture_data;
	vector<int> sort_by_size;

	DWORD* texture;
	Vect2i texture_size;

	friend struct cTextureAtlasSort;

	bool TryCompact();

	void CompactRecursive(int& cur,int position_x,int position_y,int size);
};

#endif  _TEXTURE_ATLAS_H_
