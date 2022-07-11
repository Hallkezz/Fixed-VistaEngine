#ifndef _VISGENERIC_DEFINE_H_
#define _VISGENERIC_DEFINE_H_

enum AttributeCamera
{
	ATTRCAMERA_PERSPECTIVE			=	1<<4,	// �����������
	ATTRCAMERA_WRITE_ALPHA			=	1<<5, 			
	ATTRCAMERA_ZBUFFER				=	1<<6, 	
	ATTRCAMERA_FLOAT_ZBUFFER		=	1<<7, 	
	ATTRCAMERA_MIRAGE				=   1<<13,
	ATTRCAMERA_REFLECTION			=	1<<16,	// ������ �������� ������-���������
	ATTRCAMERA_SHADOW				=	1<<17,	// ������ �������� ������-����
	ATTRCAMERA_CLEARZBUFFER			=	1<<19,
	ATTRCAMERA_SHOWCLIP				=	1<<20,
	ATTRCAMERA_SHADOWMAP			=	1<<21,
	ATTRCAMERA_ZMINMAX				=	1<<22,
	ATTRCAMERA_NOT_CALC_PROJ		=	1<<23,
	ATTRCAMERA_ZINVERT				=	1<<24,
	ATTRCAMERA_NOCLEARTARGET		=	1<<25,
	ATTRCAMERA_NOZWRITE				=	1<<26,
};

enum eAttributeUnkObj
{
// general
	ATTRUNKOBJ_IGNORE				=	1<<0,	// ������ ������������ = �������� ���������� = �� ���������
	ATTRUNKOBJ_DELETED				=	1<<1,
	ATTRUNKOBJ_ATTACHED				=	1<<2,		
	ATTRUNKOBJ_CREATED_IN_LOGIC		=	1<<3,	//������� ��������� � ���������� ������ ������ ��������� ����� ���� ��� �� ��� ��� ������ � ������������

	ATTRUNKOBJ_ADDBLEND				=	1<<7,	// �������� ������, ������ ��������� � MAT_ALPHA_ADDBLEND
	ATTRUNKOBJ_COLLISIONTRACE		=	1<<9,	// ��������� ��� �����������
	ATTRUNKOBJ_NO_USELOD			=	1<<12,
	ATTRUNKOBJ_MIRAGE				=	ATTRCAMERA_MIRAGE,

	ATTRUNKOBJ_REFLECTION			=	ATTRCAMERA_REFLECTION,//==16 ������ ����� ����������
	ATTRUNKOBJ_SHADOW				=	ATTRCAMERA_SHADOW,	//==17 ������ ���������� ���������� ���� (�� ���� ��� ���������, �� ATTRCAMERA_SHADOWMAP)
	ATTR3DX_NOUPDATEMATRIX			=	1<<18,  //��� 3dx �� ������������� �������
	ATTRUNKOBJ_IGNORE_NORMALCAMERA	=	1<<20,	// ������ �� ��������� � ���������� ������

	//3dx
	ATTR3DX_UNDERWATER				=   1<<22,
	ATTR3DX_NO_RESIZE_TEXTURES		=   1<<23,  // 
	ATTRUNKOBJ_2PASS_ZBUFFER		=   1<<24,  // ��� �������������� ��������, ���� ��������� ������� � Z ������ ��� ��������� � ColorBuffer,
												// � ������ �������� ��������� � ColorBuffer, ��� ��������� � Z ������
	ATTRUNKOBJ_SHOW_FLAT_SILHOUETTE	=	1<<26,  // ��� ��������, ������� ����� �����, �� ������� ���������  � ���� ���������� ������������ �������.
												// ���������� ������ ��� ���������� RENDERDEVICE_MODE_STENCIL
	ATTRUNKOBJ_HIDE_BY_DISTANCE		=	1<<27,  // ������ ��������� ���� �������, ����� ���������� ������� ���������.
	ATTR3DX_HIDE_LIGHTS				=   1<<28,  // ��������� ����� ��������� ���� ��������.
	ATTR3DX_NO_SELFILLUMINATION		=   1<<29,  // ����������� �������� ������������ (������������ �������� EnableSelfIllumination)
	ATTR3DX_ALWAYS_FLAT_SILUETTE	=	1<<30,
	ATTRUNKOBJ_NOLIGHT				=	1<<31,	// ������ �� ���������� ����������� ����� �����
};

enum eAttributeLight
{
// general
	ATTRLIGHT_IGNORE				=	1<<0,	// �������� ����� ������������ = �������� ���������� = �� ���������
// private
	ATTRLIGHT_DIRECTION				=	1<<4,	// ����������� �������� �����
	ATTRLIGHT_SPHERICAL_SPRITE		=	1<<5,	// �������� ����� �������� ����� ��������
	//ATTRLIGHT_SPHERICAL_OBJECT � ATTRLIGHT_SPHERICAL_TERRAIN ����� ������������� ������ ��� ��������.
	ATTRLIGHT_SPHERICAL_OBJECT		=	1<<6,	// ����������� �������� �����, ����������� ���� ������(!!!!)�� �������
	ATTRLIGHT_SPHERICAL_TERRAIN		=   1<<7,	// ����������� �������� �����, ����������� ���� ������ �� �����
};

enum eAttributeSimply
{
	ATTRSIMPLY3DX_OPACITY				=	1<<26,  //���������� ����, �� �������������.
};

#endif // _VISGENERIC_DEFINE_H_
