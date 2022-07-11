#ifndef __UI_INVENTORY_H__
#define __UI_INVENTORY_H__

#include "UI_Types.h"
#include "UI_Sprite.h"
#include "UI_References.h"

#include "Inventory.h"

class Archive;
class UI_InventoryLogicData;

/// ����������� ������������� �������� � ���������
class UI_InventoryItem
{
public:
	UI_InventoryItem();
	UI_InventoryItem(const InventoryItem& item, UI_InventoryType inventory_type);
	UI_InventoryItem(const Recti& position, const UI_Sprite& sprite) : position_(position),
		sprite_(&sprite), isActivated_(false) { }

	const Recti& position() const { return position_; }
	void setPosition(const Recti& position){ position_ = position; }

	const UI_Sprite* sprite() const { return sprite_; }
	void setSprite(const UI_Sprite& sprite){ sprite_ = &sprite; }

	bool isActivated() const { return isActivated_; }

	void redraw(const Vect2f& position, float alpha) const;
	void redraw(const Rectf& position, float alpha) const;

private:

	bool isActivated_;

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

	void quant(float dt);
	bool redraw() const;

	const Vect2i& size() const { return size_; }
	int cellType() const;

	bool isEquipment() const { return inventoryType_ == UI_INVENTORY_EQUIPMENT; }
	bool isQuickAccess() const { return inventoryType_ == UI_INVENTORY_QUICK_ACCESS; }
	UI_InventoryType inventoryType() const { return inventoryType_; }

	/// ��������, ������ �� ������� �� ��������� �����������
	/// ������� � ��������� �������� �� �����������, ��������� ������ �� �������
	bool checkPosition(const Vect2i& pos, const Vect2i& size) const;
	Vect2i adjustPosition(const Vect2i& pos, const Vect2i& size) const;

	bool updateItems(const UI_InventoryItem* item_ptr, int item_count);

	const InventoryItem* hoverItem(const InventorySet* inv, const Vect2f& screen_pos) const;

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
	InventoryCellType cellType_;

	UI_InventoryType inventoryType_;
	/// ��� ����� (��� ����������)
	EquipmentSlotType equipmentSlotType_;
	/// ��� ����� (��� ���������� �������� �������)
	QuickAccessSlotType quickAccessSlotType_;

	/// ��� ������ � ������ ���������
	UI_Sprite cellEmptySprite_;
	/// ��� ����������� ������
	UI_Sprite cellSprite_;
	/// ��� �������������� ������ (��� ��������� �������� �������)
	UI_Sprite cellActiveSprite_;

	/// ������������ ������� (���������� ���� ����� ������� � ����)
	Recti highlight_;

	UI_InventoryItems items_;

	enum CellState {
		CELL_EMPTY,
		CELL_NOT_EMPTY,
		CELL_HIGHLITED,
		CELL_ACTIVATED
	};

	CellState cellState(const Vect2i& cell_index) const;

	void redrawCell(const Rectf& position, CellState state = CELL_EMPTY) const;
	void redrawItem(const UI_InventoryItem& item) const;

	/// �������� ��������� �� ����� � ��������
	Rectf screenCoords(const Recti& index_coords) const;
	Vect2f screenCoords(const Vect2i& index_coords) const;

	/// �������� �������� ��������� � ������
	/// ���������� false, ���� ����� �� �������� � �����
	bool getCellCoords(const Vect2f& screen_coords, Vect2i& index_coords) const;

	/// ������ ���������, ������� � ���������
	/// ������ �� ����������� ���������� ����������
	const UI_InventoryItems* items() const { return &items_; }

	Vect2i itemSize(const UI_InventoryItem& item) const;
	Recti itemPosition(const UI_InventoryItem& item) const;
	Vect2i itemPosition(const Vect2i& index_coords) const;

	bool canPutItem(const Vect2i& pos, const Vect2i& size) const;
};

typedef UI_ControlReferenceRefined<UI_ControlInventory> UI_InventoryReference;
typedef vector<UI_InventoryReference> UI_InventoryReferences;

#endif /* __UI_INVENTORY_H__ */

