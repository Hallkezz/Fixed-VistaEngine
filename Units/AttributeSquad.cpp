#include "StdAfx.h"
#include "AttributeSquad.h"
#include "Serialization.h"
#include "TypeLibraryImpl.h"

WRAP_LIBRARY(FormationPatterns, "FormationPattern", "�������� ��������", "Scripts\\Content\\FormationPattern", 0, true);

WRAP_LIBRARY(AttributeSquadTable, "AttributeSquad", "���� �������", "Scripts\\Content\\AttributeSquad", 0, false);

///////////////////////////////////////////////////

void FormationPattern::Cell::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(type, "type", "���");
}

void FormationPattern::serialize(Archive& ar)
{
	StringTableBase::serialize(ar); 
	ar.serialize(cells_, "cells", "������");
}

///////////////////////////////////////////////////

AttributeSquad::Formation::Formation() 
: rotateFront(true), attackByRadius(true), attackRadius(100), hardFormation(false), correctSpeed(false)
{
}

void AttributeSquad::Formation::serialize(Archive& ar)
{
 	ar.serialize(formationPattern, "formationPattern", "�������");
 	ar.serialize(rotateFront, "rotateFront", "������������ �����");
	ar.serialize(hardFormation, "hardFormation", "������� ��������");
 	ar.serialize(attackByRadius, "attackByRadius", "��������� �� ��������� �������");
	ar.serialize(correctSpeed, "correctSpeed", "�������������� �������� ������");
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
}

void AttributeSquad::serialize(Archive& ar) 
{
	StringTableBase::serialize(ar);
	ar.serialize(numbers, "numbers", "���������� ������");
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
		if(numbers.empty())
			numbers.push_back(UnitNumber());
	}

//	ar.serialize(homePositionOffsetFactor, "homePositionOffsetFactor", "homePositionOffsetFactor");
//	ar.serialize(formationRadiusBase, "formationRadiusBase", "formationRadiusBase");
//	ar.serialize(followDistanceFactor, "followDistanceFactor", "followDistanceFactor");
}

