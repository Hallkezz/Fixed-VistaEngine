#include "StdAfx.h"
#include "UI_MinimapSymbol.h"
#include "Render\src\Texture.h"
#include "Serialization\Serialization.h"
#include "Serialization\EnumDescriptor.h"


// ------------------- UI_MinimapSymbol

UI_MinimapSymbol::UI_MinimapSymbol()
{
	type = SYMBOL_RECTANGLE;

	scaleByEvent = false;

	legionColor = false;
	color = Color4f(1.f, 1.f, 1.f, 1.f);

	scale = 1.0f;
}

void UI_MinimapSymbol::serialize(Archive& ar)
{
	ar.serialize(type, "type", "���");
	ar.serialize(scaleByEvent, "scaleByEvent", "������������� �� ������� �������/�����");
	if(!scaleByEvent)
		ar.serialize(scale, "selfScale", "����������� ������� �������");
	ar.serialize(legionColor, "useLegionColor", "������� � ���� �������");
	if(type == SYMBOL_SPRITE)
		ar.serialize(sprite, "sprite", "�����������");
	else if(!legionColor)
		ar.serialize(color, "color", "����������� ���� �������");
}

int UI_MinimapSymbol::lifeTime() const
{ 
	return sprite.isAnimated() ? max(sprite.texture()->GetTotalTime(), 100) : 100;
}


// ------------------- UI_MinimapEventStatic

UI_MinimapEventStatic::UI_MinimapEventStatic()
{
	isImportant_ = false;
	validTime_ = -1;
}

UI_MinimapEventStatic::UI_MinimapEventStatic(const UI_MinimapSymbol& symb)
: UI_MinimapSymbol(symb)
{
	isImportant_ = false;
	validTime_ = -1.f;
}

void UI_MinimapEventStatic::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(isImportant_, "isImportant", "������� ����������� ��� ��������");
	if(isImportant_)
		ar.serialize(validTime_, "validTime", "����� � ������� �������� ����� ������� � ����� �������������");
}

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(UI_MinimapSymbol, SymbolType, "UI_MinimapSymbol::Type")
REGISTER_ENUM_ENCLOSED(UI_MinimapSymbol, SYMBOL_RECTANGLE, "�������������")
REGISTER_ENUM_ENCLOSED(UI_MinimapSymbol, SYMBOL_SPRITE, "�����������")
END_ENUM_DESCRIPTOR_ENCLOSED(UI_MinimapSymbol, SymbolType)