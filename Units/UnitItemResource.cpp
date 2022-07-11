#include "StdAfx.h"
#include "UnitItemResource.h"
#include "Player.h"
#include "Universe.h"
#include "RangedWrapper.h"

REGISTER_CLASS(AttributeBase, AttributeItemResource, "�������-������")
REGISTER_CLASS(UnitBase, UnitItemResource, "�������-������");
REGISTER_CLASS_IN_FACTORY(UnitFactory, UNIT_CLASS_ITEM_RESOURCE, UnitItemResource)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(AttributeItemResource, ItemType, "���� ���������-��������")
REGISTER_ENUM_ENCLOSED(AttributeItemResource,ITEM_DEFAULT, "������� �������")
REGISTER_ENUM_ENCLOSED(AttributeItemResource,ITEM_TREE, "������")
REGISTER_ENUM_ENCLOSED(AttributeItemResource,ITEM_FENCE, "�����")
REGISTER_ENUM_ENCLOSED(AttributeItemResource,ITEM_BUSH, "����")
REGISTER_ENUM_ENCLOSED(AttributeItemResource,ITEM_STONE, "������")
END_ENUM_DESCRIPTOR_ENCLOSED(AttributeItemResource,ItemType)


AttributeItemResource::AttributeItemResource()
: AttributeBase()
{
	unitClass_ = UNIT_CLASS_ITEM_RESOURCE;
	unitAttackClass = ATTACK_CLASS_LIGHT;
	collisionGroup = COLLISION_GROUP_REAL;
	rigidBodyPrm = RigidBodyPrmReference("Building");

	itemType = ITEM_DEFAULT;

	appearanceDelay = 0;
	useLifeTime = false;
	lifeTime = 60;

	enableHiding = false;
//	enablePathFind = false;

	barrierUpgrade = false;
}

void AttributeItemResource::serialize(Archive& ar)
{
    __super::serialize(ar);

	collisionGroup = COLLISION_GROUP_REAL;
	excludeCollision = unitClass() == UNIT_CLASS_ITEM_RESOURCE ? EXCLUDE_COLLISION_BULLET : 0;

	ar.serialize(itemType, "itemType", "��� ��������");
	ar.serialize(barrierUpgrade, "barrierUpgrade", "������ ��� ��������");
	unitClass_ = UNIT_CLASS_ITEM_RESOURCE;

	switch (itemType) {
	case ITEM_DEFAULT: 
		ar.serialize(enablePathTracking, "enablePathTracking", "�������� ������ ������");
//		ar.serialize(enablePathFind, "enablePathFinding", "��������� � ������ ����");
		rigidBodyPrm = RigidBodyPrmReference("Item");
		break;
	case ITEM_TREE: 
		ar.serialize(enablePathTracking, "enablePathTracking", "�������� ������ ������");
		rigidBodyPrm = RigidBodyPrmReference("Environment Tree");
		break;
	case ITEM_FENCE: 
		ar.serialize(enablePathTracking, "enablePathTracking", "�������� ������ ������");
		rigidBodyPrm = RigidBodyPrmReference("Environment Fence");
		break;
	case ITEM_STONE: 
		ar.serialize(enablePathTracking, "enablePathTracking", "�������� ������ ������");
		rigidBodyPrm = RigidBodyPrmReference("Environment Stone");
		break;
	case ITEM_BUSH: 
		enablePathTracking = false;
		rigidBodyPrm = RigidBodyPrmReference("Building");
		collisionGroup = 0;
		break;
	}

	if(itemType == ITEM_DEFAULT){
		ar.serialize(enableHiding, "enableHiding", "�������� ��� �������");
		ar.serialize(producedPlacementZone, "producedPlacementZone", "����������� ����");
		ar.serialize(producedPlacementZoneRadius, "producedPlacementZoneRadius", "������ ����������� ����");
		if(ar.isInput() && producedPlacementZoneRadiusMax_ < producedPlacementZoneRadius)
			producedPlacementZoneRadiusMax_ = producedPlacementZoneRadius;

	}

	ar.serialize(appearanceDelay, "appearanceDelay", "�������� ���������");
	ar.serialize(useLifeTime, "useLifeTime", "������������ ����� �����");
	if(useLifeTime)
		ar.serialize(lifeTime, "lifeTime", "����� �����");

	ar.serialize(mass, "mass", "�����");

}

bool AttributeItemResource::isChainNecessary(ChainID chainID) const 
{
	switch(chainID){
		case CHAIN_MOVE:
			return itemType == ITEM_BUSH;

		default:
			return true;
	}
}

UnitItemResource::UnitItemResource(const UnitTemplate& data) 
: UnitObjective(data)
{ 
//	parameters_.set(0);
	attr().parametersArithmetics.reserve(parameters_);
	attr().parametersArithmetics.apply(parameters_);
	parametersMax_ = parameters_;
	parametersSum_ = parameters().sum();
	if(attr().appearanceDelay){
		appearanceTimer_.start(attr().appearanceDelay*1000);
		setCollisionGroup(0);
		hide(HIDE_BY_INITIAL_APPEARANCE, true);
	}
	if(attr().useLifeTime)
		killTimer_.start((attr().appearanceDelay + attr().lifeTime)*1000);
	
	posibleStates_ = UnitItemPosibleStates::instance();
}

void UnitItemResource::Quant()
{
	start_timer_auto();

	__super::Quant();

	if(abnormalStateType())
		parameters_.set(0.001f);

	if(appearanceTimer_()){
		appearanceTimer_.stop();
		setCollisionGroup(attr().collisionGroup);
		hide(HIDE_BY_INITIAL_APPEARANCE, false);
	}

	if(killTimer_())
		Kill();
	
	if(attr().enableHiding)
		hideCheck();
}

void UnitItemResource::collision(UnitBase* p, const ContactInfo& contactInfo)
{
	__super::collision(p, contactInfo);

	if(attr().itemType == AttributeItemResource::ITEM_BUSH && (p->attr().environmentDestruction & ENVIRONMENT_BUSH))
		setChain(CHAIN_MOVE);
}

void UnitItemResource::hideCheck()
{
	ItemHideScaner hideScaner(attr().producedPlacementZone);
	universe()->unitGrid.Scan(position().xi(), position().yi(), round(attr().producedPlacementZoneRadius), hideScaner);
	if(hideScaner.needHide()){
		setCollisionGroup(0);
		hide(HIDE_BY_PLACED_BUILDING, true);
	}else{
		setCollisionGroup(attr().collisionGroup);
		hide(HIDE_BY_PLACED_BUILDING, false);
	}

}
