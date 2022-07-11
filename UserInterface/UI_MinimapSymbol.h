#ifndef __UI_MINIMAP_SYMBOL_H__
#define __UI_MINIMAP_SYMBOL_H__

#include "UI_Sprite.h"

/// �������� ������� ��� ����������� ������-���� ������� �� ���������
class UI_MinimapSymbol
{
public:
	UI_MinimapSymbol();

	void serialize(Archive& ar);

	enum SymbolType {
		/// �������������
		SYMBOL_RECTANGLE,
		/// ������
		SYMBOL_SPRITE
	};

	bool operator == (const UI_MinimapSymbol& obj) const { return type == obj.type && scale == obj.scale
		&& legionColor == obj.legionColor && (!legionColor || color == obj.color)
		&& (type == SYMBOL_RECTANGLE || sprite == obj.sprite);
	}

	/// �������� ����� ����� ������� �� ���������
	int lifeTime() const;
	
public:

	/// ��� �����������
	SymbolType type;
	/// ����������� �������
	float scale;
	/// ������������� �� �������/�������
	bool scaleByEvent;
	/// ������������ ���� ������� ��� ���������
	bool legionColor;
	/// ����������� ���� �������
	Color4f color;
	/// ������ ��� ��������� �� ���������
	UI_Sprite sprite;
};


/// �������� ������� �� ��������� � �������� ����� �������
class UI_MinimapEventStatic : public UI_MinimapSymbol
{
public:
	UI_MinimapEventStatic();
	UI_MinimapEventStatic(const UI_MinimapSymbol& symb);

	void serialize(Archive& ar);

	bool operator == (const UI_MinimapEventStatic& obj) const { return UI_MinimapSymbol::operator ==(obj); }

	float validTime() const { return validTime_; }

private:

	/// ������ �������, ������� ����������� ��� ��������
	bool isImportant_;
	/// ����� �� ������� ����� ��������� �������
	float validTime_;
};

#endif //__UI_MINIMAP_SYMBOL_H__