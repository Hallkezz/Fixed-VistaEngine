#include "StdAfx.h"
#include "UnitItemInventory.h"
#include "Player.h"
#include "RangedWrapper.h"

UNIT_LINK_GET(UnitItemInventory)

REGISTER_CLASS(AttributeBase, AttributeItemInventory, "�������-���������")
REGISTER_CLASS(UnitBase, UnitItemInventory, "�������-���������");
REGISTER_CLASS_IN_FACTORY(UnitFactory, UNIT_CLASS_ITEM_INVENTORY, UnitItemInventory)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(AttributeItemInventory, EquipmentType, "���� ����������")
REGISTER_ENUM_ENCLOSED(AttributeItemInventory, EQUIPMENT_NONE, "�� ����������")
REGISTER_ENUM_ENCLOSED(AttributeItemInventory, EQUIPMENT_WEAPON, "������")
END_ENUM_DESCRIPTOR_ENCLOSED(AttributeItemInventory, EquipmentType)

AttributeItemInventory::AttributeItemInventory()
: AttributeBase()
{
	unitClass_ = UNIT_CLASS_ITEM_INVENTORY;
	unitAttackClass = ATTACK_CLASS_LIGHT;
	collisionGroup = COLLISION_GROUP_REAL;
	rigidBodyPrm = RigidBodyPrmReference("Building");

	stayTime = 5;
	arealRadius = 100;
	sightRadius = 300;

	appearanceDelay = 0;
	useLifeTime = false;
	lifeTime = 60;

	inventorySize = Vect2i(1, 1);
	inventoryCellType = 0;

	equipmentType = EQUIPMENT_NONE;
	equipmentSlot = 0;
}

void AttributeItemInventory::serialize(Archive& ar)
{
    __super::serialize(ar);

	collisionGroup = COLLISION_GROUP_REAL;
	unitClass_ = UNIT_CLASS_ITEM_INVENTORY;
	excludeCollision = unitClass() == UNIT_CLASS_ITEM_INVENTORY ? EXCLUDE_COLLISION_BULLET : 0;

	ar.serialize(parametersInitial, "parametersInitial", "����������� ��������� (�����������)");
	ar.serialize(parametersArithmetics, "parametersArithmetics", "���������� ��� ���������");

	ar.serialize(inventorySize, "inventorySize", "���������� ������������");
	ar.serialize(inventoryCellType, "inventoryCellType", "��� ������");
	ar.serialize(inventorySprite, "inventorySprite", "��������");
	ar.serialize(equipmentType, "equipmentType", "��� ����������");

	if(isEquipment()){
		ar.serialize(equipmentSlot, "equipmentSlot", "���� ����������");

		switch(equipmentType){
		case EQUIPMENT_WEAPON:
			ar.serialize(weaponReference, "weaponReference", "������");
			break;
		}
	}

	ar.serialize(appearanceDelay, "appearanceDelay", "�������� ���������");
	ar.serialize(useLifeTime, "useLifeTime", "������������ ����� �����");
	if(useLifeTime)
		ar.serialize(lifeTime, "lifeTime", "����� �����");

	ar.serialize(mass, "mass", "�����");

}

UnitItemInventory::UnitItemInventory(const UnitTemplate& data) 
: UnitObjective(data)
{ 
	if(attr().appearanceDelay){
		appearanceTimer_.start(attr().appearanceDelay*1000);
		setCollisionGroup(0);
		streamLogicCommand.set(fCommandSetIgnored, model()) << true;
		stopPermanentEffects();
	}
	if(attr().useLifeTime)
		killTimer_.start((attr().appearanceDelay + attr().lifeTime)*1000);
	
	posibleStates_ = UnitItemPosibleStates::instance();
}

void UnitItemInventory::Quant()
{
	start_timer_auto();

	__super::Quant();

	if(abnormalStateType())
		parameters_.set(0.001f);

	if(appearanceTimer_()){
		appearanceTimer_.stop();
		setCollisionGroup(attr().collisionGroup);
		streamLogicCommand.set(fCommandSetIgnored, model()) << false;
		startPermanentEffects();
	}

	if(killTimer_())
		Kill();
	
}

void UnitItemInventory::collision(UnitBase* p, const ContactInfo& contactInfo)
{
	__super::collision(p, contactInfo);

}
