
#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "Parameters.h"
#include "XTL\SwapVector.h"

class UnitItemInventory;
class UnitActing;

class AttributeItemInventory;
class UI_ControlInventory;
class InventorySet;

struct WeaponAmmoType;

struct InventoryCellTypeString : StringTableBaseSimple
{
	InventoryCellTypeString(const char* name = "") : StringTableBaseSimple(name) {}
};

typedef StringTable<InventoryCellTypeString> InventoryCellTypeTable;
typedef StringTableReference<InventoryCellTypeString, true> InventoryCellType;

struct EquipmentSlotTypeString : StringTableBaseSimple
{
	EquipmentSlotTypeString(const char* name = "") : StringTableBaseSimple(name) {}
};

typedef StringTable<EquipmentSlotTypeString> EquipmentSlotTypeTable;
typedef StringTableReference<EquipmentSlotTypeString, true> EquipmentSlotType;

struct QuickAccessSlotTypeString : StringTableBaseSimple
{
	QuickAccessSlotTypeString(const char* name = "") : StringTableBaseSimple(name) {}
};

typedef StringTable<QuickAccessSlotTypeString> QuickAccessSlotTypeTable;
typedef StringTableReference<QuickAccessSlotTypeString, true> QuickAccessSlotType;

class InventoryPosition
{
public:
	InventoryPosition(const Vect2i& pos = Vect2i(0,0), int index = -1) : cellIndex_(pos), inventoryIndex_(index){ }
	bool operator == (const InventoryPosition& pos) const { return cellIndex_ == pos.cellIndex_ && inventoryIndex_ == pos.inventoryIndex_; }

	const Vect2i& cellIndex() const { return cellIndex_; }
	void setCellIndex(const Vect2i& pos){ cellIndex_ = pos; }

	int inventoryIndex() const { return inventoryIndex_; }
	void setInventoryIndex(int index){ inventoryIndex_ = index; }

	void serialize(Archive& ar);

private:
	Vect2i cellIndex_;
	int inventoryIndex_;
};

/// ������� � ���������
class InventoryItem
{
public:
	InventoryItem();
	InventoryItem(const UnitItemInventory* p);
	InventoryItem(const AttributeItemInventory& attr);

	~InventoryItem();

	bool operator == (const InventoryItem& item) const
	{
		return (position_ == item.position_ && attribute_ == item.attribute_);
	}

	bool operator ! () const { return !attribute_; }
	bool operator () () const { return (attribute_ != 0); }

	void clear();

	const InventoryPosition& position() const { return position_; }
	void setPosition(const InventoryPosition& pos){ position_ = pos; }

	const Vect2i& cellIndex() const { return position_.cellIndex(); }

	int inventoryIndex() const { return position_.inventoryIndex(); }
	void setInventoryIndex(int index){ position_.setInventoryIndex(index); }

	const Vect2i& size() const;
	const UI_Sprite& sprite(UI_InventoryType inventory_type = UI_INVENTORY) const;

	const ParameterSet& parameters() const { return parameters_; }
	void setParameters(const ParameterSet& parameters){ parameters_ = parameters; }
	void setParameter(float value, ParameterType::Type type){ parameters_.set(value, type); }
	const ParameterArithmetics& arithmetics() const { return arithmetics_; }
	const AttributeItemInventory* attribute() const;

	void clone(InventoryItem& item) const;
	bool isClone() const { return isClone_; }
	bool isClone(const InventoryItem& clone) const;
	const InventoryPosition& parentPosition() const { return parentPosition_; }
	void setParentPosition(const InventoryPosition& pos){ parentPosition_ = pos; }

	bool isActivated() const { return isActivated_; }
	void setActivated(bool state){ isActivated_ = state; }

	/// ������������-"����"
	void serialize(Archive& ar);

	void showDebugInfo() const;

private:

	/// ���������� �������� � �������
	InventoryPosition position_;

	InventoryPosition parentPosition_;
	bool isClone_;

	bool isActivated_;

	/// ����������, ������� ���� ��������� ��� ������ �������� �� ����������
	ParameterArithmetics arithmetics_;

	ParameterSet parameters_;
	AttributeItemInventoryReference attribute_;
};

typedef SwapVector<InventoryItem> InventoryItems;

class Inventory
{
public:
	Inventory(UI_ControlInventory* control = 0);
	~Inventory();

	void setOwner(InventorySet* owner){ owner_ = owner; }

	UI_ControlInventory* control() const { return control_; }

	int index() const { return index_; }
	void setIndex(int idx){ index_ = idx; }

	int weaponSlotIndex() const { return weaponSlotIndex_; }
	void setWeaponSlotIndex(int idx){ weaponSlotIndex_ = idx; }

	bool checkCellType(const UnitItemInventory* item) const;
	bool checkCellType(const InventoryItem& item) const;

	bool canPutItem(const UnitItemInventory* item) const;
	bool canPutItem(const UnitItemInventory* item, const Vect2i& position) const;
	bool canPutItem(const InventoryItem& item) const;
	bool canPutItem(const InventoryItem& item, const Vect2i& position) const;

	bool putItem(const UnitItemInventory* item);
	bool putItem(const UnitItemInventory* item, const Vect2i& position);
	bool putItem(const InventoryItem& item);
	bool putItem(const InventoryItem& item, const Vect2i& position);

	bool removeItem(const InventoryItem* item);
	bool removeItem(const Vect2i& position);

	bool removeClonedItems(const InventoryItem* item);

	const InventoryItems& items() const { return items_; }
	const InventoryItem* getItem(const Vect2i& cell_index) const;
	InventoryItem* getItem(const Vect2i& cell_index);
	void clearItems(){ items_.clear(); }

	bool isCellEmpty(const Vect2i& cell_index) const;

	bool isEquipment() const;
	bool isQuickAccess() const;
	UI_InventoryType inventoryType() const;

	int cellType() const; 
	/// ������ ��������� � �������
	Vect2i size() const;

	void updateControl() const;

	/// ������������-"����"
	void serialize(Archive& ar);

private:

	InventoryItems items_;

	InventorySet* owner_;
	mutable UI_ControlInventory* control_;

	int index_;

	/// ����� ���������� �����, ��� ����������
	int weaponSlotIndex_;

	bool checkItemPosition(const Vect2i& pos, const InventoryItem& item) const;
	bool checkItemPosition(const Vect2i& pos, const Vect2i& size) const;
	Vect2i adjustItemPosition(const Vect2i& pos, const Vect2i& size) const;

	Vect2i itemSize(const UnitItemInventory* item) const;
	Vect2i itemSize(const InventoryItem& item) const;

	bool onPutItem(InventoryItem& item);
	bool onRemoveItem(InventoryItem& item);
};

typedef SwapVector<Inventory> Inventories;

/// ��������� ���������
class InventorySet
{
public:
	InventorySet();

	void setOwner(UnitActing* unit);

	void reserve(int size){ inventories_.reserve(size);	}

	bool add(UI_ControlInventory* control);

	const InventoryItem* getItem(int position) const;
	/// ���������� true, ���� ������ � ������������ \a position ������.
	bool isCellEmpty(int position) const;

	/// ��������, ������ �� ������� � ���������.
	bool canPutItem(const UnitItemInventory* item, int position = -1) const;
	bool canPutItem(const InventoryItem& item, int position = -1) const;
	/// ����� ������� � ���������.
	/// ���� �� ����, ���������� false.
	bool putItem(const UnitItemInventory* item, int position = -1);
	/// ����� ������� � ���������.
	/// ���� �� ����, ���������� false.
	bool putItem(const InventoryItem& item, int position = -1);
	bool putItem(const InventoryItem& item, UI_InventoryType inventory_type);

	/// ������� �������, ������� � ������ ����� \a position
	bool removeItem(int position);
	bool removeItem(const InventoryPosition& pos);

	bool onPutItem(InventoryItem& item, const Inventory& inventory);
	bool onRemoveItem(InventoryItem& item, const Inventory& inventory);

	bool reloadWeapon(int slot_index);
	bool removeWeapon(int slot_index);
	bool updateWeapon(int slot_index);

	/// ������ �������, ������� � ������ ����� \a position
	bool takeItem(int position);
	/// ������� ������� ������� � ������ ����� \a position
	/** ���� ������ �� �������, �� ���������� �������� � ������,
	 ����� ������� ����, ���� �� ���������, �� ������� ���� ������.
	*/
	bool returnItem(int position = -1);
	bool removeTakenItem(){ takenItem_.clear(); return true; }

	void dropItems();

	const InventoryItem& takenItem() const { return takenItem_; }

	const Inventories& getList() const { return inventories_; }

	/// ���������� ������ ������
	int getPosition(const UI_ControlInventory* control, const Vect2i& pos) const;

	void updateUI() const;

	/// ������������-"����"
	void serialize(Archive& ar);

	static UI_QuickAccessMode quickAccessMode(){ return quickAccessMode_; }
	static void setQuickAccessMode(UI_QuickAccessMode mode){ quickAccessMode_ = mode; }

private:

	Inventories inventories_;

	InventoryItem takenItem_;
	int takenItemPosition_;

	UnitActing* owner_;

	/// ���������� ���������� ������ �� �������
	bool parsePosition(int position, InventoryPosition& out_position) const;

	bool removeClonedItems(const InventoryItem* item);
	bool putToEquipmentSlot(const InventoryItem& item);
	InventoryItem* getItem(const InventoryPosition& pos);

	/// ����� ����������� �� ���������, ���������� ������� ���������������� �������� ��� -1
	bool findWeaponAmmo(const WeaponAmmoType* ammo_type, InventoryPosition& pos);

	static UI_QuickAccessMode quickAccessMode_;

	void clearItems();
};

#endif /* __INVENTORY_H__ */
