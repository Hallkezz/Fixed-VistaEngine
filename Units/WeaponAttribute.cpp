#include "stdafx.h"

#include "WeaponAttribute.h"
#include "WeaponPrms.h"
#include "UnitAttribute.h"
#include "XPrmArchive.h"
#include "MultiArchive.h"
#include "TypeLibraryImpl.h"
#include "UnitActing.h"

#include "Player.h"


BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, WeaponClass, "WeaponPrm::WeaponClass")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_BEAM, "������� ������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_PROJECTILE, "���������� ��������� ������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_AREA_EFFECT, "����������� �� ���� ������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_WAITING_SOURCE, "��������� � ���������� ����������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_GRIP, "������ - ������")
//REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_TELEPORT, "������ ������������")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, WeaponClass)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, ShootingOnMoveMode, "WeaponPrm::ShootingOnMoveMode")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_WHILE_STANDING, "����� �����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_WHILE_MOVING, "�� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_WHILE_RUNNING, "�� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_WHILE_IN_TRANSPORT, "� ����������")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, ShootingOnMoveMode)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, ShootingMode, "WeaponPrm::ShootingMode")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_MODE_DEFAULT, "������� �����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_MODE_INTERFACE, "�� ������� �� ����������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_MODE_ALWAYS, "������")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, ShootingMode)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, UnitMode, "WeaponPrm::UnitMode")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_WATER_BOTTOM, "�� ���")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_WATER, "�� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_GROUND, "�� �����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_AIR, "� �������")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, UnitMode)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, TargetUnitMode, "WeaponPrm::TargetUnitMode")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_WATER_BOTTOM, "���� �� ���")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_WATER, "��������� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_GROUND, "�������� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_AIR, "��������� ����")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, TargetUnitMode)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, RangeType, "WeaponPrm::RangeType")
REGISTER_ENUM_ENCLOSED(WeaponPrm, LONG_RANGE, "������ �������� ���")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHORT_RANGE, "������ �������� ���")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ANY_RANGE, "������������� ������")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, RangeType)

BEGIN_ENUM_DESCRIPTOR(AffectMode, "AffectMode")
REGISTER_ENUM(AFFECT_OWN_UNITS, "������ �� ����� ������")
REGISTER_ENUM(AFFECT_FRIENDLY_UNITS, "�� ����� � ������� ������")
REGISTER_ENUM(AFFECT_ALLIED_UNITS, "�� ������� ������")
REGISTER_ENUM(AFFECT_ENEMY_UNITS, "�� ��������� ������")
REGISTER_ENUM(AFFECT_ALL_UNITS, "�� ���� ������")
REGISTER_ENUM(AFFECT_NONE_UNITS, "�� ������������ �� ������")
END_ENUM_DESCRIPTOR(AffectMode)

BEGIN_ENUM_DESCRIPTOR(WeaponDirectControlMode, "WeaponDirectMode")
REGISTER_ENUM(WEAPON_DIRECT_CONTROL_DISABLE, "�� ��������")
REGISTER_ENUM(WEAPON_DIRECT_CONTROL_NORMAL, "�������� �� ������� ����� ������ ����")
REGISTER_ENUM(WEAPON_DIRECT_CONTROL_ALTERNATE, "�������� �� ������� ������ ������ ����")
REGISTER_ENUM(WEAPON_DIRECT_CONTROL_AUTO, "������������� �������� ����")
END_ENUM_DESCRIPTOR(WeaponDirectControlMode)

BEGIN_ENUM_DESCRIPTOR(WeaponSourcesCreationMode, "WeaponSourcesCreationMode")
REGISTER_ENUM(WEAPON_SOURCES_CREATE_ON_TARGET_HIT, "��� ��������� � ����")
REGISTER_ENUM(WEAPON_SOURCES_CREATE_ON_GROUND_HIT, "��� ��������� � �����")
REGISTER_ENUM(WEAPON_SOURCES_CREATE_ALWAYS, "������")
END_ENUM_DESCRIPTOR(WeaponSourcesCreationMode)

BEGIN_ENUM_DESCRIPTOR(WeaponGroupShootingMode, "WeaponGroupShootingMode")
REGISTER_ENUM(WEAPON_GROUP_INDEPENDENT, "����������")
REGISTER_ENUM(WEAPON_GROUP_MODE_PRIORITY, "� ������������ � �����������")
END_ENUM_DESCRIPTOR(WeaponGroupShootingMode)

WRAP_LIBRARY(WeaponGroupTypeTable, "WeaponGroup", "���� ����� ������", "Scripts\\Content\\WeaponGroup", 1, false);

// ------------------------------

WeaponAimAnglePrm::WeaponAimAnglePrm()
{
	precision_ = 1.0f;
	turnSpeed_ = 10.0f;
	turnSpeedDirectControl_ = 2000.0f;

	valueMin_ = 0;
	valueMax_ = 360;
	valueDefault_ = 0;

	rotateByLogic_ = false;
}

void WeaponAimAnglePrm::serialize(Archive& ar)
{
	ar.serialize(nodeGraphics_, "nodeGraphics", "&��� ���� ��� ���������� ���������");
	ar.serialize(nodeLogic_, "nodeLogic", "&��� ����������� ���� ��� ���������� ���������");

	ar.serialize(rotateByLogic_, "rotateByLogic", "������������ ����������� ���� �� ���� �����������");

	ar.serialize(precision_, "precision", "�������� ���������");

	ar.serialize(turnSpeed_, "turnSpeed", "�������� ��������, ������/���");
	ar.serialize(turnSpeedDirectControl_, "turnSpeedDirectControl", "�������� �������� ��� ������ ����������, ������/���");

	ar.serialize(valueMin_, "valueMin", "����������� ��������");
	ar.serialize(valueMax_, "valueMax", "������������ ��������");
	ar.serialize(valueDefault_, "valueDefault", "�������� �� ���������");

	if(valueDefault_ < valueMin_) valueDefault_ = valueMin_;
	if(valueDefault_ > valueMax_) valueDefault_ = valueMax_;
}

WeaponAimControllerPrm::WeaponAimControllerPrm()
{
	isEnabled_ = false;
	hasAnimation_ = false;
	animationChainCounter_ = 0;
	animationChainAimCounter_ = 0;
	angleThetaPrm_.setValueLimits(-90, 90);
}

void WeaponAimControllerPrm::serialize(Archive& ar)
{
	ar.serialize(isEnabled_, "isEnabled", "&��������");

	if(!ar.serialize(barrels_, "barrels", "������")){
		Barrel barrel;
		barrel.serialize(ar);
		barrels_.push_back(barrel);
	}

	ar.serialize(hasAnimation_, "hasAnimation", "���� ��������");
	ar.serialize(animationChainCounter_, "animationChainCounter", "����� ������� �������� ��������");
	ar.serialize(animationChainAimCounter_, "animationChainAimCounter", "����� ������� �������� ��������");

	ar.serialize(anglePsiPrm_, "anglePsiPrm", "�������������� ����");
	ar.serialize(angleThetaPrm_, "angleThetaPrm", "������������ ����");
}

void WeaponAimControllerPrm::Barrel::serialize(Archive& ar)
{
	ar.serialize(nodeLogic_, "nodeLogic", "&����� ������ �������");
	ar.serialize(nodeGraphics_, "nodeGraphics", "&����� ����������� ��������");
}

// ------------------------------

WeaponSlotAttribute::WeaponSlotAttribute()
{
	equipmentCellType_ = 0;
	priority_ = 0;
}

void WeaponSlotAttribute::serialize(Archive& ar)
{
	ar.serialize(aimControllerPrm_, "aimControllerPrm", "���������� ����������");
	ar.serialize(weaponPrmReference_, "weaponPrmReference", "&�������� ������");

	ar.serialize(equipmentCellType_, "equipmentCellType", "��� ������ ����������");
	ar.serialize(groupType_, "groupType", "������ ������");
	ar.serialize(priority_, "priority", "���������");
}

// ------------------------------

void WeaponPrm::Upgrade::serialize(Archive& ar)
{
	ar.serialize(reference_, "reference", "&������");
}

WeaponPrm::WeaponPrm() : fireEffect_(false)
{
	ID_ = -1;

	clearTargets_ = false;
	exclusiveTarget_ = false;
	clearAttackClickMode_ = true;

	disableOwnerMove_ = false;

	fireCostAtOnce_ = false;

	queueFire_ = false;
	queueFireDelay_ = 200;

	weaponClass_ = WEAPON_BEAM;
	attackClass_ = ATTACK_CLASS_IGNORE;

	affectMode_ = AFFECT_ENEMY_UNITS;

	rangeType_ = LONG_RANGE;
	shootingMode_ = SHOOT_MODE_DEFAULT;
	directControlMode_ = WEAPON_DIRECT_CONTROL_NORMAL;

	shootingOnMoveMode_ = SHOOT_WHILE_STANDING | SHOOT_WHILE_MOVING | SHOOT_WHILE_RUNNING;

	isInterrupt_ = true;
	fireDuringClick_ = false;
	canShootThroughShield_ = false;
	canShootUnderFogOfWar_ = true;
	clearTargetOnLoading_ = false;

	targetUnitMode_.resize(UNIT_MODE_COUNT, TARGET_MODE_ALL);

	aimLockTime_ = 0.2f;
	
	visibleTimeOnShoot_ = 0.f;

	fireRadiusCircle_.color.set(255, 255, 255, 0);
	fireRadiusCircle_.width = 4;
	fireRadiusCircle_.dotted = CircleColor::CIRCLE_CHAIN_LINE;

	fireMinRadiusCircle_ = fireRadiusCircle_;
	fireMinRadiusCircle_.width = 2;

	hideCursor_ = false;
	alwaysPutInQueue_ = false;
}

WeaponPrm::~WeaponPrm()
{
}

void WeaponPrm::serialize(Archive& ar)
{
	ar.serialize(ID_, "ID", 0);
	ar.serialize(tipsName_, "tipsName", "��� ������ ��� ����������/����� ��� ������");

	ar.serialize(aimLockTime_, "aimLockTime", "����� ����������� � �������� ���������");

	ar.serialize(queueFire_, "queueFire", "�������� ���������");
	if(queueFire_){
		float time = float(queueFireDelay_) / 1000.f;
		ar.serialize(time, "queueFireDelay", "�������� ����� ���������� � �������");
		queueFireDelay_ = round(time * 1000.f);
	}

	ar.serialize(clearTargets_, "clearTargets", "���������� ���� ����� ��������");
	ar.serialize(exclusiveTarget_, "exclusiveTarget", "�� ����� ������ ���� � ������ �������");
	ar.serialize(clearAttackClickMode_, "clearAttackClickMode", "���������� ������ ����� �������� ����");

	ar.serialize(targetMark_, "targetMark", "������������ ����� ������������");
	ar.serialize(hideCursor_, "hideCursor", "������� ������ �� ����� ������������");

	if(weaponClass_ == WeaponPrm::WEAPON_BEAM || weaponClass_ == WeaponPrm::WEAPON_GRIP){
		ar.serialize(damage_, "damage", "�����������");
	}

	ar.serialize(attackClass_, "attackClass", "����� ��������� ������");
	ar.serialize(affectMode_, "affectMode", "�� ���� ���������");

	ar.serialize(rangeType_, "rangeType", "��� ������");
	ar.serialize(shootingMode_, "shootingMode", "����� ��������");
	ar.serialize(directControlMode_, "directControlMode", "����� �������� � ������ ����������");
	ar.serialize(directControlEffect_, "directControlEffect", "���������� ��� ������� ����������");
	ar.serialize(directControlDisabledEffect_, "directControlDisabledEffect", "���������� ��� ������� ���������� ��� �������� �������� ��� ��������");

	ar.serialize(effect_, "effect", "���������� ��� �������� ������");
	ar.serialize(disabledEffect_, "disabledEffect", "���������� ��� �������� ������ ��� �������� �������� ��� ��������");

	ar.serialize(canShootThroughShield_, "canShootThroughShield", "�������� ������ �������� ����");
	ar.serialize(canShootUnderFogOfWar_, "canShootUnderFogOfWar", "�������� ��� ����� �����");

	ar.serialize(shootingOnMoveMode_, "shootingOnMoveMode", "����� ��������");

	ar.serialize(isInterrupt_, "isInterrupt", "����������� ��������");
	ar.serialize(clearTargetOnLoading_, "clearTargetOnLoading", "���������� ���� �� ����� �����������");

	ar.serialize(disableOwnerMove_, "disableOwnerMove", "��������� ������ � ������� ����� � ����");

	ar.openBlock("surfaceMode", "��������� ����");
	for(int i = 0; i < UNIT_MODE_COUNT; i++){
		UnitMode mode = UnitMode(i);
		if(!ar.serialize(targetUnitMode_[i], getEnumName(mode), getEnumNameAlt(mode)))
			targetUnitMode_[i] = TARGET_MODE_ALL;
	}
	ar.closeBlock();

	ar.serialize(fireDuringClick_, "fireDuringClick", "�������� ���� ������ ������");

	ar.serialize(fireCost_, "fireCost", "��������� ��������");
	ar.serialize(fireCostAtOnce_, "fireCostAtOnce", "������� ��� ��������� �����");

	ar.serialize(parameters_, "parameters", "������ ��������� (���������, ����� ��������, ����� �����������, ��������)");
	ar.serialize(visibleTimeOnShoot_, "visibleTimeOnShoot", "����� ��������� ��� ��������");
	ar.serialize(fireRadiusCircle_, "fireRadiusCircle", "������ ������� �����");
	ar.serialize(fireMinRadiusCircle_, "fireMinRadiusCircle", "������ ������������ ������� �����");

	if(weaponClass_ == WeaponPrm::WEAPON_BEAM || weaponClass_ == WeaponPrm::WEAPON_GRIP)
		ar.serialize(abnormalState_, "abnormalState", "����������� �� ����");

	ar.serialize(fireEffect_, "fireEffect", "���������� ��������");
	ar.serialize(fireSound_, "fireSound", "���� ��������");

	ar.serialize(upgrades_, "upgrades", "��������");

	ar.openBlock("cost", "��������� ��������");
	ar.serialize(accessValue_, "accessValue", "����������� ���������");
	
	ar.serialize(accessBuildingsList_, "accessBuildingsList", "����������� ��������");

	ar.closeBlock();
}

void WeaponPrm::updateIdentifiers()
{
	int id = 1;
	for(WeaponPrmLibrary::Strings::const_iterator it = WeaponPrmLibrary::instance().strings().begin(); it != WeaponPrmLibrary::instance().strings().end(); ++it) {
		if(it->get()) {
			it->get()->setID(id++);
		}
	}
}

const WeaponPrm* WeaponPrm::accessibleUpgrade(UnitActing* ownerUnit) const
{
	const WeaponPrm* upgrade = 0;

	WeaponPrm::Upgrades::const_iterator it;
	FOR_EACH(upgrades(), it){
		if(const WeaponPrm* p = (*it)()){
			if(ownerUnit->weaponAccessible(p)){
				upgrade = p;
			}
		}
	}


	if(upgrade){
		const WeaponPrm* prm = upgrade->accessibleUpgrade(ownerUnit);

		if(prm)
			upgrade = prm;
	}

	return upgrade;
}

bool WeaponPrm::checkTargetMode(UnitMode owner_mode, UnitMode target_mode) const 
{
	start_timer_auto();

	return (targetUnitMode_[owner_mode] & (1 << target_mode)) != 0;
}

WeaponPrm* WeaponPrm::getWeaponPrmByID(int id)
{
	WeaponPrmLibrary::Strings::const_iterator it;
	FOR_EACH(WeaponPrmLibrary::instance().strings(), it)
		if(it->get() && it->get()->ID() == id)
			return it->get();
	return 0;
}

WeaponPrm* WeaponPrm::defaultPrm()
{
	static WeaponPrm prm;
	prm.setShootingMode(SHOOT_MODE_INTERFACE);
	return &prm;
}

// ------------------------------

AccessBuilding::AccessBuilding()
{
	needConstructed = true;
}

void AccessBuilding::serialize(Archive& ar)
{
	ar.serialize(building, "building", "&������");
	ar.serialize(needConstructed, "needConstructed", "&������ ���� ���������");
}

bool AccessBuildings::serialize(Archive& ar, const char* name, const char* nameAlt)
{
	return ar.serialize(static_cast<vector<AccessBuilding>&>(*this), name, "&���");
}

// ------------------------------

void WeaponGroupType::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(shootingMode_, "shootingMode", "����� �������� � ������");
}

