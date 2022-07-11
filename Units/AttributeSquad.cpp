#include "StdAfx.h"
#include "AttributeSquad.h"
#include "IronLegion.h"
#include "Serialization\Serialization.h"
#include "Serialization\SerializationFactory.h"
#include "Serialization\StringTableImpl.h"

WRAP_LIBRARY(FormationPatterns, "FormationPattern", "�������� ��������", "Scripts\\Content\\FormationPattern", 0, LIBRARY_EDITABLE);

WRAP_LIBRARY(AttributeSquadTable, "AttributeSquad", "���� �������", "Scripts\\Content\\AttributeSquad", 0, LIBRARY_EDITABLE | LIBRARY_IN_PLACE);

BEGIN_ENUM_DESCRIPTOR(VelocityCorrection, "VelocityCorrection");
REGISTER_ENUM(VELOCITY_DEFAULT, "�� ��������������");
REGISTER_ENUM(VELOCITY_MIN, "�� �����������");
REGISTER_ENUM(VELOCITY_AVERAGE, "�� �������");
REGISTER_ENUM(VELOCITY_MAX, "�� ������������");
END_ENUM_DESCRIPTOR(VelocityCorrection);

///////////////////////////////////////////////////

void FormationPattern::Cell::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(type, "type", "^");
}

void FormationPattern::serialize(Archive& ar)
{
	StringTableBase::serialize(ar); 
	ar.serialize(cells_, "cells", "������");
}

///////////////////////////////////////////////////

AttributeSquad::Formation::Formation() 
: rotateFront(true), attackByRadius(true), attackRadius(100), uniformFormation(true), velocityCorrection(VELOCITY_DEFAULT)
{
}

void AttributeSquad::Formation::serialize(Archive& ar)
{
 	ar.serialize(formationPattern, "formationPattern", "^");
 	ar.serialize(rotateFront, "rotateFront", "������������ �����");
	ar.serialize(uniformFormation, "uniformFormation", "���������� ��������");
 	ar.serialize(attackByRadius, "attackByRadius", "��������� �� ��������� �������");
	if(!ar.serialize(velocityCorrection, "velocityCorrection", "�������������� �������� ������")){
		bool correctSpeed = false;
		ar.serialize(correctSpeed, "correctSpeed", "�������������� �������� ������");
		if(correctSpeed)
			velocityCorrection = VELOCITY_AVERAGE;
	}
	if(!ar.isEdit() || attackByRadius)
 		ar.serialize(attackRadius, "attackRadius", "������ �����");
}

AttributeSquad::AttributeSquad(const char* name) 
: StringTableBase(name)
{
	homePositionOffsetFactor.set(1.2f, 0);
	formationRadiusBase = 10;
	followDistanceFactor = 1; // followDistanceFactor*(radius1 + radius2)
	unitClass_ = UNIT_CLASS_SQUAD;
	internal = true;
	enableJoin = true;
	automaticJoin = false;
	automaticJoinRadius = 100;
	automaticJoinRadiusEffect.color.set(255, 255, 255, 0);
	joinRadius = 100;
	joinRadiusEffect.color.set(255, 255, 255, 0);
	disableMainUnitAutoAttack = false;
	forceUnitsAutoAttack = false;
	followMainUnitInAutoMode = false;
	showSightSector = true;
	showSelectRadius = false;
	accountingNumber = 0;
	showWayPoint = true;
	showAllWayPoints = false;
	showAllWayPointDist = 15.f;
}

void AttributeSquad::serialize(Archive& ar) 
{
	if(ar.isOutput() && !ar.isEdit()){	// ������� ������� - ������ ����� �������
		allowedUnitsAttributes.clear();
		AttributeLibrary::Map::const_iterator mi;
		FOR_EACH(AttributeLibrary::instance().map(), mi){
			const AttributeBase* attribute = mi->get();
			if(attribute && attribute->isLegionary()){
				const AttributeLegionary* legionary = safe_cast<const AttributeLegionary*>(attribute);
				if(legionary->squad == this)
					allowedUnitsAttributes.add(legionary);
			}
		}
	}

	StringTableBase::serialize(ar);
	ar.serialize(parametersInitial, "parametersInitial", "������ (���������) ��������� ������");
	ar.serialize(formations, "formations", "��������");
	ar.serialize(enableJoin, "enableJoin", "��������� ����������� �������");
	ar.serialize(automaticJoin, "automaticJoin", "������������� ������������ ������");
	if(automaticJoin){
		ar.serialize(automaticJoinRadius, "automaticJoinRadius", "������ ��������������� �������������");
		ar.serialize(automaticJoinRadiusEffect, "automaticJoinRadiusEffect", "������������ ������� ��������������� �������������");
	}
	ar.serialize(joinRadius, "joinRadius", "������������ ������ ����������� �������");
	ar.serialize(joinRadiusEffect, "joinRadiusEffect", "������������ ������������� ������� ����������� �������");

	if(ar.isInput()){
		internal = true;
		if(formations.empty())
			formations.push_back(Formation());
	}

	ar.serialize(accountingNumber, "accountingNumber", "�����, ����������� � ������������ ���������� ������");
	ar.serialize(unitNumberMaxType, "unitNumberMaxType", "��� ������������� ���������� ������");
	ar.serialize(allowedUnits, "allowedUnits", "������ ���������� ����� ������");

	ar.serialize(disableMainUnitAutoAttack, "disableMainUnitAutoAttack", "��������� �������������� ����� � �������� �����");
	ar.serialize(forceUnitsAutoAttack, "forceUnitsAutoAttack", "�� ������� ����� ������� ������ �������������");

	ar.serialize(followMainUnitInAutoMode, "followMainUnitInAutoMode", "��������� �� ������� � �������������� ������");

	ar.serialize(permanentEffects, "permanentEffects", "���������� �������");
	ar.serialize(mainUnitEffect, "mainUnitEffect", "������ ��� �������� ����� � ������");
	ar.serialize(waitingUnitEffect, "waitingUnitEffect", "������ ��� ������� ����� � ������");
	ar.serialize(showSightSector, "showSightSector", "���������� ������ ������");

	if(ar.openBlock("Interface", "���������")){
		ar.serialize(selectSprites_, "Miniatures", "���������");

		ar.serialize(selectionCursor_, "selection_cursor", "������ ������");
		selectionCursorProxy_ = selectionCursor_;

		ar.serialize(initialHeightUIParam, "initialHeightUIParam", "������ ����� ��� ������ ��������");

		if(ar.openBlock("squadSign", "���� ������")){
			ar.serialize(showSpriteForUnvisible, "showSpriteForUnvisible", "�������� ����� ����� ������ �� �����");
			ar.serialize(selectBySprite, "selectBySprite", "��������� �� �����");
			ar.serialize(selectSprites, "selectSprites", "��������� �������");
			ar.closeBlock();
		}

		if(ar.openBlock("minimap", "����������� �� ���������")){
			ar.serialize(minimapSymbolType_, "symbolType", "��� �������");
			if(minimapSymbolType_ == UI_MINIMAP_SYMBOLTYPE_SELF){
				ar.serialize(minimapSymbol_, "minimapSymbol", "�� ����������");
				minimapSymbol_.scaleByEvent = false;
				ar.serialize(minimapPermanentSymbol_, "minimapSymbolSelected", "����������");
				minimapPermanentSymbol_.scaleByEvent = false;
			}
			ar.closeBlock();
		}

		ar.serialize(showWayPoint, "showWayPoint", "���������� ����� ����������");
		ar.serialize(showTriggerWayPoint, "showTriggerWayPoint", "���������� ��� ������� ���� �� ��������");
		ar.serialize(targetPoint, "targetPoint", "����������� ������� ����� ����������");

		ar.serialize(showAllWayPoints, "showAllWayPoints", "���������� ��� ����� ����");
		if(showAllWayPoints)
			ar.serialize(showAllWayPointDist, "showAllWayPointDist", "���������� ����� ���������");

		if(ar.openBlock("selection", "��� �������")){
			ar.serialize(showSelectRadius, "showSelectRadius", "���������� ������");
			ar.serialize(selectRadius, "selectRadius", "������ ������ ��� �������, 0 - �� ��������� �������");
			ar.closeBlock();
		}

		ar.closeBlock();
	}

	ar.serialize(allowedUnitsAttributes, "allowedUnitsAttributes", 0);

//	ar.serialize(homePositionOffsetFactor, "homePositionOffsetFactor", "homePositionOffsetFactor");
//	ar.serialize(formationRadiusBase, "formationRadiusBase", "formationRadiusBase");
//	ar.serialize(followDistanceFactor, "followDistanceFactor", "followDistanceFactor");
}

