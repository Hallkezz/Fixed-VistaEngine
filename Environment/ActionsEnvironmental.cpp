#include "stdafx.h"
#include "ActionsEnvironmental.h"

#include "VistaRender\postEffects.h"
#include "Environment.h"
#include "SourceManager.h"
#include "Anchor.h"

#include "Water\Fallout.h"
#include "RenderObjects.h"
#include "Water\SkyObject.h"
#include "Water\CoastSprites.h"
#include "vMap.h"
#include "GameOptions.h"
#include "Game\SoundApp.h"
#include "Game\Universe.h"
#include "Render\src\Gradients.h"
#include "Render\src\VisGeneric.h"
#include "Serialization\SerializationFactory.h"

STARFORCE_API void initActionsEnvironmental()
{
SECUROM_MARKER_HIGH_SECURITY_ON(12);

REGISTER_CLASS(Action, ActionActivateSources, "���������� ��������\\������������ ���������")
REGISTER_CLASS(Action, ActionKillSource, "���������� ��������\\���������� ��������")
REGISTER_CLASS(Action, ActionDeactivateSources, "���������� ��������\\�������������� ���������")
REGISTER_CLASS(Action, ActionActivateMinimapMark, "���������� ��������\\������������ ������� �� ���������")
REGISTER_CLASS(Action, ActionDeactivateMinimapMarks, "���������� ��������\\�������������� ������� �� ���������")
REGISTER_CLASS(Action, ActionSetFogOfWar, "���������� ��������\\����� �����(����������� ���������������)")
REGISTER_CLASS(Action, ActionSetSilhouette, "���������� ��������\\�������(�� ������������!!!)") 

REGISTER_CLASS(Action, ActionSetFallout, "������\\���������� ��������� �������")
REGISTER_CLASS(Action, ActionSetFalloutType, "������\\��������� ������� (������ ����������)")
REGISTER_CLASS(Action, ActionSetFalloutFlood, "������\\��������� ������� (���������� �����)")


REGISTER_CLASS(Action, ActionSetWaterOpacity, "������\\���������� ������������ ����")
REGISTER_CLASS(Action, ActionSetWind, "������\\���������� ����������� �����")
REGISTER_CLASS(Action, ActionSetFog, "������\\���������� �����")
REGISTER_CLASS(Action, ActionSetEnvironmentTime, "������\\���������� ����� �����")
REGISTER_CLASS(Action, ActionSetWaterColor, "������\\���������� ���� ����")
REGISTER_CLASS(Action, ActionSetReflectSkyColor, "������\\���������� ���� ���������� ����")
REGISTER_CLASS(Action, ActionSetTimeScale, "������\\�������� ������� ������� ����� �� ����")
REGISTER_CLASS(Action, ActionSetWaterLevel, "������\\���������� ������� ����")

REGISTER_CLASS(Action, ActionSetEffect, "���������� ��������\\��������/��������� ������");

SECUROM_MARKER_HIGH_SECURITY_OFF(12);
}

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(ActionSetFallout, TypeFallout, "TypeFallout")
REGISTER_ENUM_ENCLOSED(ActionSetFallout, NONE, "��� �������")
REGISTER_ENUM_ENCLOSED(ActionSetFallout, RAIN, "�����")
REGISTER_ENUM_ENCLOSED(ActionSetFallout, SNOW, "����")
END_ENUM_DESCRIPTOR_ENCLOSED(ActionSetFallout, TypeFallout)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(ActionSetEffect, Effects, "Effects")
REGISTER_ENUM_ENCLOSED(ActionSetEffect, EFFECT_MONOCHROME, "�����������")
REGISTER_ENUM_ENCLOSED(ActionSetEffect, EFFECT_BLOOM, "��������")
REGISTER_ENUM_ENCLOSED(ActionSetEffect, EFFECT_UNDER_WATER, "���������")
REGISTER_ENUM_ENCLOSED(ActionSetEffect, EFFECT_DOF, "������� ��������(Floating Z-Buffer)")
REGISTER_ENUM_ENCLOSED(ActionSetEffect, EFFECT_COLORDODGE, "�����������")
END_ENUM_DESCRIPTOR_ENCLOSED(ActionSetEffect, Effects)

ActionSetTimeScale::ActionSetTimeScale()
{
	dayTimeScale_ = 500.f;
	nightTimeScale_ = 1000.f;
}

void ActionSetTimeScale::serialize(Archive& ar)
{
	__super::serialize(ar);
	if(ar.openBlock("Time scale", "������� ������� �����")){
		ar.serialize(dayTimeScale_, "dayTimeScale", "������� ������� ����");
		ar.serialize(nightTimeScale_, "nightTimeScale", "������� ������� �����");
		ar.closeBlock();
	}
}

void ActionSetTimeScale::activate()
{
	if(environment)
		environment->environmentTime()->setTimeScale(dayTimeScale_, nightTimeScale_);
}

void ActionSetWaterLevel::activate()
{ 
	environment->water()->SetEnvironmentWater(waterHeight); 
	environment->water()->SetDampfK(water_dampf_k); 
}

ActionSetWind::ActionSetWind()
{
	angleDirection = 0;
	windPower = 1;
	windType = WindMap::DIRECTION_TO_CENTER_MAP;
}

void ActionSetWind::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(windType, "windType", "��� �����");
	ar.serialize(angleDirection, "angleDirection", "���� ����������� �����");
	ar.serialize(windPower, "windPower", "���� �����");
}

void ActionSetWind::activate()
{	
	float angle = M_PI * (1.5f + angleDirection / 180.0f);
	Vect2f dir = Vect2f(cos(angle) * windPower, sin(angle) * windPower);
	windMap->setDirection(dir);
	windMap->setWindType(windType);
	windMap->updateRect(0, 0, vMap.H_SIZE - 1, vMap.V_SIZE - 1);
}

ActionSetEffect::ActionSetEffect()
{
	switchMode = ON;
	effects = EFFECT_BLOOM;
}

void ActionSetEffect::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(switchMode, "switchMode", "��������");
	ar.serialize(effects, "effects", "������");
}

bool ActionSetEffect::automaticCondition() const
{
	if(!__super::automaticCondition())
		return false;

	bool flag = switchMode == ON ? true : false;
	switch(effects) {
		case EFFECT_BLOOM:
			if(flag)
				return !environment->PEManager()->isActive(PE_BLOOM);
			else 
				return environment->PEManager()->isActive(PE_BLOOM);
			
		case EFFECT_MONOCHROME:
			if(flag)
				return !environment->PEManager()->isActive(PE_MONOCHROME);
			else 
				return environment->PEManager()->isActive(PE_MONOCHROME);
			
		case EFFECT_UNDER_WATER:
			if(flag)
				return !environment->PEManager()->isActive(PE_UNDER_WATER);
			else 
				return environment->PEManager()->isActive(PE_UNDER_WATER);

		case EFFECT_DOF:
			if(flag)
				return !gb_VisGeneric->GetEnableDOF();
			else
				return gb_VisGeneric->GetEnableDOF();
		case EFFECT_COLORDODGE:
			if(flag)
				return !environment->PEManager()->isActive(PE_COLOR_DODGE);
			else 
				return environment->PEManager()->isActive(PE_COLOR_DODGE);

	}
	return false;
}

void ActionSetEffect::activate()
{
	bool flag = switchMode == ON ? true : false;
	switch(effects) {
		case EFFECT_BLOOM:
			environment->PEManager()->setActive(PE_BLOOM,flag);
			break;
		case EFFECT_MONOCHROME:
			environment->PEManager()->setActive(PE_MONOCHROME,flag);
			break;
		case EFFECT_UNDER_WATER:
			environment->PEManager()->setActive(PE_UNDER_WATER,flag);
			break;
		case EFFECT_DOF:
			gb_VisGeneric->SetEnableDOF(flag);
			break;
		case EFFECT_COLORDODGE:
			environment->PEManager()->setActive(PE_COLOR_DODGE,flag);
	}
}

ActionActivateSources::ActionActivateSources() 
{
	active_ = true;
}

void ActionActivateSources::serialize(Archive& ar) 
{
	__super::serialize(ar);

	ar.serialize(sources_, "sources_", "sources_");
}	

bool ActionKillSource::automaticCondition() const
{
	return source_;
}

void ActionKillSource::activate() 
{
		if(source_)
			source_->kill();
}

void ActionKillSource::serialize(Archive& ar) 
{
	__super::serialize(ar);
	ar.serialize(source_, "source_", "��������");
}

void ActionActivateSources::activate() 
{
	Sources::iterator i;
	FOR_EACH(sources_, i){
		if(!*i){
			xassertStr(0 && "�������� �� ����� �� ������: ", i->c_str());
		}
		else
			(*i)->setActivity(active_);
	}
}

ActionDeactivateSources::ActionDeactivateSources()
{
	active_ = false;
}

/////////////////////////////////////////////////////
void ActionActivateMinimapMark::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(anchor_, "|anchor|anchorName", "��� �������");
}

void ActionActivateMinimapMark::activate()
{
	if(anchor_)
		anchor_->setSelected(active_);
}
/////////////////////////////////////////////////////

ActionSetFallout::ActionSetFallout()
{
	type = NONE;
	intencity = 0.5;
	time = 5;
	rainConstant = 0;
}

void ActionSetFallout::activate() 
{
	cFallout& fallout = *environment->fallout();
	cWater& water = *environment->water();
	switch(type){
	case NONE:
		fallout.Set(FALLOUT_CURRENT,0, time);
		//fallout.End(time);
		water.SetRainConstant(-rainConstant);
		break;
	case RAIN:
		fallout.Set(FALLOUT_RAIN,intencity / 100.f, time);
		water.SetRainConstant(-rainConstant);
		break;
	case SNOW:
		fallout.Set(FALLOUT_SNOW,intencity / 100.f, time);
		break;
	}
}

void ActionSetFallout::serialize(Archive& ar) 
{
	__super::serialize(ar);
	ar.serialize(type, "type", "��� �������");
	ar.serialize(RangedWrapperf(intencity, 0, 100, 0.01f), "intensity", "������������� (0-100)");
	ar.serialize(time, "time", "����� ��������");
	ar.serialize(rainConstant, "rainConstant", "����: �������� ���������");
}

ActionSetFalloutType::ActionSetFalloutType()
{
	type = ActionSetFallout::NONE;
	intencity = 50;
	time = 5;
}

void ActionSetFalloutType::activate() 
{
	cFallout& fallout = *environment->fallout();
	switch(type){
	case ActionSetFallout::NONE:
		fallout.Set(FALLOUT_CURRENT,0, time);
		break;
	case ActionSetFallout::RAIN:
		fallout.Set(FALLOUT_RAIN,intencity / 100.f, time);
		break;
	case ActionSetFallout::SNOW:
		fallout.Set(FALLOUT_SNOW,intencity / 100.f, time);
		break;
	}
}

void ActionSetFalloutType::serialize(Archive& ar) 
{
	__super::serialize(ar);
	ar.serialize(type, "type", "��� �������");
	ar.serialize(RangedWrapperf(intencity, 0, 100, 0.01f), "intensity", "������������� (0-100)");
	ar.serialize(time, "time", "����� ��������");
}

ActionSetFalloutFlood::ActionSetFalloutFlood()
{
	rainConstant = 0;
}

void ActionSetFalloutFlood::activate() 
{
	cWater& water = *environment->water();
	water.SetRainConstant(-rainConstant);
}

void ActionSetFalloutFlood::serialize(Archive& ar) 
{
	__super::serialize(ar);
	ar.serialize(rainConstant, "rainConstant", "����: �������� ���������");
}
/////////////////////////////////////////////////////

ActionSetEnvironmentTime::ActionSetEnvironmentTime()
{
	time = 12;
}

void ActionSetEnvironmentTime::activate() 
{
	environment->environmentTime()->SetTime(time);
}

void ActionSetEnvironmentTime::serialize(Archive& ar) 
{
	__super::serialize(ar);
	ar.serialize(time, "time", "����� �����");
}

////////////////////////////////////////////////////

ActionSetFog::ActionSetFog()
{
	fog_enable = false;
	fog_start = 300;
	fog_end = 3000;
}

void ActionSetFog::activate()
{
	environment->setFogEnabled(fog_enable);
	environment->setFogStart(fog_start);
	environment->setFogEnd(fog_end);
}

void ActionSetFog::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(fog_enable, "fog_enable", "�������� �����");
	ar.serialize(fog_start, "fog_start", "������� ������� ������");
	ar.serialize(fog_end, "fog_end", "������� ������� ������");
}

////////////////////////////////////////////////////
ActionSetFogOfWar::ActionSetFogOfWar()
{
	mode_ = MODE_OFF;
}

void ActionSetFogOfWar::activate()
{
	switch(mode_){
		case MODE_OFF:
			universe()->setShowFogOfWar(false);
			break;
		case MODE_ON:
			universe()->setShowFogOfWar(true);	
			break;
		case MODE_RESTORE:
			universe()->setShowFogOfWar(!debugDisableFogOfWar);	
	}
}

ActionSetWaterOpacity::ActionSetWaterOpacity()
{
	if(environment){
		cWater* water = environment->water(); 
		if(water)
			opacityGradient_ = water->GetOpacity();
	}
}

void ActionSetWaterOpacity::serialize(Archive& ar)
{
	__super::serialize(ar);
	if(environment){
		cWater* water = environment->water(); 
	}
	ar.serialize(static_cast<WaterGradient&>(opacityGradient_), "zLevelOpacityGradient", "������������ ���� �� ������ �������");
}

void ActionSetWaterOpacity::activate()
{
	if(environment){
		cWater* water = environment->water(); 
		if(water)
			water->SetOpacity(opacityGradient_);
	}
}

ActionSetReflectSkyColor::ActionSetReflectSkyColor()
{
	reflectSkyColor_.set(255,255,255,0);
	switchMode_ = ON;
}

void ActionSetReflectSkyColor::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(reflectSkyColor_, "reflectSkyColor", "����");
	ar.serialize(switchMode_, "switchMode", "��������");
}

void ActionSetReflectSkyColor::activate()
{
	if(environment){
		EnvironmentTime* envTime = environment->environmentTime();
		if(envTime){
			envTime->setEnableChangeReflectSkyColor( switchMode_ == ON ? false : true);
			if(switchMode_ == ON)
				envTime->SetCurReflectSkyColor(reflectSkyColor_);
		}			
	}
}

void ActionSetFogOfWar::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(mode_, "mode", "��������");
}

ActionSetWaterColor::ActionSetWaterColor()
: riverColor_(0, 0, 0, 1), seaColor_(0, 0, 0, 1)
{
	transitionTime_ = 5;
}

void ActionSetWaterColor::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(riverColor_, "riverColor", "���� ���������� ����");
	ar.serialize(seaColor_, "seaColor", "���� �������� ����");
	ar.serialize(transitionTime_, "transitionTime", "����� ��������, �������");
}

void ActionSetWaterColor::activate()
{
//	environment->water()->GetColor(riverColorPrev_, seaColorPrev_);
	transitionTimer_.start(round(transitionTime_*1000));
}

bool ActionSetWaterColor::workedOut()
{
	float t = transitionTimer_.factor();
	Color4c riverColor, seaColor;
	riverColor.interpolate(riverColorPrev_, riverColor_, t);
	seaColor.interpolate(seaColorPrev_, seaColor_, t);
//	environment->water()->SetColor(riverColor, seaColor);
	return t > 1.f - FLT_EPS;
}
