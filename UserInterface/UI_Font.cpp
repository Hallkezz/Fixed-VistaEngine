#include "StdAfx.h"
#include "UI_Font.h"
#include "Serialization\Serialization.h"
#include "Serialization\ResourceSelector.h"
#include "Serialization\EnumDescriptor.h"

#include "Game\GameOptions.h"
#include "UI_Render.h"

using namespace FT;

UI_Font::UI_Font()
{
	fontSize_ = 16;
	font_ = 0;
	aaMax_ = 17;
	aaMin_ = 8;
	hinting_ = FontParam::DEFAULT;
#ifndef _FINAL_VERSION_
	inBox = false;
#endif
}

UI_Font::UI_Font(const char* file, int size)
{
	fontFile_ = file;
	fontSize_ = size;
	font_ = 0;
	aaMax_ = 17;
	aaMin_ = 8;
	hinting_ = FontParam::DEFAULT;
#ifndef _FINAL_VERSION_
	inBox = false;
#endif
}

void UI_Font::serialize(Archive& ar)
{
	static ResourceSelector::Options fontOptions("*.ttf", "Resource\\UI\\Fonts");
	ar.serialize(ResourceSelector(fontFile_, fontOptions), "fontFile", "TTF ����");
	ar.serialize(fontSize_, "fontSize", "������ (�������� ��� ������ ������ 768)");

	ar.serialize(hinting_, "hinting", "�������� ��������");

	ar.serialize(aaMin_, "aaMin", "�������� ����������� ���� ������ ��� �����");
	ar.serialize(aaMax_, "aaMax", "�������� ����������� ���� ������ ��� �����");


#ifndef _FINAL_VERSION_
	ar.serialize(inBox, "inBox", "�� ��������� �����");
#endif
}

void UI_Font::releaseFont()
{
	FT::fontManager().releaseFont(font_);
}

bool UI_Font::createFont()
{
	int newSize = round(float(fontSize_ * UI_Render::instance().windowPosition().height()) / 768.f);
	
	newSize = clamp(newSize, 4, 255);
	
	if(font_ && newSize == font_->size())
		return true;
	
	releaseFont();

	FontParam prm;
	prm.hinting = hinting_;

	prm.antialiasing = (newSize >= aaMax_ || newSize <= aaMin_);

#ifndef _FINAL_VERSION_
	prm.inBox = inBox;
#endif

	font_ = fontManager().createFont(fontFile_.c_str(), newSize, &prm);
	xxassert(font_, (XBuffer() < fontFile_.c_str() < ":" <= newSize).c_str());

	return font_ != 0;
}

void UI_LibFont::serialize(Archive& ar)
{
	StringTableBase::serialize(ar);
	ar.serialize(static_cast<UI_Font&>(*this), "font", "����");
}

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(FontParam, HintMode, "FontParam::HintMode")
REGISTER_ENUM_ENCLOSED(FontParam, DEFAULT, "���������")
REGISTER_ENUM_ENCLOSED(FontParam, BYTE_CODE_ONLY, "������ ����������")
REGISTER_ENUM_ENCLOSED(FontParam, AUTO_HINT_ONLY, "������ ��������������")
REGISTER_ENUM_ENCLOSED(FontParam, NO_HINTING, "�����������")
END_ENUM_DESCRIPTOR_ENCLOSED(FontParam, HintMode)