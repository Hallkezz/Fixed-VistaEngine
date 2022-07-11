#ifndef __UI_INVENTORY_H__
#define __UI_INVENTORY_H__

#include "UI_Types.h"
#include "UI_References.h"

class Archive;
class Inventory;
class InventoryItem;
class UI_InventoryLogicData;

/// ����������� ������������� �������� � ���������
class UI_InventoryItem
{
public:
	UI_InventoryItem();
	explicit UI_InventoryItem(const InventoryItem& item);
	UI_InventoryItem(const Recti& position, const UI_Sprite& sprite) : position_(position),
		sprite_(&sprite) { }

	const Recti& position() const { return position_; }
	void setPosition(const Recti& position){ position_ = position; }

	const UI_Sprite* sprite() const { return sprite_; }
	void setSprite(const UI_Sprite& sprite){ sprite_ = &sprite; }

	void redraw(const Rectf& position, float alpha) const;

private:

	/// ���������� � ��������� ������������
	Recti position_;

	/// ������� ��� ��������
	const UI_Sprite* sprite_;
};

typedef std::vector<UI_InventoryItem> UI_InventoryItems;

/// ���������, ����������� �������������.
class UI_ControlInventory : public UI_ControlBase
{
public:
	UI_ControlInventory();
	~UI_ControlInventory();

	bool redraw() const;

	const Vect2i& size() const { return size_; }
	int cellType() const { return cellType_; }

	bool isEquipment() const { return isEquipment_; }

	/// ��������, ������ �� ������� �� ��������� �����������
	/// ������� � ��������� �������� �� �����������, ��������� ������ �� �������
	bool checkPosition(const Vect2i& pos, const Vect2i& size) const;
	Vect2i adjustPosition(const Vect2i& pos, const Vect2i& size) const;

	bool updateItems(const UI_InventoryItem* item_ptr, int item_count);

	void serialize(Archive& ar);

protected:

	bool inputEventHandler(const UI_InputEvent& event);

private:

	/// ���������� �����
	Vect2i size_;

	/// ��� �����
	/** 
	�� ����� ���� �����������, ����� �� ������
	���� ������� � ������
	*/
	int cellType_;

	/// ��������� - ����������
	bool isEquipment_;

	/// ��� ������ � ������ ���������
	UI_Sprite cellEmptySprite_;
	/// ��� ����������� ������
	UI_Sprite cellSprite_;

	UI_InventoryItems items_;

	/// ���������� true, ���� � ������ \a cell_index ������ �� �����
	bool isCellEmpty(const Vect2i& cell_index) const;

	void redrawCell(const Rectf& position, bool empty_state) const;
	void redrawItem(const UI_InventoryItem& item) const;

	/// �������� ��������� �� ����� � ��������
	Rectf screenCoords(const Recti& index_coords) const;
	/// �������� ��������� �� ����� � ��������
	Vect2i screenCoords(const Vect2i& index_coords) const;

	/// �������� �������� ��������� � ������
	/// ���������� false, ���� ����� �� �������� � �����
	bool getCellCoords(const Vect2f& screen_coords, Vect2i& index_coords) const;

	/// ������ ���������, ������� � ���������
	/// ������ �� ����������� ���������� ����������
	const UI_InventoryItems* items() const { return &items_; }

	Vect2i itemSize(const UI_InventoryItem& item) const;
	Recti itemPosition(const UI_InventoryItem& item) const;
};

typedef UI_ControlReferenceRefined<UI_ControlInventory> UI_InventoryReference;
typedef std::list<UI_InventoryReference> UI_InventoryReferences;

#endif /* __UI_INVENTORY_H__ */

