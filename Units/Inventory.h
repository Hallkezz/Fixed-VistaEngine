
#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "Parameters.h"
#include "SwapVector.h"

class UnitItemInventory;
class UnitActing;

class AttributeItemInventory;
class UI_ControlInventory;
class InventorySet;

/// ������� � ���������
class InventoryItem
{
public:
	InventoryItem();
	InventoryItem(const UnitItemInventory* p);

	~InventoryItem();

	bool operator == (const InventoryItem& item) const
	{
		return (position_ == item.position_ && attribute_ == item.attribute_);
	}

	bool operator ! () const { return !attribute_; }
	bool operator () () const { return (attribute_ != 0); }

	void clear();

	const Vect2i& position() const { return position_; }
	void setPosition(const Vect2i& pos){ position_ = pos; }

	int inventoryIndex() const { return inventoryIndex_; }
	void setInventoryIndex(int index){ inventoryIndex_ = index; }

	const Vect2i& size() const;
	const UI_Sprite& sprite() const;

	const ParameterSet& parameters() const { return parameters_; }
	const AttributeItemInventory* attribute() const;

	/// ������������-"����"
	void serialize(Archive& ar);

private:

	/// ���������� �������� � �������
	Vect2i position_;
	int inventoryIndex_;

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

	bool checkCellType(const UnitItemInventory* item) const;
	bool checkCellType(const InventoryItem& item) const;

	bool canPutItem(const UnitItemInventory* item) const;
	bool canPutItem(const UnitItemInventory* item, const Vect2i& position) const;

	bool putItem(const UnitItemInventory* item);
	bool putItem(const UnitItemInventory* item, const Vect2i& position);
	bool putItem(const InventoryItem& item);
	bool putItem(const InventoryItem& item, const Vect2i& position);

	bool removeItem(const InventoryItem* item);
	bool removeItem(const Vect2i& position);

	const InventoryItems& items() const { return items_; }
	const InventoryItem* getItem(const Vect2i& cell_index) const;
	void clearItems(){ items_.clear(); }

	bool isCellEmpty(const Vect2i& cell_index) const;

	bool isEquipment() const;

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

	bool checkItemPosition(const Vect2i& pos, const InventoryItem& item) const;
	bool checkItemPosition(const Vect2i& pos, const Vect2i& size) const;
	Vect2i adjustItemPosition(const Vect2i& pos, const Vect2i& size) const;

	Vect2i itemSize(const UnitItemInventory* item) const;
	Vect2i itemSize(const InventoryItem& item) const;

	bool onPutItem(const InventoryItem& item);
	bool onRemoveItem(const InventoryItem& item);
};

typedef SwapVector<Inventory> Inventories;

/// ��������� ���������
class InventorySet
{
public:
	InventorySet();

	void setOwner(UnitActing* unit){ owner_ = unit; }

	void reserve(int size){ inventories_.reserve(size);	}

	bool add(UI_ControlInventory* control);

	const InventoryItem* getItem(int position) const;
	/// ���������� true, ���� ������ � ������������ \a position ������.
	bool isCellEmpty(int position) const;

	/// ��������, ������ �� ������� � ���������.
	bool canPutItem(const UnitItemInventory* item, int position = -1) const;
	/// ����� ������� � ���������.
	/// ���� �� ����, ���������� false.
	bool putItem(const UnitItemInventory* item, int position = -1);
	/// ����� ������� � ���������.
	/// ���� �� ����, ���������� false.
	bool putItem(const InventoryItem& item, int position = -1);

	/// ������� �������, ������� � ������ ����� \a position
	bool removeItem(int position);

	bool onPutItem(const InventoryItem& item, const Inventory& inventory);
	bool onRemoveItem(const InventoryItem& item, const Inventory& inventory);

	/// ������ �������, ������� � ������ ����� \a position
	bool takeItem(int position);
	/// ������� ������� ������� � ������ ����� \a position
	/** ���� ������ �� �������, �� ���������� �������� � ������,
	 ����� ������� ����, ���� �� ���������, �� ������� ���� ������.
	*/
	bool returnItem(int position = -1);
	bool removeTakenItem(){ takenItem_.clear(); return true; }

	const InventoryItem& takenItem() const { return takenItem_; }

	const Inventories& getList() const { return inventories_; }

	/// ���������� ������ ������
	int getPosition(const UI_ControlInventory* control, const Vect2i& pos) const;

	void updateUI() const;

	/// ������������-"����"
	void serialize(Archive& ar);

private:

	Inventories inventories_;

	InventoryItem takenItem_;
	int takenItemPosition_;

	UnitActing* owner_;

	/// ���������� ���������� ������ �� �������
	bool parsePosition(int position, int& inventory_index, Vect2i& cell_index) const;

	void clearItems();
};

#endif /* __INVENTORY_H__ */
