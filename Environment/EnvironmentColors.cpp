#include "StdAfx.h"
#include "EnvironmentColors.h"
#include "Serialization.h"
#include "RangedWrapper.h"

EnvironmentAttributes::EnvironmentAttributes()
{
	fogOfWarColor_.set(0, 0, 0, 200);
	scout_area_alpha_ = 220;
	fogMinimapAlpha_ = 255;
	fog_start_ = 900.f;
	fog_end_ = 1100.f;
	game_frustrum_z_min_ = 5.f;
	game_frustrum_z_max_vertical_ = 4000.f;
	game_frustrum_z_max_horizontal_ = 4000.f;
	hideByDistanceFactor_ = 40.f; 
	hideByDistanceRange_ = 100.f;
	hideSmoothly_ = false;
	effectHideByDistance_ = false;
	effectNearDistance_ = 0.f;
	effectFarDistance_ = 100000.f;
	dayTimeScale_ = 500.f;
	nightTimeScale_ = 1000.f;
	rainConstant_ = 1.f;
	water_dampf_k_ = 7;
	height_fog_circle_ = 500;
	miniDetailTexResolution_ = 4;
}

void EnvironmentAttributes::serialize(Archive& ar)
{
	ar.openBlock("Fog of war", "����� �����");
		ar.serialize(fogOfWarColor_, "fogOfWarColor", "���� ������ �����");
		fogOfWarColor_.a = clamp(fogOfWarColor_.a , 100, 255);
		ar.serialize(scout_area_alpha_, "scout_area_alpha", "������������ ������������");
		scout_area_alpha_ = clamp(scout_area_alpha_, 100, 255);
		ar.serialize(RangedWrapperi(fogMinimapAlpha_, 100, 255), "fogMinimapAlpha", "������������ ������ ����� �� ���������");
	ar.closeBlock();

	ar.openBlock("Environment fog", "����� �� ����");
		ar.serialize(fog_start_, "fog_start", "������� ������� ������");
		ar.serialize(fog_end_, "fog_end", "������� ������� ������");
		ar.serialize(RangedWrapperi(height_fog_circle_, 0, 2000), "height_fog_circle", "������ �������� � ������");
	ar.closeBlock();
	
	ar.openBlock("Efects", "�������");
		ar.serialize(effectHideByDistance_, "effectHideByDistance", "�������� ������� ��� ��������");
		ar.serialize(effectNearDistance_, "effectNearDistance", "������� ������� ��������");
		ar.serialize(effectFarDistance_, "effectFarDistance", "������� ������� ��������");
	ar.closeBlock();

	ar.openBlock("Camera frustrum", "������� �����");
		ar.serialize(RangedWrapperf(game_frustrum_z_min_, 1.0f, 100.0f), "game_frustrum_z_min", "������� ������� ������");
		ar.serialize(RangedWrapperf(game_frustrum_z_max_vertical_, 100.0f, 13000.0f), "game_frustrum_z_max", "������� ������� ������ (� ������������ ���������)");
		if(ar.isInput())
			game_frustrum_z_max_horizontal_ = game_frustrum_z_max_vertical_;
		ar.serialize(RangedWrapperf(game_frustrum_z_max_horizontal_, 100.0f, 13000.0f), "game_frustrum_z_max_horizontal", "������� ������� ������ (� �������������� ���������)");
		//ar.serialize(hideByDistanceFactor_, "hideByDistanceFactor", "�������� ��� �������� (������)");
		//ar.serialize(hideByDistanceRange_, "hideByDistanceRange", "�������� ��� �������� (��������)");
		ar.serialize(hideSmoothly_, "hideSmoothly", "�������� ������");
	ar.closeBlock();

	ar.openBlock("Time scale", "������� ������� �����");
		ar.serialize(dayTimeScale_, "dayTimeScale", "������� ������� ����");
		ar.serialize(nightTimeScale_, "nightTimeScale", "������� ������� �����");
	ar.closeBlock();

	ar.openBlock("Water", "����");
		ar.serialize(rainConstant_, "rainConstant", "�������� ���������");
		ar.serialize(RangedWrapperi(water_dampf_k_, 1, 15), "water_dampf_k", "�������� �������");
	ar.closeBlock();

	ar.serialize(RangedWrapperi(miniDetailTexResolution_, 1, 5), "miniDetailTexResolution", "���������� �������������� ��������");

	ar.serialize(timeColors_,"timeColors_","���� �������");
	ar.serialize(coastSprites_,"coastSprites","���������� �������");
	ar.serialize(fallout_,"fallout","������");
	ar.serialize(waterPlume_,"waterPlume","����� �� ����");
	ar.serialize(windMap_,"windMap","�����");
}
