#ifndef __UNIT_ITEM_INVENTORY_H__
#define __UNIT_ITEM_INVENTORY_H__

#include "UnitObjective.h"
//#include "IronBuilding.h"

class AttributeItemInventory : public AttributeBase
{
public:

	/// ���� ����������
	enum EquipmentType {
		/// �� ����������
		EQUIPMENT_NONE,
		/// ������
		EQUIPMENT_WEAPON
	};

	float stayTime;
	float arealRadius;
	float sightRadius;

	int appearanceDelay;
	bool useLifeTime;
	int lifeTime;

	/// ���������� � ��������� ������������, � �������
	Vect2i inventorySize;
	/// ��� ������ ���������
	int inventoryCellType;
	/// ��� �������� � ���������
	UI_Sprite inventorySprite;
	/// �������� �� �����������
	EquipmentType equipmentType;
	/// ���� ����������
	int equipmentSlot;

	/// ��� ������ (��� ����������-������)
	WeaponPrmReference weaponReference;
    
	AttributeItemInventory();
	void serialize(Archive& ar);
	bool isEquipment() const { return (equipmentType != EQUIPMENT_NONE); }
};


class UnitItemInventory : public UnitObjective
{
public:
	const AttributeItemInventory& attr() const { return safe_cast_ref<const AttributeItemInventory&>(UnitReal::attr()); }

	UnitItemInventory(const UnitTemplate& data);

	void Quant();
	void executeCommand(const UnitCommand& command) {}
	void collision(UnitBase* p, const ContactInfo& contactInfo);
	float parametersSum() const { return parametersSum_; }

private:
		
	float parametersSum_;
	DelayTimer appearanceTimer_;
	DelayTimer killTimer_;
};

#endif //__UNIT_ITEM_INVENTORY_H__
