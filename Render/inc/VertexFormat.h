#ifndef __VERTEX_FORMAT_H_INCLUDED__
#define __VERTEX_FORMAT_H_INCLUDED__

#include "umath.h"

#pragma pack(push,1)
/// ����� ������ �������� ������� - ��. ���� DirectX

struct sVertexXYZ
{
	Vect3f	pos;/// ��������� ������� � ���������� ������������
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZD : public sVertexXYZ
{
	sColor4c	diffuse;
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZT1: public sVertexXYZ
{ 
	float			uv[2];/// ����������� �� ��������0
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	inline Vect2f& GetTexel1()			{ return *((Vect2f*)&uv[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZDT1 : public sVertexXYZD
{
	float			uv[2];/// ����������� �� ��������1
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	inline Vect2f& GetTexel1()			{ return *((Vect2f*)&uv[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZDT2 : public sVertexXYZDT1
{
	float			uv2[2];/// ����������� �� ��������2
	inline float& u2()					{ return uv2[0]; }
	inline float& v2()					{ return uv2[1]; }
	inline Vect2f& GetTexel2()			{ return *((Vect2f*)&uv2[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZDS : public sVertexXYZD
{
	sColor4c	specular;
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZDST1 : public sVertexXYZDS
{
	float			uv[2];
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	inline Vect2f& GetTexel1()			{ return *((Vect2f*)&uv[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZDST2 : public sVertexXYZDST1
{
	float			uv[2];
	inline float& u2()					{ return uv[0]; }
	inline float& v2()					{ return uv[1]; }
	inline Vect2f& GetTexel2()			{ return *((Vect2f*)&uv[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZWT1
{
	float			x,y,z,w;/// ��������������� � ���������� ������������ ���������� �������. � ������ ��������� �� ����������� ������� ������.
	float			uv[2];
	Vect3f& GetVect3f()					{ return *(Vect3f*)&x; }
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZW
{
	float			x,y,z,w;
	inline int& xi()					{ return *((int*)&x); }
	inline int& yi()					{ return *((int*)&y); }
	inline int& zi()					{ return *((int*)&z); }
	Vect3f& GetVect3f()					{ return *(Vect3f*)&x; }
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZWD:public sVertexXYZW
{
	sColor4c	diffuse;
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZWDT1 : public sVertexXYZWD
{
	float			uv[2];
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	inline Vect2f& GetTexel()			{ return *((Vect2f*)&uv[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZWDT2 : public sVertexXYZWDT1
{
	float			uv[2];
	inline float& u2()					{ return uv[0]; }
	inline float& v2()					{ return uv[1]; }
	inline Vect2f& GetTexel2()			{ return *((Vect2f*)&uv[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZWDT3 : public sVertexXYZWDT2
{
	float			uv[2];
	inline float& u3()					{ return uv[0]; }
	inline float& v3()					{ return uv[1]; }
	inline Vect2f& GetTexel3()			{ return *((Vect2f*)&uv[0]); }
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZN : public sVertexXYZ
{
	Vect3f	n;/// �������
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZND : public sVertexXYZN
{
	sColor4c	diffuse;
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZNDS : public sVertexXYZND
{
	sColor4c	specular;
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZNDST1 : public sVertexXYZNDS
{
	float			uv[2];
	inline Vect2f& GetTexel()			{ return *(Vect2f*)&uv[0]; }
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZNDST2 : public sVertexXYZNDST1
{
	float			uv[2];
	inline float& u2()					{ return uv[0]; }
	inline float& v2()					{ return uv[1]; }
	inline Vect2f& GetTexel2()			{ return *(Vect2f*)&uv[0]; }
	static struct IDirect3DVertexDeclaration9* declaration;
};
struct sVertexXYZNT1 : public sVertexXYZN
{
	float			uv[2];
	inline Vect2f& GetTexel()			{ return *(Vect2f*)&uv[0]; }
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct sVertexXYZINT1:public sVertexXYZ
{
	BYTE index[4];
	Vect3f	n;
	float			uv[2];
	inline Vect2f& GetTexel()			{ return *(Vect2f*)&uv[0]; }
	inline float& u1()					{ return uv[0]; }
	inline float& v1()					{ return uv[1]; }
	static struct IDirect3DVertexDeclaration9* declaration;
};

class cSkinVertex
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
	int vb_size;
	void* p;
	float* cur;
	int offset_texel;
	int offset_bump_s;
	int offset_bump_t;
	int offset_texel2;
public:
	static IDirect3DVertexDeclaration9* declaration;
	static IDirect3DVertexDeclaration9* declaration_diffuse;
	static IDirect3DVertexDeclaration9* declaration_bump;
	static IDirect3DVertexDeclaration9* declaration_diffuse_bump;
	static IDirect3DVertexDeclaration9* declaration_uv2;
	static IDirect3DVertexDeclaration9* declaration_diffuse_uv2;
	static IDirect3DVertexDeclaration9* declaration_bump_uv2;
	static IDirect3DVertexDeclaration9* declaration_diffuse_bump_uv2;

	cSkinVertex(int num_weight_,bool bump_,bool uv2_);
	void SetVB(void* p_,int vb_size_){p=p_;vb_size=vb_size_;}
	void Select(int n){cur=(float*)(vb_size*n+(char*)p);}
	Vect3f& GetPos(){return *(Vect3f*)cur;}
	sColor4c& GetIndex(){return *(sColor4c*)(3+cur);}
	Vect3f& GetNorm(){return *(Vect3f*)(4+cur);}
	BYTE& GetWeight(int idx);
	Vect2f& GetTexel(){return *(Vect2f*)(offset_texel+cur);}

	/// S,T,SxT=N - ������� �������������� � ������������ ������������ ��� �����.
	Vect3f& GetBumpS(){return *(Vect3f*)(offset_bump_s+cur);}
	Vect3f& GetBumpT(){return *(Vect3f*)(offset_bump_t+cur);}

	Vect2f& GetTexel2(){return *(Vect2f*)(offset_texel2+cur);}

	IDirect3DVertexDeclaration9* GetDeclaration();
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
	sColor4c	diffuse;
	static struct IDirect3DVertexDeclaration9* declaration;
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
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct shortVertexXYZD:public shortVertexXYZ
{
	sColor4c	diffuse;
	static struct IDirect3DVertexDeclaration9* declaration;
};

struct shortVertexGrass : public shortVertexXYZD
{
	sColor4c n;
	short uv[4];
	float uv2;
	static struct IDirect3DVertexDeclaration9* declaration;
};
#pragma pack(pop)

#endif
