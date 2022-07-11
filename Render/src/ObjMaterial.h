#ifndef __OBJMATERIAL_H__
#define __OBJMATERIAL_H__

enum eAttributeMaterial
{
	ATTRIBUTE_MATERIAL_NULL		=	0<<0,
	// ��������� ����������� ���������	1..7
	ATTRIBUTE_COLOR_MUL			=	1<<1,	// �������� �������� ����������������� ����
	ATTRIBUTE_COLOR_ADD			=	1<<2,	// �������� �������� ���������� ����
	ATTRIBUTE_ALPHA				=	1<<3,	// �������� �������� ������������
	ATTRIBUTE_TEXTURE			=	1<<4,	// �������� �������� ��������
	ATTRIBUTE_OPACITY			=	1<<5,	// �������� � ������������� � ��������
	ATTRIBUTE_ALPHATEST			=	1<<6,	// �������� � ��������� ��� ������� ���� �������� ���������
//	ATTRIBUTE_BUMP				=	1<<7,	// �������� � ���������� �����
	// ��������� ������������� ����	10..15
	ATTRIBUTE_PLASTIC			=	1<<10,	// ���������� - ������ ������������ ��� ���������
	ATTRIBUTE_METAL				=	1<<11	// ������ - ������ ������������ ��� ����������� ������������ � ������
};

// cObjectMaterial - ����� ��������� ��� �������� (KIND_OBJ_MATERIAL,TYPE_NULL)
// ������� �������� �� ������ �� ����������, �������� ����������� ��������� �������-������� � �.�.
class cObjectMaterial : public cUnknownClass
{	// �������� �������, ���� ��������� ��� = texture * MulColor + AddColor
	eAttributeMaterial	attribute;

	cUnknownClass		*Texture;		// ������ ������������� ��������
//	cUnknownClass		*BumpMap;

	sColor4f			MulCol;			// ����������������� ���������� �����
	sColor4f			AddCol;			// ���������� ���������� �����
public:
	cObjectMaterial():cUnknownClass(KIND_OBJ_MATERIAL)	{ Texture=0; attribute=ATTRIBUTE_MATERIAL_NULL; MulCol.set(1,1,1,1); AddCol.set(0,0,0,1); }
	cObjectMaterial(cObjectMaterial *Material)			{ attribute=Material->attribute; if(Texture=Material->Texture) Texture->IncRef(); MulCol=Material->MulColor(); AddCol=Material->AddColor(); }
	~cObjectMaterial()									{ Release(); }
	void Release();

	void DelTexture(cUnknownClass *Texture);
	void SetTexture(cUnknownClass *Texture,int number=0);
	void SetAttribute(int Attribute)					{ attribute=eAttributeMaterial(Attribute); }
	int GetAttribute(int Attribute)						{ return attribute&Attribute; }
	int GetAttribute()									{ return attribute; }

	inline sColor4f& MulColor()							{ return MulCol; }
	inline sColor4f& AddColor()							{ return AddCol; }

	inline void SetMulColor(sColor4f& color);
	inline void SetAddColor(sColor4f& color);
};

#endif //__OBJMATERIAL_H__
