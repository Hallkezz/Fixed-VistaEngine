#include "StdAfx.h"
#include "UI_GlobalAttributes.h"
#include "UI_Render.h"
#include "CommonLocText.h"

WRAP_LIBRARY(UI_GlobalAttributes, "UI_GlobalAttributes", "UI_GlobalAttributes", "Scripts\\Content\\UI_GlobalAttributes", 0, 0);

UI_GlobalAttributes::UI_GlobalAttributes()
{
	chatDelay_ = 5.f;
	privateMessage_ = Color4f(0, 1, 0, 1); 
	systemMessage_ = Color4f(0, 1, 0, 1);
}

const UI_Cursor* UI_GlobalAttributes::getMoveCursor(int dir){
	switch(dir){
		case 1:
			return cursor(UI_CURSOR_SCROLL_UP);
		case 2:
			return cursor(UI_CURSOR_SCROLL_LEFT);
		case 3:
			return cursor(UI_CURSOR_SCROLL_UPLEFT);
		case 4:
			return cursor(UI_CURSOR_SCROLL_RIGHT);
		case 5:
			return cursor(UI_CURSOR_SCROLL_UPRIGHT);
		case 8:
			return cursor(UI_CURSOR_SCROLL_BOTTOM);
		case 10:
			return cursor(UI_CURSOR_SCROLL_BOTTOMLEFT);
		case 12:
			return cursor(UI_CURSOR_SCROLL_BOTTOMRIGHT);
		default:
			return 0;
	}
}

void UI_GlobalAttributes::serialize(Archive& ar)
{
	ar.serialize(UI_Render::instance(), "UI_Render", "UI_Render");

	UI_Task::serializeColors(ar);

	ar.serialize(privateMessage_, "privateMessage", "���� �������/��������� ���������");
	ar.serialize(systemMessage_, "systemMessage", "���� ���������� ���������");
	ar.serialize(chatDelay_, "chatMessageDelay", "����� ����������� �������� ���-��������� (���)");

	ar.serialize(cursors_, "cursors", "�������");

	ar.serialize(messageSetups_, "messageSetups", "���������");

	ar.serialize(hintAttributes_, "hintAttributes", "����������� ��������� ��� ������� ���������");

	if(ar.isEdit())
		ar.serialize(CommonLocText::instance(), "locTexts", "����� ������ ��� �����������");
}
