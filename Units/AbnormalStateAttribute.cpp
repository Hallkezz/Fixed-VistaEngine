#include "stdafx.h"

#include "Serialization\StringTableImpl.h"
#include "UnitAttribute.h"
#include "Serialization\SerializationFactory.h"

WRAP_LIBRARY(ExplodeTable, "ExplodeTable", "���� �������", "Scripts\\Engine\\ExplodeTable", 0, 0);

WRAP_LIBRARY(AbnormalStateTypeTable, "AbnormalStateType", "���� �����������", "Scripts\\Content\\AbnormalStateType", 1, LIBRARY_EDITABLE);

REGISTER_CLASS(DeathAttribute, DeathAttribute, "DeathAttribute")

void ExplodeProperty::serialize(Archive& ar) 
{
	StringTableBase::serialize(ar); 
	ar.serialize(enableExplode, "enableExplode", "enableExplode"); 
	ar.serialize(animatedDeath, "animatedDeath", "animatedDeath"); 
	ar.serialize(enableRagDoll, "enableRagDoll", "enableRagDoll");
	ar.serialize(alphaDisappear, "alphaDisappear", "alphaDisappear"); 
	ar.serialize(explodeFactors, "explodeAxisFactors", "explodeAxisFactors" ); 
	ar.serialize(selfPower, "selfPower", "selfPower"); 
	ar.serialize(liveTime, "liveTimeInt", "liveTime");
	ar.serialize(z_gravity, "z_gravity", "z_gravity"); 
	ar.serialize(friction, "friction", "friction");
	ar.serialize(restitution, "restitution", "restitution"); 
}

// -----------------------------------------
int AbnormalStateType::maskCounter_ = 0;

AbnormalStateType::AbnormalStateType(const char* name) 
: StringTableBase(name)
{
	priority = 1;
}

void AbnormalStateType::serialize(Archive& ar)
{
	if(ar.isInput() && !ar.isEdit()){
		mask_ = 1 << maskCounter_++;
		xassert(maskCounter_ <= 31 && "����� ���������� ����������� ������ ���� ������ 32");
	}

	StringTableBase::serialize(ar);
	ar.serialize(priority, "priority", "&���������");
}

// -----------------------------------------

DeathAttribute::DeathAttribute()
{
	enableExplodeFantom = false;
	explodeFactor = 1.0f;
}

void DeathAttribute::serialize(Archive& ar)
{
	ar.serialize(explodeReference, "explodeReference", "��� ������");
	ar.serialize(explodeFactor, "explodeFactor", "����������� �������� ������");
	ar.serialize(effectAttr, "effectAttr", "������ ��� ��������");
	ar.serialize(effectAttrFly, "effectAttrFly", "������ �� �����");
	ar.serialize(enableExplodeFantom, "enableExplodeFantom", "��������� ������� �� ����");

	ar.serialize(sources, "sources", "���������, ���������� ����� ������");
}

// -----------------------------------------
UnitColor UnitColor::ZERO;

AbnormalStateEffect::AbnormalStateEffect()
{
	deathAttribute_ = 0;
}

AbnormalStateEffect::~AbnormalStateEffect()
{
	delete deathAttribute_;
}

void AbnormalStateEffect::serialize(Archive& ar)
{
	ar.serialize(type_, "type", "&���");

	ar.serialize(effectColor_, "effectColor", "���� �����");
	ar.serialize(effectAttribute_, "effectAttribute", "������");
	ar.serialize(soundReference_, "soundReference", "����");

	if(!ar.inPlace()){
		bool hasDeathAttribute = deathAttribute_ != 0;
		ar.serialize(hasDeathAttribute, "hasDeathAttribute", "���� ��������� ������");
		if(hasDeathAttribute && !deathAttribute_)
			deathAttribute_ = new DeathAttribute;
		else if(!hasDeathAttribute && deathAttribute_){
			delete deathAttribute_;
			deathAttribute_ = 0;
		}
		if(deathAttribute_)
			ar.serialize(*deathAttribute_, "deathAttribute", "������");
	}
	else
		ar.serializePolymorphic(deathAttribute_, "deathAttribute", "������");
}

// -----------------------------------------

AbnormalStateAttribute::AbnormalStateAttribute()
{
	duration_ = 1.0f;
	durationRnd_ = 0.0f;
	useArithmetics_ = false;
	freeze_ = false;
	freezeAttack_ = false;
}

void AbnormalStateAttribute::serialize(Archive& ar)
{
	ar.serialize(type_, "type", "&���");

	ar.serialize(duration_, "duration", "������������ ��������");
	ar.serialize(durationRnd_, "durationRnd", "������� ������������ ��������");

	damage_ *= 1.f/logicPeriodSeconds;
	ar.serialize(damage_, "damage", "����������� � �������");
	damage_ *= logicPeriodSeconds;

	ar.serialize(useArithmetics_, "useArithmetics", "������������ ����������");
	if(useArithmetics_ || ar.inPlace())
		ar.serialize(arithmetics_, "arithmetics", "����������");
	
	ar.serialize(freeze_, "freeze", "������������");
	ar.serialize(freezeAttack_, "freezeAttack", "������������ ���� ����� ���������");
}

void AbnormalStateAttribute::applyParameterArithmetics(const ArithmeticsData& arithmetics)
{
	if(arithmetics.address & ArithmeticsData::DAMAGE)
		arithmetics.apply(damage_);
}

void AbnormalStateAttribute::applyParameterArithmeticsOnDamage(const ArithmeticsData& parameterArithmetics)
{
	parameterArithmetics.apply(damage_);
}

