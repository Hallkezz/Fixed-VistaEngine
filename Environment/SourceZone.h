#ifndef __ZONE_SOURCE_H_INCLUDED__
#define __ZONE_SOURCE_H_INCLUDED__

#include "SourceBase.h"
#include "EffectReference.h"
#include "SourceEffect.h"
#include "..\Util\Range.h"
#include "..\Units\UnitAttribute.h"
#include "..\Units\IronBullet.h"
#include "..\render\src\lighting.h"

class UnitBase;

class SourceZone : public SourceBase
{
public:
	/// ��� ���� (������������� ��� ��������������)
	enum ZoneEditType {
		ZONE_GENERATOR,
		ZONE_AFFECT,
		ZONE_WALK_EFFECT
	};

	SourceZone();
	SourceZone(const SourceZone& original);
	SourceBase* clone () const {
		return new SourceZone(*this);
	}
	~SourceZone();

	void setRadius (float radius) {
		__super::setRadius(radius);
		if (enabled_ && active_) {
			effectInit();
			effectStart();
		}
	}

	SourceType type() const { return SOURCE_ZONE; }
	Se3f currentPose();

	void serialize(Archive& ar);

	void setEditType(ZoneEditType editType){ editType_ = editType; }
	ZoneEditType editType() const { return editType_; }
	const AttributeBase* generatedUnit() const { return unitReference_; }

	void setEffect(const EffectAttribute& effectAttribute);

	void setEffectRadius (Rangef radius) { effectRadius_ = radius; }
	Rangef effectRadius () const { return effectRadius_; }

	void setEffectScale (Rangef scale) { effectScale_ = scale; }
	Rangef effectScale () const { return effectScale_; }

	const ParameterCustom& damage() const { return damage_; }
	const AbnormalStateAttribute& abnormalState() const { return abnormalState_; }

	void quant();

	void apply(UnitBase* target);

	/// ������ ��������� ��������
	enum UnitGenerationMode
	{
		/// ������� ���������� �� ����������� ����
		GENERATION_MODE_TERRAIN,
		/// ����� ������ �������������� ���������� �� ����
		GENERATION_MODE_ZONE,
		/// ������� ������ � ����
		GENERATION_MODE_SKY
	};

	UnitGenerationMode unitGenerationMode() const { return unitGenerationMode_; } 

	void showDebug() const;

	bool getParameters(WeaponSourcePrm& prm) const;
	bool setParameters(const WeaponSourcePrm& prm, const WeaponTarget* target = 0);

protected:
	void start();
	void stop();

private:

	/// ��������� - ���./����.
	ZoneEditType editType_;

	/// ������������ ���� ��� ���
	bool enableRotation_;

	Rangef effectRadius_;
	Rangef effectScale_;
	EffectAttribute effectAttribute_;
	int effectTime_;

	/// �����������, ��������� �����
	ParameterCustom damage_;
	/// ����������� �� �����
	AbnormalStateAttribute abnormalState_;

	/// ������������ ����� ������
	AttributeProjectileReference projectileReference_;
	/// ������������ ����� ����
	AttributeReference unitReference_;
	bool createBuiltBuilding_;
	/// �������� �������������� �����
	Rangef unitAnglePsi_;
	/// �������� ���������� �� ���������
	Rangef unitAngleTheta_;
	/// �������� ��������� ���������
	Rangef unitVelocity_;
	/// �������� ������� ���������
	Rangef unitAngularVelocity_;
	/// ����� ���������
	UnitGenerationMode unitGenerationMode_;
	/// ������������� ��������� ��������, � ��������
	Rangef unitGenerationPeriod_;
	/// ���������� ������������ ������������ ������
	int unitCount_;
	/// ���������� ���������� � ������� ������
	int unitCur_;
	/// �������� ��������� ������ (���� 0 ���������� ��� �����)
	int unitGenerationInterval_;
	/// ����������� ��������
	bool unitGenerationOnce_;
	/// ������, �� ������� ���������� ������� (��� GENERATION_MODE_SKY)
	float unitGenerationHeight_;
	/// ��������� �� �������
	bool generateByRadius_;
	
	DurationTimer unitGenerationTimer_;
	DurationTimer unitIntervalTimer_;

	struct EffectControllerNode{
		EffectController effect;
		int a, b, c, d;
		float angle;
		float effect_radius;
		EffectControllerNode(const BaseUniverseObject* owner, const Vect2f delta = Vect2f::ZERO):
			effect(owner, delta){
			effect.setPlacementMode(EffectController::PLACEMENT_BY_ATTRIBUTE);
			};
		~EffectControllerNode(){};
	};
	typedef std::vector<EffectControllerNode> EffectControllers;
	EffectControllers effectControllers_;
	/// �������� ������������ ������� ����������� �������
	Rangef walk_effect_range;

	/// ����� �� ������� ���������� ������ ������� ������ ����
	int wander_time;

	void effectInit();
	void effectStart();
	void effectStop();

	float scaleRnd() const;

	void generate_effect_modificators(EffectControllerNode&);

	/// ������ ����, ������������� ��� ������� � ��������
	void generateUnits();

	void initZone();

	void applyDamage(UnitBase* target) const;
};

class SourceDetector : public SourceDamage
{
public:
	enum ZoneEditType {
		ZONE_DETECTOR,
		ZONE_HIDER
	};

	SourceDetector();
	SourceType type() const { return SOURCE_DETECTOR; }
	SourceBase* clone() const { return new SourceDetector(*this); }

	void quant();
	void serialize(Archive& ar);

	void sourceApply(SourceBase* source);
	void apply(UnitBase* target);
	bool canApply(const UnitBase* target) const;
private:
	ZoneEditType editType_;
};

class SourceFreeze : public SourceDamage
{
public:
	SourceFreeze();
	SourceType type() const { return SOURCE_FREEZE; }
	SourceBase* clone() const { return new SourceFreeze(*this); }
	
	void apply(UnitBase* target);
};

#endif // #ifndef __ZONE_SOURCE_H_INCLUDED__
