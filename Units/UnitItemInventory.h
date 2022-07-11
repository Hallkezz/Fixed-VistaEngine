#ifndef __UNIT_ITEM_INVENTORY_H__
#define __UNIT_ITEM_INVENTORY_H__

#include "UnitObjective.h"
#include "UserInterface\UI_Types.h"

class AttributeItemInventory : public AttributeBase
{
public:

	/// ���� ����������
	enum EquipmentType {
		/// �� ����������
		EQUIPMENT_NONE,
		/// ����������
		EQUIPMENT_GENERAL,
		/// ������
		EQUIPMENT_WEAPON,
		/// ����������
		EQUIPMENT_AMMO,
		
		EQUIPMENT_HEALTH
	};

	float stayTime;
	float arealRadius;
	float sightRadius;

	int appearanceDelay;
	bool useLifeTime;
	int lifeTime;

	/// ������� ������ ���� �� ������� � ���������
	float jumpImpulse;
	float jumpAngularSpeed;

	/// ���������� � ��������� ������������, � �������
	Vect2i inventorySize;
	/// ��� ������ ���������
	InventoryCellType inventoryCellType;
	/// ��� �������� � ���������
	UI_Sprite inventorySprite;
	/// ��� �������� � ��������� �������� �������
	UI_Sprite quickAccessSprite;
	/// �������� �� �����������
	EquipmentType equipmentType;
	/// ���� ����������
	EquipmentSlotType equipmentSlotType;
	/// ���� �������� �������
	QuickAccessSlotType quickAccessSlotType;

	/// ��� ������ (��� ����������-������)
	WeaponPrmReference weaponReference;
	/// ��� ����������� (��� ����������-�����������)
	WeaponAmmoTypeReference ammoTypeReference;
    
	AttributeItemInventory();
	void serialize(Archive& ar);
	bool isEquipment() const { return (equipmentType != EQUIPMENT_NONE); }
	int getInventoryCellType(UI_InventoryType inventory_type = UI_INVENTORY) const;

	/// ����� �������� �� �����
	LocString tipText;
};


class UnitItemInventory : public UnitObjective
{
public:
	const AttributeItemInventory& attr() const { return safe_cast_ref<const AttributeItemInventory&>(UnitReal::attr()); }

	UnitItemInventory(const UnitTemplate& data);

	void Quant();
	void graphQuant(float dt);

	void executeCommand(const UnitCommand& command) {}
	float parametersSum() const { return parametersSum_; }

	/// ������������� ����� �� ������� � ���������.
	void jump();

	void setPose(const Se3f& poseIn, bool initPose);

private:
		
	float parametersSum_;
	LogicTimer appearanceTimer_;
	LogicTimer killTimer_;
};

#endif //__UNIT_ITEM_INVENTORY_H__
