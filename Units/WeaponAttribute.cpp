#include "stdafx.h"

#include "WeaponAttribute.h"
#include "WeaponPrms.h"
#include "UnitAttribute.h"
#include "Serialization\XPrmArchive.h"
#include "Serialization\MultiArchive.h"
#include "Serialization\StringTableImpl.h"
#include "UnitActing.h"
#include "Serialization\SerializationFactory.h"
#include "Serialization\ResourceSelector.h"

#include "Player.h"

BEGIN_ENUM_DESCRIPTOR(UI_MarkObjectModeID, "UI_MarkObjectModeID")
REGISTER_ENUM(UI_MARK_NONE, "�� ���������")
REGISTER_ENUM(UI_MARK_CAN_ATTACK_ENEMY, "����� ���������")
REGISTER_ENUM(UI_MARK_CAN_PICK_UNIT, "����� ����� ����")
REGISTER_ENUM(UI_MARK_CAN_PICK_RESOURCE, "����� ��������� �������")
END_ENUM_DESCRIPTOR(UI_MarkObjectModeID)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, WeaponClass, "WeaponPrm::WeaponClass")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_BEAM, "������� ������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_PROJECTILE, "���������� ��������� ������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_AREA_EFFECT, "����������� �� ���� ������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_PAD, "���� ������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_WAITING_SOURCE, "��������� � ���������� ����������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_GRIP, "������ - ������")
//REGISTER_ENUM_ENCLOSED(WeaponPrm, WEAPON_TELEPORT, "������ ������������")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, WeaponClass)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, ShootingMode, "WeaponPrm::ShootingMode")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_MODE_DEFAULT, "������� �����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_MODE_INTERFACE, "�� ������� �� ����������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_MODE_ALWAYS, "������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, SHOOT_MODE_SQUAD_LEADER, "����� ������� � ������")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, ShootingMode)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, UnitMode, "WeaponPrm::UnitMode")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_WATER_BOTTOM, "�� ���")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_WATER, "�� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_GROUND, "�� �����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_AIR, "� �������")
REGISTER_ENUM_ENCLOSED(WeaponPrm, ON_GROUND_LYING, "���� �� �����")
END_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, UnitMode)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(WeaponPrm, TargetUnitMode, "WeaponPrm::TargetUnitMode")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_WATER_BOTTOM, "���� �� ���")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_WATER, "��������� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_GROUND, "�������� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_AIR, "��������� ����")
REGISTER_ENUM_ENCLOSED(WeaponPrm, TARGET_ON_GROUND_LYING, "������� �� ����� ����")
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

BEGIN_ENUM_DESCRIPTOR(WeaponSyndicateControlMode, "WeaponSyndicateControlMode")
REGISTER_ENUM(WEAPON_SYNDICATE_CONTROL_DISABLE, "�� ��������")
REGISTER_ENUM(WEAPON_SYNDICATE_CONTROL_NORMAL, "�������� ���� ������� �� ����������")
REGISTER_ENUM(WEAPON_SYNDICATE_CONTROL_FORCE, "�������� �� ������� ����� ���� ��������")
REGISTER_ENUM(WEAPON_SYNDICATE_CONTROL_AUTO, "������������� �������� ����")
END_ENUM_DESCRIPTOR(WeaponSyndicateControlMode)

BEGIN_ENUM_DESCRIPTOR(WeaponSourcesCreationMode, "WeaponSourcesCreationMode")
REGISTER_ENUM(WEAPON_SOURCES_CREATE_ON_TARGET_HIT, "��� ��������� � ����")
REGISTER_ENUM(WEAPON_SOURCES_CREATE_ON_GROUND_HIT, "��� ��������� � �����")
REGISTER_ENUM(WEAPON_SOURCES_CREATE_ALWAYS, "������")
END_ENUM_DESCRIPTOR(WeaponSourcesCreationMode)

BEGIN_ENUM_DESCRIPTOR(WeaponGroupShootingMode, "WeaponGroupShootingMode")
REGISTER_ENUM(WEAPON_GROUP_INDEPENDENT, "����������")
REGISTER_ENUM(WEAPON_GROUP_MODE_PRIORITY, "� ������������ � �����������")
END_ENUM_DESCRIPTOR(WeaponGroupShootingMode)

WRAP_LIBRARY(WeaponGroupTypeTable, "WeaponGroup", "���� ����� ������", "Scripts\\Content\\WeaponGroup", 1, LIBRARY_EDITABLE);
WRAP_LIBRARY(WeaponAmmoTypeTable, "WeaponAmmoType", "���� �������� � ������", "Scripts\\Content\\WeaponAmmoType", 0, LIBRARY_EDITABLE);

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

	nodeGraphicsOffset_ = Se3f::ID;
	nodeLogicOffset_ = Se3f::ID;
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

	if(ar.isOutput())
		updateOffsets();

	ar.serialize(nodeGraphicsOffset_, "nodeGraphicsOffset", 0);
	ar.serialize(nodeLogicOffset_, "nodeLogicOffset", 0);

	if(valueDefault_ < valueMin_) valueDefault_ = valueMin_;
	if(valueDefault_ > valueMax_) valueDefault_ = valueMax_;
}

bool WeaponAimAnglePrm::updateOffsets()
{
	if(cObject3dx* model = AttributeBase::model()){
		int node_index = nodeGraphics();
		if(node_index != -1){
			nodeGraphicsOffset_ = model->GetNodePosition(node_index);
			nodeGraphicsOffset_.trans() = Vect3f::ZERO;
		}
	}

	if(cObject3dx* logic_model = AttributeBase::logicModel()){
		int node_index = nodeLogic();
		if(node_index != -1){
			if(rotateByLogic()){
				string parent_node_name = nodeLogic().c_str();
				parent_node_name += "_logic";
				int parent_node = logic_model->FindNode(parent_node_name.c_str());
				if(parent_node != -1){
					Mats mat = Mats::ID;
					int index = -1;
					logic_model->GetNodeOffset(node_index, mat, parent_node);
					nodeLogicOffset_ = Se3f(mat.rot(), Vect3f::ZERO);
				}
			}
			else {
				nodeLogicOffset_ = logic_model->GetNodePosition(node_index);
				nodeLogicOffset_.trans() = Vect3f::ZERO;
			}
		}
	}

	return true;
}

WeaponAimControllerPrm::WeaponAimControllerPrm()
{
	isEnabled_ = false;
	isCorrectionEnabled_ = false;
	hasAnimation_ = false;
	angleThetaPrm_.setValueLimits(-90, 90);
}

const WeaponAimControllerPrm WeaponAimControllerPrm::EMPTY;

void WeaponAimControllerPrm::serialize(Archive& ar)
{
	if(!ar.serialize(weaponAnimationType_, "weaponAnimationType", "&��� ������ ��� ��������")){//CONVERSION 5.11.07
		WeaponPrmReference weaponPrmReference;
		ar.serialize(weaponPrmReference, "weaponPrmReference", 0);
		if(const WeaponPrm* weaponPrm = weaponPrmReference){
			WeaponAnimationTypeTable::instance().add(WeaponAnimationTypeString(weaponPrmReference.c_str()));
			const_cast<WeaponPrm*>(weaponPrm)->setAnimationType(WeaponAnimationType(weaponPrmReference.c_str()));
			weaponAnimationType_ = weaponPrm->animationType();
		}
	}
	ar.serialize(alternativeWeaponAnimationType_, "alternativeWeaponAnimationType", "��� �������������� ������ ��� ��������");

	ar.serialize(isEnabled_, "isEnabled", "&��������");
	ar.serialize(isCorrectionEnabled_, "isCorrectionEnabled", "��������� ��������� ����� ���������");

	ar.serialize(barrels_, "barrels", "������");
	
	ar.serialize(hasAnimation_, "hasAnimation", "&���� ��������");
	
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
	priority_ = 0;
	externalAnimationSettings_ = false;
	animationSlotID_ = 0;
}

int WeaponSlotAttribute::findAimControllerPrm(WeaponAnimationType weapon) const
{
	int n = aimControllersPrm_.size();
	for(int i = 0; i < n; ++i)
		if(aimControllersPrm_[i].weaponAnimationType() == weapon)
			return i;
	return -1;
}

const WeaponAimControllerPrm& WeaponSlotAttribute::aimControllerPrm(int index) const 
{ 
	if(index == -1)
		return WeaponAimControllerPrm::EMPTY;
	return aimControllersPrm_[index]; 
}

void WeaponSlotAttribute::serialize(Archive& ar)
{
	bool conversion = !ar.serialize(aimControllersPrm_, "aimControllersPrm", "���������� ����������"); //CONVERSION 1.11.07
	
	ar.serialize(weaponPrmReference_, "weaponPrmReference", "&�������� ������");

	if(conversion && weaponPrmReference_){
		aimControllersPrm_.resize(1);
		ar.serialize(aimControllersPrm_[0], "aimControllerPrm", "���������� ����������");
		aimControllersPrm_[0].setWeaponAnimationType(weaponPrmReference_->animationType());
	}

	ar.serialize(equipmentSlotType_, "equipmentSlotType", "��� ������ ����������");
	ar.serialize(groupType_, "groupType", "������ ������");
	ar.serialize(priority_, "priority", "��������� (��� ������, ��� ������ ������)");

	ar.serialize(externalAnimationSettings_, "externalAnimationSettings", "����� ��������� �������� �� ������� �����");

	if(externalAnimationSettings_)
		ar.serialize(animationSlotID_, "animationSlotID", "����� �����, �� �������� ������� ��������� ��������");
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
	ignoreMouseDblClick_ = false;

	continuousFire_ = false;

	disableOwnerMove_ = false;

	fireCostAtOnce_ = false;
	fireTimeMin_ = 0.f;

	queueFire_ = false;
	queueFireDelay_ = 200;

	weaponClass_ = WEAPON_BEAM;
	attackClass_ = ATTACK_CLASS_IGNORE;

	affectMode_ = AFFECT_ENEMY_UNITS;

	rangeType_ = LONG_RANGE;
	shootingMode_ = SHOOT_MODE_DEFAULT;
	directControlMode_ = WEAPON_DIRECT_CONTROL_NORMAL;
	syndicateControlMode_ = WEAPON_SYNDICATE_CONTROL_NORMAL;

	shootingOnMoveMode_ = SHOOT_WHILE_STANDING | SHOOT_WHILE_MOVING | SHOOT_WHILE_RUNNING;

	isInterrupt_ = true;
	fireDuringClick_ = false;
	canShootThroughShield_ = false;
	canShootUnderFogOfWar_ = true;
	clearTargetOnLoading_ = false;
	mainSquadUnitReload_ = false;

	targetUnitMode_.resize(UNIT_MODE_COUNT, TARGET_MODE_ALL);

	aimLockTime_ = 0.2f;
	disableAimReturn_ = false;
	enableAutoScan_ = false;
	autoScanPeriod_ = 10.f;
	
	visibleTimeOnShoot_ = 0.f;

	fireRadiusCircle_.color.set(255, 255, 255, 0);

	fireMinRadiusCircle_ = fireRadiusCircle_;
	fireMinRadiusCircle_.width = 2;

	fireEffectiveRadiusCircle_ = fireRadiusCircle_;

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

	ar.serialize(animationType_, "animationType", "��� ������ ��� ��������");

	ar.serialize(disableAimReturn_, "disableAimReturn", "�� ���������� � �������� ���������");
	if(!disableAimReturn_){
		ar.serialize(aimLockTime_, "aimLockTime", "����� ����������� � �������� ���������");

		ar.serialize(enableAutoScan_, "enableAutoScan", "�������������� ������������");
		if(enableAutoScan_)
			ar.serialize(autoScanPeriod_, "autoScanPeriod", "������ ��������������� ������������");
	}

	ar.serialize(queueFire_, "queueFire", "�������� ���������");
	if(queueFire_){
		float time = float(queueFireDelay_) / 1000.f;
		ar.serialize(time, "queueFireDelay", "�������� ����� ���������� � �������");
		queueFireDelay_ = round(time * 1000.f);
	}

	if(weaponClass_ == WeaponPrm::WEAPON_BEAM)
		ar.serialize(continuousFire_, "continuousFire", "����������� ��������");

	ar.serialize(clearTargets_, "clearTargets", "���������� ���� ����� ��������");
	ar.serialize(exclusiveTarget_, "exclusiveTarget", "�� ����� ������ ���� � ������ �������");
	ar.serialize(clearAttackClickMode_, "clearAttackClickMode", "���������� ������ ����� �������� ����");
	ar.serialize(ignoreMouseDblClick_, "ignoreMouseDblClick", "�� ���������� �� ������� ������");

	if(ar.openBlock("visualisation", "������������")){
		ar.serialize(targetMarks_, "targetMarks", "������������ ����� ������������");
		ar.serialize(hideCursor_, "hideCursor", "������� ������ �� ����� ������������");
	
		if(ar.openBlock("effects", "�����������")){
			ar.serialize(fireEffect_, "fireEffect", "���������� ��������");
			ar.serialize(effect_, "effect", "���������� ��� �������� ������");
			ar.serialize(disabledEffect_, "disabledEffect", "���������� ��� �������� ������ ��� �������� �������� ��� ��������");
			ar.serialize(directControlEffect_, "directControlEffect", "���������� ��� ������� ����������");
			ar.serialize(directControlDisabledEffect_, "directControlDisabledEffect", "���������� ��� ������� ���������� ��� �������� �������� ��� ��������");
			ar.closeBlock();
		}

		if(ar.openBlock("circles", "������")){
			ar.serialize(fireMinRadiusCircle_, "fireMinRadiusCircle", "������ ������������ ������� �����");
			ar.serialize(fireRadiusCircle_, "fireRadiusCircle", "������ ������� ����� ��� ��������");
			ar.serialize(fireEffectiveRadiusCircle_, "fireEffectiveRadiusCircle", "������ ������� ����� � ���������");
			ar.closeBlock();
		}
		ar.closeBlock();
	}
	ar.serialize(fireSound_, "fireSound", "���� ��������");
	
	if(weaponClass_ == WeaponPrm::WEAPON_BEAM || weaponClass_ == WeaponPrm::WEAPON_GRIP || weaponClass_ == WeaponPrm::WEAPON_PAD){
		ar.serialize(damage_, "damage", "�����������");
	}

	ar.serialize(attackClass_, "attackClass", "����� ��������� ������");
	ar.serialize(affectMode_, "affectMode", "�� ���� ���������");

	ar.serialize(rangeType_, "rangeType", "��� ������");
	ar.serialize(shootingMode_, "shootingMode", "����� ��������");
	ar.serialize(directControlMode_, "directControlMode", "����� �������� � ������ ����������");
	ar.serialize(syndicateControlMode_, "syndicateControlMode", "����� �������� � ����������� ����������");

	ar.serialize(canShootThroughShield_, "canShootThroughShield", "�������� ������ �������� ����");
	ar.serialize(canShootUnderFogOfWar_, "canShootUnderFogOfWar", "�������� ��� ����� �����");

	ar.serialize(shootingOnMoveMode_, "shootingOnMoveMode", "����� ��������");

	ar.serialize(isInterrupt_, "isInterrupt", "����������� ��������");
	ar.serialize(clearTargetOnLoading_, "clearTargetOnLoading", "���������� ���� �� ����� �����������");
	ar.serialize(mainSquadUnitReload_, "mainSquadUnitReload", "���������� ����������� ��� �������� � ������");

	ar.serialize(disableOwnerMove_, "disableOwnerMove", "��������� ������ � ������� ����� � ����");

	if(ar.openBlock("surfaceMode", "��������� ����")){
		for(int i = 0; i < UNIT_MODE_COUNT; i++){
			UnitMode mode = UnitMode(i);
			if(!ar.serialize(targetUnitMode_[i], getEnumName(mode), getEnumNameAlt(mode)))
				targetUnitMode_[i] = TARGET_MODE_ALL;
		}
		ar.closeBlock();
	}

	ar.serialize(fireDuringClick_, "fireDuringClick", "�������� ���� ������ ������");

	ar.serialize(fireCost_, "fireCost", "��������� ��������");
	ar.serialize(fireCostAtOnce_, "fireCostAtOnce", "������� ��� ��������� �����");
	if(!fireCostAtOnce_)
		ar.serialize(fireTimeMin_, "fireTimeMin", "����������� ����� ��������");

	ar.serialize(parameters_, "parameters", "������ ��������� (���������, ����� ��������, ����� �����������, ��������, ���������� �����������, ������������ ���������� �����������, ������ (���������� ���������))");

	ar.serialize(ammoType_, "ammoType", "��� �����������");

	ar.serialize(visibleTimeOnShoot_, "visibleTimeOnShoot", "����� ��������� ��� ��������");

	if(weaponClass_ == WeaponPrm::WEAPON_BEAM || weaponClass_ == WeaponPrm::WEAPON_GRIP || weaponClass_ == WeaponPrm::WEAPON_PAD)
		ar.serialize(abnormalState_, "abnormalState", "����������� �� ����");


	ar.serialize(upgrades_, "upgrades", "��������");

	if(ar.openBlock("cost", "��������� ��������")){
		ar.serialize(accessValue_, "accessValue", "����������� ���������");
		
		ar.serialize(accessBuildingsList_, "accessBuildingsList", "����������� ��������");

		ar.closeBlock();
	}
}

bool WeaponPrm::canShootMoving() const 
{
	return (shootingOnMoveMode_ & (SHOOT_WHILE_MOVING | SHOOT_WHILE_GRABBLING | SHOOT_WHILE_CRAWLING | SHOOT_WHILE_RUNNING)) != 0; 
}

bool WeaponPrm::canShootInMovementMode(bool isMoving, int movementMode) const 
{ 
	if(isMoving) 
		return (shootingOnMoveMode_ & movementMode) == movementMode; 
	return ((((shootingOnMoveMode_ << 4) | MODE_RUN) & movementMode)) == movementMode; 
}

bool WeaponPrm::canShootWhileInTransport() const 
{ 
	return (shootingOnMoveMode_ & SHOOT_WHILE_IN_TRANSPORT) != 0; 
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

void WeaponPrm::getFireCostReal(ParameterSet& out) const
{
	out = fireCost_;

	if(fireCostAtOnce_)
		return;

	float fireTime_ = round(parameters_.findByType(ParameterType::FIRE_TIME, 0.5f) * 1000.f);

	if(fireTime_ > FLT_EPS)
		out *= 1.f / (fireTime_ / 1000.f);
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

