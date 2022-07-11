#ifndef __VERTEX_FORMAT_H_INCLUDED__
#define __VERTEX_FORMAT_H_INCLUDED__

#include "Render\3dx\umath.h"
#include "XMath\Colors.h"

struct IDirect3DVertexDeclaration9;

#pragma pack(push,1)
/// ����� ������ �������� ������� - ��. ���� DirectX

struct RENDER_API sVertexXYZ
{
	Vect3f	pos;/// ��������� ������� � ���������� ������������
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZD : public sVertexXYZ
{
	Color4c	diffuse;
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZT1: public sVertexXYZ
{ 
	float			uv[2];/// ����������� �� ��������0
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	inline Vect2f& GetTexel1()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZDT1 : public sVertexXYZD
{
	float			uv[2];/// ����������� �� ��������1
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	inline Vect2f& GetTexel1()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZDT2 : public sVertexXYZDT1
{
	float			uv2[2];/// ����������� �� ��������2
	inline float& u2()					{ return uv2[0]; }
	inline float& v2()					{ return uv2[1]; }
	inline Vect2f& GetTexel2()			{ return *((Vect2f*)&uv2[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZDS : public sVertexXYZD
{
	Color4c	specular;
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZDST1 : public sVertexXYZDS
{
	float			uv[2];
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	inline Vect2f& GetTexel1()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZDST2 : public sVertexXYZDST1
{
	float			uv[2];
	inline float& u2()					{ return uv[0]; }
	inline float& v2()					{ return uv[1]; }
	inline Vect2f& GetTexel2()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZWT1
{
	float			x,y,z,w;/// ��������������� � ���������� ������������ ��������� �������. � ������ ��������� �� ����������� ������� ������.
	float			uv[2];
	Vect3f& GetVect3f()					{ return *(Vect3f*)&x; }
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZW
{
	float			x,y,z,w;
	inline int& xi()					{ return *((int*)&x); }
	inline int& yi()					{ return *((int*)&y); }
	inline int& zi()					{ return *((int*)&z); }
	Vect3f& GetVect3f()					{ return *(Vect3f*)&x; }
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZWD : public sVertexXYZW
{
	Color4c	diffuse;
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZWDT1 : public sVertexXYZWD
{
	float			uv[2];
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZWDT2 : public sVertexXYZWDT1
{
	float			uv[2];
	inline float& u2()					{ return uv[0]; }
	inline float& v2()					{ return uv[1]; }
	inline Vect2f& GetTexel2()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZWDT3 : public sVertexXYZWDT2
{
	float			uv[2];
	inline float& u3()					{ return uv[0]; }
	inline float& v3()					{ return uv[1]; }
	inline Vect2f& GetTexel3()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZWDT4 : public sVertexXYZWDT3
{
	float			uv[2];
	inline float& u4()					{ return uv[0]; }
	inline float& v4()					{ return uv[1]; }
	inline Vect2f& GetTexel4()			{ return *((Vect2f*)&uv[0]); }
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZN : public sVertexXYZ
{
	Vect3f	n;/// �������
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZND : public sVertexXYZN
{
	Color4c	diffuse;
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZNDS : public sVertexXYZND
{
	Color4c	specular;
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZNDST1 : public sVertexXYZNDS
{
	float			uv[2];
	inline Vect2f& GetTexel()			{ return *(Vect2f*)&uv[0]; }
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZNDST2 : public sVertexXYZNDST1
{
	float			uv[2];
	inline float& u2()					{ return uv[0]; }
	inline float& v2()					{ return uv[1]; }
	inline Vect2f& GetTexel2()			{ return *(Vect2f*)&uv[0]; }
	static IDirect3DVertexDeclaration9* declaration;
};
struct RENDER_API sVertexXYZNT1 : public sVertexXYZN
{
	float			uv[2];
	inline Vect2f& GetTexel()			{ return *(Vect2f*)&uv[0]; }
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	static IDirect3DVertexDeclaration9* declaration;
};

struct RENDER_API sVertexXYZINT1 : public sVertexXYZ
{
	BYTE index[4];
	Vect3f	n;
	float			uv[2];
	inline Vect2f& GetTexel()			{ return *(Vect2f*)&uv[0]; }
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	static IDirect3DVertexDeclaration9* declaration;
};

class RENDER_API cSkinVertex
{
/*��������� ������ ��� �������� � 1..4 ������� � ���������� �����.
  ��� ���� ��� � ������� ������ ����� �������� � ���� ��������.
	Vect3f	pos;
	BYTE index[4];
	Vect3f	n;
	BYTE weight[4];
	float			uv[2];
*/
	int num_weight;
	bool bump;
	bool uv2;
	bool fur;
	int vb_size;
	void* p;
	float* cur;
	int offset_texel;
	int offset_bump_s;
	int offset_bump_t;
	int offset_texel2;
	int offset_fur;
public:
	static IDirect3DVertexDeclaration9* declaration[2][2][2][2];

	cSkinVertex(int num_weight_,bool bump_,bool uv2_,bool fur_);
	void SetVB(void* p_,int vb_size_){p=p_;vb_size=vb_size_;}
	void Select(int n){cur=(float*)(vb_size*n+(char*)p);}
	Vect3f& GetPos(){return *(Vect3f*)cur;}
	Color4c& GetIndex(){return *(Color4c*)(3+cur);}
	Vect3f& GetNorm(){return *(Vect3f*)(4+cur);}
	BYTE& GetWeight(int idx);
	Vect2f& GetTexel(){return *(Vect2f*)(offset_texel+cur);}

	/// S,T,SxT=N - ������� �������������� � ������������ ������������ ��� �����.
	Vect3f& GetBumpS(){return *(Vect3f*)(offset_bump_s+cur);}
	Vect3f& GetBumpT(){return *(Vect3f*)(offset_bump_t+cur);}

	Vect2f& GetTexel2(){return *(Vect2f*)(offset_texel2+cur);}

	Color4c& GetFur(){return *(Color4c*)(offset_fur+cur);}

	IDirect3DVertexDeclaration9* GetDeclaration();

	static void Register();
protected:
};
/*
��� ����� ���������.
���� 4*3*4=48 ���� �� ������, ������� ������� ����������. GetPos, GetNorm, GetBumpS, GetBumpT
+ GetIndex, GetWeight, GetTexel -> 16 ���� �� ������� ��������� ����������
1) GetPos, GetNorm, GetBumpS, GetBumpT -> short -> 3*4*2=24 �����, 3 ��������� + 3 �����������. (�������� ����������� �� ����� ���� ��������� ��� ��������).
2) GetTexel -> short -> 4 ����� , +1 ���������.
3) GetNorm, GetBumpS, GetBumpT -> color -> 3*4=12 ����, ��� �������������� ���������, �� �������� �����, ���� � ����������� � ��������� ����� ��������.
4) T=cross(N,S) -> 2 ��������, -12 ����
*/

struct sVertexD
{
	Color4c	diffuse;
	static IDirect3DVertexDeclaration9* declaration;
};

struct sShort2
{
	short x;
	short y;
};

struct sShort4
{
	short x;
	short y;
	short z;
	short w;
};

struct shortVertexXYZ
{
	sShort4 pos;
	static IDirect3DVertexDeclaration9* declaration;
};

struct shortVertexXYZD : public shortVertexXYZ
{
	Color4c	diffuse;
	static IDirect3DVertexDeclaration9* declaration;
};

struct shortVertexGrass : public shortVertexXYZD
{
	Color4c n;
	short uv[4];
	float uv2;
	static IDirect3DVertexDeclaration9* declaration;
};
#pragma pack(pop)

#endif
