#ifndef __WEAPON_ATTRIBUTE_H__
#define __WEAPON_ATTRIBUTE_H__

#include "Parameters.h"
#include "AbnormalStateAttribute.h"
#include "..\Util\EffectReference.h"
#include "CircleManagerParam.h"
#include "..\UserInterface\UI_MarkObjectAttribute.h"

#include "WeaponEnums.h"

class Player;
class WeaponPrmCache;
class UnitActing;

enum EnvironmentType { // ��� ��������� ��������� convertEnvironmentType2Idx � AttackClass
	ENVIRONMENT_PHANTOM = 0, 
	ENVIRONMENT_PHANTOM2 = 1 << 0, // ������ ����������� � PathTracking-�
	ENVIRONMENT_BUSH = 1 << 1, 
	ENVIRONMENT_TREE = 1 << 2, 
	ENVIRONMENT_FENCE = 1 << 3, 
	ENVIRONMENT_FENCE2 = 1 << 4, // �����, ������������� ��� ��������
	ENVIRONMENT_STONE = 1 << 5, 
	ENVIRONMENT_ROCK = 1 << 6, 

	ENVIRONMENT_SIMPLE_MAX = ENVIRONMENT_ROCK,

	ENVIRONMENT_BASEMENT = 1 << 7, 
	ENVIRONMENT_BARN = 1 << 8, 
	ENVIRONMENT_BUILDING = 1 << 9, 
	ENVIRONMENT_BRIDGE = 1 << 10, 
	ENVIRONMENT_BIG_BUILDING = 1 << 11, 
	ENVIRONMENT_INDESTRUCTIBLE = 1 << 12, // should be the last 
	ENVIRONMENT_TYPE_MAX = 14
};

inline bool isEnvironmentSimple(EnvironmentType environmentType) 
{
	return environmentType <= ENVIRONMENT_SIMPLE_MAX;
}

enum AttackClass
{
	ATTACK_CLASS_IGNORE = 0, // �����

	ATTACK_CLASS_ENVIRONMENT_BUSH = ENVIRONMENT_BUSH, 
	ATTACK_CLASS_ENVIRONMENT_TREE = ENVIRONMENT_TREE, 
	ATTACK_CLASS_ENVIRONMENT_FENCE = ENVIRONMENT_FENCE, 
	ATTACK_CLASS_ENVIRONMENT_FENCE2 = ENVIRONMENT_FENCE2, 
	ATTACK_CLASS_ENVIRONMENT_BARN = ENVIRONMENT_BARN, 
	ATTACK_CLASS_ENVIRONMENT_BUILDING = ENVIRONMENT_BUILDING, 
	ATTACK_CLASS_ENVIRONMENT_BRIDGE = ENVIRONMENT_BRIDGE, 
	ATTACK_CLASS_ENVIRONMENT_STONE = ENVIRONMENT_STONE, 
	ATTACK_CLASS_ENVIRONMENT_BIG_BUILDING = ENVIRONMENT_BIG_BUILDING, 
	ATTACK_CLASS_ENVIRONMENT_INDESTRUCTIBLE = ENVIRONMENT_INDESTRUCTIBLE, 

	ATTACK_CLASS_ENVIRONMENT = ATTACK_CLASS_ENVIRONMENT_BUSH | ATTACK_CLASS_ENVIRONMENT_TREE | ATTACK_CLASS_ENVIRONMENT_FENCE | ATTACK_CLASS_ENVIRONMENT_FENCE2 |
		ATTACK_CLASS_ENVIRONMENT_BARN | ATTACK_CLASS_ENVIRONMENT_BUILDING | ATTACK_CLASS_ENVIRONMENT_BRIDGE |
		ATTACK_CLASS_ENVIRONMENT_INDESTRUCTIBLE | ATTACK_CLASS_ENVIRONMENT_BIG_BUILDING,

	ATTACK_CLASS_LIGHT = 1 << 13, // ������
	ATTACK_CLASS_MEDIUM = 1 << 14, // �������
	ATTACK_CLASS_HEAVY = 1 << 15, // �������
	ATTACK_CLASS_AIR = 1 << 16, // ���������
	ATTACK_CLASS_AIR_MEDIUM = 1 << 17, // ��������� �������
	ATTACK_CLASS_AIR_HEAVY = 1 << 18, // ��������� �������
	ATTACK_CLASS_UNDERGROUND = 1 << 19, // ���������

	ATTACK_CLASS_BUILDING = 1 << 20, // ������
	ATTACK_CLASS_MISSILE = 1 << 21, // �������

	ATTACK_CLASS_TERRAIN_SOFT = 1 << 22, // ����� ��������
	ATTACK_CLASS_TERRAIN_HARD = 1 << 23, // ����� ����������
	ATTACK_CLASS_WATER = 1 << 24, // ����
	ATTACK_CLASS_WATER_LOW = 1 << 25, // ������������� ���� 
	ATTACK_CLASS_ICE = 1 << 26, // ˸�

	ATTACK_CLASS_GROUND = ATTACK_CLASS_TERRAIN_SOFT | ATTACK_CLASS_TERRAIN_HARD | ATTACK_CLASS_WATER,
	ATTACK_CLASS_GROUND_ALL = ATTACK_CLASS_TERRAIN_SOFT | ATTACK_CLASS_TERRAIN_HARD | ATTACK_CLASS_WATER | ATTACK_CLASS_WATER_LOW | ATTACK_CLASS_ICE,
	ATTACK_CLASS_ALL = ATTACK_CLASS_ENVIRONMENT | ATTACK_CLASS_LIGHT | ATTACK_CLASS_MEDIUM | ATTACK_CLASS_HEAVY |
			ATTACK_CLASS_AIR | ATTACK_CLASS_AIR_MEDIUM | ATTACK_CLASS_AIR_HEAVY | ATTACK_CLASS_UNDERGROUND |
			ATTACK_CLASS_BUILDING | ATTACK_CLASS_MISSILE | ATTACK_CLASS_GROUND | ATTACK_CLASS_WATER_LOW | ATTACK_CLASS_ICE
};

class WeaponAimAnglePrm
{
public:
	WeaponAimAnglePrm();

	void serialize(Archive& ar);

	const Object3dxNode& nodeGraphics() const { return nodeGraphics_; }
	bool hasNodeGraphics() const { return nodeGraphics_ != -1; }
	const Logic3dxNode& nodeLogic() const { return nodeLogic_; }
	bool hasNodeLogic() const { return nodeLogic_ != -1; }

	bool rotateByLogic() const { return rotateByLogic_; }

	float precision() const { return precision_; }

	float turnSpeed() const { return turnSpeed_; }
	float turnSpeedDirectControl() const { return turnSpeedDirectControl_; }

	float valueMin() const { return valueMin_; }
	float valueMax() const { return valueMax_; }
	float valueDefault() const { return valueDefault_; }

	void setValueLimits(float min, float max){ valueMin_ = min; valueMax_ = max; }

private:

	/// ��� ���� ��� ���������� ���������
	Object3dxNode nodeGraphics_;
	/// ��� ����������� ���� ��� ���������� ���������
	Logic3dxNode nodeLogic_;

	/// ������������ ����. ���� �� ���� �����������
	bool rotateByLogic_;

	/// �������� ���������
	float precision_;

	/// �������� ��������, ������/���
	float turnSpeed_;
	/// �������� �������� ��� ������ ����������, ������/���
	float turnSpeedDirectControl_;

	/// ����������� �������� 
	float valueMin_;
	/// ������������ �������� 
	float valueMax_;
	/// �������� �� ���������
	float valueDefault_;
};

struct AccessBuilding
{
	AttributeUnitOrBuildingReference building;
	bool needConstructed;
	AccessBuilding();
	void serialize(Archive& ar);
};

struct AccessBuildings : vector<AccessBuilding>
{
	bool serialize(Archive& ar, const char* name, const char* nameAlt);
};
typedef vector<AccessBuildings> AccessBuildingsList;

// ��������� ���������� ���������� ������
class WeaponAimControllerPrm
{
public:
	WeaponAimControllerPrm();

	void serialize(Archive& ar);

	const WeaponAimAnglePrm& anglePsiPrm() const { return anglePsiPrm_; }
	const WeaponAimAnglePrm& angleThetaPrm() const { return angleThetaPrm_; }

	int nodeLogic(int barrel_index = 0) const { return barrels_.empty() ? -1 : barrels_[barrel_index].nodeLogic(); }
	int nodeGraphics(int barrel_index = 0) const { return barrels_.empty() ? -1 : barrels_[barrel_index].nodeGraphics(); }

	int barrelCount() const { return barrels_.size(); }

	bool hasAnimation() const { return hasAnimation_; }
	int animationChainCounter() const { return animationChainCounter_; }
	int animationChainAimCounter() const { return animationChainAimCounter_; }

	bool isEnabled() const { return isEnabled_; }

private:

	bool isEnabled_;

	/// ����
	class Barrel
	{
	public:
		void serialize(Archive& ar);

		int nodeLogic() const { return nodeLogic_; }
		int nodeGraphics() const { return nodeGraphics_; }

	private:
		Logic3dxNode nodeLogic_;
		Object3dxNode nodeGraphics_;
	};

	typedef std::vector<Barrel> Barrels;
	Barrels barrels_;

	bool hasAnimation_;
	int animationChainCounter_;
	int animationChainAimCounter_;

	/// ���������� �������������� �����
	WeaponAimAnglePrm anglePsiPrm_;
	/// ���������� ������������ �����
	WeaponAimAnglePrm angleThetaPrm_;
};

/// ������� ��������� ������.
class WeaponPrm : public PolymorphicBase
{
public:
	WeaponPrm();
	virtual ~WeaponPrm();

	virtual void serialize(Archive& ar);

	/// ������ ������
	enum WeaponClass
	{
		/// ������� ������
		WEAPON_BEAM,
		/// ���������� ��������� ������
		WEAPON_PROJECTILE,
		/// ����������� �� ���� ������
		WEAPON_AREA_EFFECT,
		/// ��������� � ���������� ����������
		WEAPON_WAITING_SOURCE,
		/// ������ ������������
		//WEAPON_TELEPORT,
		/// ������ - ������
		WEAPON_GRIP
	};

	/// ��� ������ �� ��������������.
	enum RangeType
	{
		LONG_RANGE, ///< ������ �������� ���
		SHORT_RANGE, ///< ������ �������� ���
		ANY_RANGE, ///< ������������� ������ 
	};

	/// ������ �������� ������
	enum ShootingMode
	{
		/// ������� �����
		SHOOT_MODE_DEFAULT,
		/// �������� ������ �� ������� �� ����������
		SHOOT_MODE_INTERFACE,
		/// �������� ������
		SHOOT_MODE_ALWAYS
	};

	enum ShootingOnMoveMode
	{
		SHOOT_WHILE_STANDING		= 1,	///< ����� �������� ����� �����
		SHOOT_WHILE_MOVING			= 2,	///< ����� �������� ����� ���
		SHOOT_WHILE_RUNNING			= 4,	///< ����� �������� ����� �����
		SHOOT_WHILE_IN_TRANSPORT	= 8		///< ����� �������� ����� ����� � ����������
	};

	/// ������������ �����
	enum UnitMode
	{
		ON_WATER_BOTTOM = 0,	///< �� ���
		ON_WATER,			///< �� ����
		ON_GROUND,			///< �� �����
		ON_AIR				///< � �������
	};

	/// ������������ ����
	/// �������� ��� ���������� ��� ��������������
	enum TargetUnitMode
	{
		TARGET_ON_WATER_BOTTOM	= 1, ///< �� ���
		TARGET_ON_WATER			= 2, ///< �� ����
		TARGET_ON_GROUND		= 4, ///< �� �����
		TARGET_ON_AIR			= 8  ///< � �������
	};

	enum {
		UNIT_MODE_COUNT = 4,
		TARGET_MODE_ALL = TARGET_ON_WATER_BOTTOM | TARGET_ON_WATER | TARGET_ON_GROUND | TARGET_ON_AIR
	};

	int ID() const { return ID_; }
	void setID(int id){ ID_ = id; }

	const char* label() const { return tipsName_.key(); }

	bool clearTargets() const { return clearTargets_; }
	bool exclusiveTarget() const { return exclusiveTarget_; }
	bool clearAttackClickMode() const { return clearAttackClickMode_; }
	bool alwaysPutInQueue() const { return alwaysPutInQueue_; }

	virtual bool targetOnWaterSurface() const { return true; }

	AffectMode affectMode() const { return affectMode_; }

	WeaponClass weaponClass() const { return weaponClass_; }
	int aimLockTime() const { return int(aimLockTime_*1000.0f); }

	bool queueFire() const { return queueFire_; }
	int queueFireDelay() const { return queueFireDelay_; }

	bool isShortRange() const { return (rangeType_ == SHORT_RANGE || rangeType_ == ANY_RANGE); }
	bool isLongRange() const { return (rangeType_ == LONG_RANGE || rangeType_ == ANY_RANGE); }

	bool canShootOnMove() const { return (shootingOnMoveMode_ & SHOOT_WHILE_MOVING); }
	bool canShootWhileRunning() const { return (shootingOnMoveMode_ & SHOOT_WHILE_RUNNING); }
	bool canShootWhileStanding() const { return (shootingOnMoveMode_ & SHOOT_WHILE_STANDING); }
	bool canShootWhileInTransport() const { return (shootingOnMoveMode_ & SHOOT_WHILE_IN_TRANSPORT); }
	bool isInterrupt() const { return isInterrupt_; }
	bool fireDuringClick() const { return fireDuringClick_; }

	bool canShootThroughShield() const { return canShootThroughShield_; }
	bool canShootUnderFogOfWar() const { return canShootUnderFogOfWar_; }
	bool clearTargetOnLoading() const { return clearTargetOnLoading_; }

	bool disableOwnerMove() const { return disableOwnerMove_; }

	const WeaponDamage& damage() const { return damage_; }

	const AbnormalStateAttribute& abnormalState() const { return abnormalState_; }

	const EffectAttributeAttachable& directControlEffect() const { return directControlEffect_; }
	const EffectAttributeAttachable& directControlDisabledEffect() const { return directControlDisabledEffect_; }

	const EffectAttributeAttachable& effect() const { return effect_; }
	const EffectAttributeAttachable& disabledEffect() const { return disabledEffect_; }

	const EffectAttributeAttachable& fireEffect() const { return fireEffect_; }
	const SoundAttribute* fireSound() const { return fireSound_; }
	const CircleEffect& fireRadiusCircle() const { return fireRadiusCircle_; }
	const CircleEffect& fireMinRadiusCircle() const { return fireMinRadiusCircle_; }

	int attackClass() const { return attackClass_; }

	bool fireCostAtOnce() const { return fireCostAtOnce_; }
	const ParameterCustom& fireCost() const { return fireCost_; }
	const ParameterCustom& parameters() const { return parameters_; }

	float visibleTimeOnShoot() const { return visibleTimeOnShoot_; }

	ShootingMode shootingMode() const { return shootingMode_; }
	void setShootingMode(ShootingMode mode){ shootingMode_ = mode; }

	WeaponDirectControlMode directControlMode() const { return directControlMode_; }

	class Upgrade
	{
	public:
		void serialize(Archive& ar);
		const WeaponPrmReference& reference() const{ return reference_; }
		const WeaponPrm* operator()() const { return reference_; }
	private:
		WeaponPrmReference reference_;
	};

	typedef std::vector<Upgrade> Upgrades;
	const Upgrades& upgrades() const { return upgrades_; }
	const WeaponPrm* accessibleUpgrade(UnitActing* ownerUnit) const;

	const AccessBuildingsList& accessBuildingsList() const { return accessBuildingsList_; }
	const ParameterCustom& accessValue() const { return accessValue_; }

	bool hasTargetMark() const { return !targetMark_.isEmpty(); }
	const UI_MarkObjectAttribute& targetMark() const { return targetMark_; }
	bool hideCursor() const { return hideCursor_; }

	bool checkTargetMode(UnitMode owner_mode, UnitMode target_mode) const;

	virtual void initCache(WeaponPrmCache& cache) const { }

	static void updateIdentifiers();

	static WeaponPrm* getWeaponPrmByID(int id);
	static WeaponPrm* defaultPrm();

protected:

	/// ����� ������
	WeaponClass weaponClass_;
	/// ���� true, �� ���� ������ �������� � �������
	bool alwaysPutInQueue_;

private:

	/// ���������� ID ������
	/// ��������������� ��� ������ ������ ����������
	int ID_;
	
	LocString tipsName_;

	/// �� ���� ��������� - �� �����/�������/�����/���� ������
	AffectMode affectMode_;

	/// ���� true, �� ���� ����� �������� ������������
	bool clearTargets_;
	/// ���� true, �� ����������� ������ ���� �� ����������� ������� ������
	bool exclusiveTarget_;
	/// ���� true, �� ����� �������� ���� ����� (\a UI_LogicDispatcher::clickMode_) ������������
	bool clearAttackClickMode_;

	/// ��������� ������ � ������� ����� � ����
	bool disableOwnerMove_;

	/// ��������� ������ - ��������� ��������, ������� � �.�.
	ParameterCustom parameters_;

	/// ��������� ������������ ������� �����
	CircleEffect fireRadiusCircle_;
	CircleEffect fireMinRadiusCircle_;

	/// �� ����� ����� ���������� ������� ��� ��������
	float visibleTimeOnShoot_;

	/// �������� ���������
	bool queueFire_;
	/// �������� ����� ���������� � �������
	int queueFireDelay_;

	/// �����, � ������� �������� ������ ����� �������� ������� ��������� �� ����
	float aimLockTime_;

	/// ��������� �����������
	WeaponDamage damage_;

	/// ����� �� �������� ����� �����/���/�����
	BitVector<ShootingOnMoveMode> shootingOnMoveMode_;

	/// �����������?
	bool isInterrupt_;
	/// �������� ���� �� ��������� ������
	bool fireDuringClick_;
	/// �������� ������ �������� ����
	bool canShootThroughShield_;
	/// �������� ��� ����� �����
	bool canShootUnderFogOfWar_;

	/// ��� �� ���� ����� ��������
	std::vector<BitVector<TargetUnitMode> > targetUnitMode_;

	/// ������ ������� ��� �����������
	bool clearTargetOnLoading_;

	/// ����������� �� ����
	AbnormalStateAttribute abnormalState_;

	/// ��� ������ - �������� ��� �������� ��������
	RangeType rangeType_;

	/// ����� ��������
	ShootingMode shootingMode_;
	/// ����� �������� � ������ ����������
	WeaponDirectControlMode directControlMode_;
	/// ����������, ������� ���������� �� ����� ����� ������ ������� � ������ ����������
	EffectAttributeAttachable directControlEffect_;
	/// ����������, ������� ���������� �� ����� ����� ������ ������� � ������ ���������� ���� ��� �������� ������������ ��������
	EffectAttributeAttachable directControlDisabledEffect_;

	/// ����������, ������� ���������� �� ����� ����� ������ ��������
	EffectAttributeAttachable effect_;
	/// ����������, ������� ���������� �� ����� ����� ������ ��������, �� �� ������� �������� ��� ��������
	EffectAttributeAttachable disabledEffect_;

	/// ������ ��������� ������
	BitVector<AttackClass> attackClass_;

	/// ��������� ��������
	ParameterCustom fireCost_;
	/// ������� ��� ��������� �����
	bool fireCostAtOnce_;

	/// ���������� ��������
	EffectAttributeAttachable fireEffect_;
	/// ���� ��������
	SoundReference fireSound_;

	/// ������������ ����� ������������
	UI_MarkObjectAttribute targetMark_;
	/// true ���� ���� ������� ������ �� ����� ������������
	bool hideCursor_;

	Upgrades upgrades_;

	AccessBuildingsList accessBuildingsList_;
	ParameterCustom accessValue_;

	bool checkConditions(const UnitBase* owner_unit) const;
};

/// ������ ������ ������ � ������
enum WeaponGroupShootingMode
{
	WEAPON_GROUP_INDEPENDENT, ///< ����������
	WEAPON_GROUP_MODE_PRIORITY ///< � ������������ � �����������
};

class WeaponGroupType : public StringTableBase
{
public:
	WeaponGroupType(const char* name = "") : StringTableBase(name) { shootingMode_ = WEAPON_GROUP_INDEPENDENT; }

	void serialize(Archive& ar);

	bool operator > (const WeaponGroupType& rhs) const { return index_ > rhs.index_; }
	bool operator < (const WeaponGroupType& rhs) const { return index_ < rhs.index_; }

	WeaponGroupShootingMode shootingMode() const { return shootingMode_; }

private:
	WeaponGroupShootingMode shootingMode_;
};

typedef StringTable<WeaponGroupType> WeaponGroupTypeTable;
typedef StringTableReference<WeaponGroupType, true> WeaponGroupTypeReference;

/// �������� ������
class WeaponSlotAttribute
{
public:
	WeaponSlotAttribute();

	void serialize(Archive& ar);

	const WeaponAimControllerPrm& aimControllerPrm() const { return aimControllerPrm_; }
	const WeaponPrm* weaponPrm() const { return weaponPrmReference_; }
	const WeaponPrmReference weaponPrmReference() const { return weaponPrmReference_; }

	int equipmentCellType() const { return equipmentCellType_; }

	bool isEmpty() const { return !weaponPrm(); }

	const WeaponGroupType* groupType() const { return groupType_; }
	int priority() const { return priority_; }

private:

	WeaponAimControllerPrm aimControllerPrm_;
	WeaponPrmReference weaponPrmReference_;

	int equipmentCellType_;

	WeaponGroupTypeReference groupType_;
	/// ��������� - ��� ������, ��� ������ ������
	int priority_;
};


#endif // __WEAPON_ATTRIBUTE_H__

