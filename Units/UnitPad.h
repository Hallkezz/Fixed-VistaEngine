#ifndef __UNIT_PAD_H__
#define __UNIT_PAD_H__

#include "UnitActing.h"

//////////////////////////////////////////
// ����-����
//////////////////////////////////////////

class AttributePad : public AttributeBase
{
public:
	AttributePad();
	void serialize(Archive& ar);

	vector<Logic3dxNode> nodes;
};

class UnitPad : public UnitActing
{
public:
	UnitPad(const UnitTemplate& data);

	const AttributePad& attr() const { return safe_cast_ref<const AttributePad&>(__super::attr()); }
	bool selectAble() const { return false; }
};


//////////////////////////////////////////
// ������� ������
//////////////////////////////////////////

class AttributePlayer : public AttributeBase
{
public:
	AttributePlayer();

	void serialize(Archive& ar);

	float padRespawnTime;
};

class UnitPlayer : public UnitActing
{
public:
	UnitPlayer(const UnitTemplate& data);

	const AttributePlayer& attr() const { return safe_cast_ref<const AttributePlayer&>(__super::attr()); }

	void serialize(Archive& ar);

	void executeCommand(const UnitCommand& command);

	void Quant();

	bool selectAble() const { return false; }

	float currentCapacity() const { return currentCapacity_; } // [0..1]
	void setCurrentCapacity(float value) { currentCapacity_ = value; }

	bool waitPadRespawn() const { return padWaitRespawn_.busy(); }

	UnitPad* createPad(int weaponid, const AttributePad* attr);
	UnitPad* getPad(int weaponid) const { return (!weaponid || weaponid == padWeaponId_) ? pad_ : 0; }
	void destroyPad(int weaponid);

	void setUsedByTrigger(int priority, const void* action, int time) {}
	bool checkUsage(void* action) const { return true; }

private:
	void validatePad();
	float currentCapacity_;

	int padWeaponId_;
	UnitLink<UnitPad> pad_;
	ParameterSet padPrm_;

	/// ���� �� �������, ����������� ������. ����� ������� � ���� �� �����������.
	bool padSelfDestroyed_;
	/// ���� ����������, ���� �����������
	LogicTimer padWaitRespawn_;
};

#endif