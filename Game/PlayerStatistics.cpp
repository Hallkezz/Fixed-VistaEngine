#include "StdAfx.h"
#include "PlayerStatistics.h"
#include "Serialization\Serialization.h"
#include "EventParameters.h"
#include "Player.h"
#include "GlobalAttributes.h"

BEGIN_ENUM_DESCRIPTOR(StatisticType, "��� ����������")
REGISTER_ENUM(STAT_UNIT_MY_KILLED,"���-�� ����� ������ ������")
REGISTER_ENUM(STAT_BUILDING_MY_KILLED, "���-�� ����� ������ ������") 
REGISTER_ENUM(STAT_OBJECTS_MY_KILLED, "���-�� ����� ������ ������ � ������") 
REGISTER_ENUM(STAT_UNIT_ENEMY_KILLED, "���-�� ��������� ������ ������")
REGISTER_ENUM(STAT_BUILDING_ENEMY_KILLED, "���-�� ��������� ������ ������")
REGISTER_ENUM(STAT_OBJECTS_ENEMY_KILLED, "���-�� ��������� ������ ������ � ������")
REGISTER_ENUM(STAT_UNIT_ENEMY_CAPTURED, "���-�� ����������� ��������� ������")
REGISTER_ENUM(STAT_BUILDING_ENEMY_CAPTURED, "���-�� ����������� ��������� ������")
REGISTER_ENUM(STAT_OBJECTS_ENEMY_CAPTURED, "���-�� ����������� ��������� ������ � ������")
REGISTER_ENUM(STAT_UNIT_MY_CAPTURED, "���-�� ����� ����������� ������")
REGISTER_ENUM(STAT_BUILDING_MY_CAPTURED, "���-�� ����� ����������� ������")
REGISTER_ENUM(STAT_OBJECTS_MY_CAPTURED, "���-�� ����� ����������� ������ � ������")
REGISTER_ENUM(STAT_UNIT_MY_BUILT, "���-�� ����� ����������� ������")
REGISTER_ENUM(STAT_BUILDING_MY_BUILT, "���-�� ����� ����������� ������")
REGISTER_ENUM(STAT_OBJECTS_MY_BUILT, "���-�� ����� ����������� ������ � ������")
REGISTER_ENUM(STAT_KEYPOINTS_CAPTURED, "���-�� ����������� �������� �����")
REGISTER_ENUM(STAT_KEYPOINTS_LOST, "���-�� ���������� �������� �����")
REGISTER_ENUM(STAT_HEROES_ENEMY_KILLED, "���-�� ��������� ������ ������")
REGISTER_ENUM(STAT_HEROES_MY_KILLED, "���-�� ���� ������ ������")
REGISTER_ENUM(STAT_RESOURCES_COLLECTED, "����� ��������� ��������")
REGISTER_ENUM(STAT_RESOURCES_SPENT, "����� ����������� ��������")
REGISTER_ENUM(STAT_OBJECTS_MY_KILLED_RESOURCES, "��������� ���� ���� ������ ������")
REGISTER_ENUM(STAT_OBJECTS_ENEMY_KILLED_RESOURCES, "��������� ���� ��������� ������ ������")
REGISTER_ENUM(STAT_PICKED_ITEMS, "���������� ��������� ���������")
REGISTER_ENUM(TOTAL_TIME_PLAYED, "����� ����� ����")
REGISTER_ENUM(TOTAL_GAMES_PLAYED, "���������� ��������� ���")
REGISTER_ENUM(TOTAL_WINS, "����� �����")
REGISTER_ENUM(TOTAL_LOSSES, "����� ���������")
REGISTER_ENUM(TOTAL_DISCONNECTIONS, "����� ������� ����������")
REGISTER_ENUM(STAT_RATING, "�������")
END_ENUM_DESCRIPTOR(StatisticType)
 
PlayerStatistics::PlayerStatistics()
{
	finalized_ = false;
	for(int i = 0; i < STAT_LAST_ENUM; i++)
		statistics_[i] = 0;
}

int PlayerStatistics::operator[](StatisticType type) const
{
	return (type == STAT_RESOURCES_COLLECTED || type == STAT_RESOURCES_SPENT) ? statistics_[type] / 100 : statistics_[type];
}

void PlayerStatistics::registerEvent(const Event& event, Player* player)
{
	if(finalized_)
		return;

	switch (event.type()) {
		case Event::CREATE_OBJECT:
		case Event::PRODUCE_UNIT_IN_SQUAD:
			{
				const EventUnitPlayer& eventUnit = safe_cast_ref<const EventUnitPlayer&>(event);
				if(eventUnit.unit()->player() == player)
					if(eventUnit.unit()->attr().isObjective() && eventUnit.unit()->attr().isLegionary()){
						statistics_[STAT_UNIT_MY_BUILT]++; // ���������� ��� ���� (�� ������!)
						statistics_[STAT_OBJECTS_MY_BUILT]++; // ���������� ��� ���� (�� ������!)
					}
			}
			break;
		case Event::KILL_OBJECT:
			{
				const EventUnitMyUnitEnemy& eventUnit = safe_cast_ref<const EventUnitMyUnitEnemy&>(event);
				const UnitBase* unitMy = eventUnit.unitMy();
				if(unitMy->attr().excludeFromAutoAttack)
					return;
				if(unitMy)
					if(unitMy->attr().isPad())
						return;
					if(eventUnit.unitMy()->player() == player){ 
						statistics_[STAT_OBJECTS_MY_KILLED]++; 
						statistics_[STAT_OBJECTS_MY_KILLED_RESOURCES] += unitMy->attr().installValue.dot(GlobalAttributes::instance().resourseStatisticsFactors) 
							+ unitMy->attr().creationValue.dot(GlobalAttributes::instance().resourseStatisticsFactors);
						if(unitMy->attr().isBuilding()){
							statistics_[STAT_BUILDING_MY_KILLED]++; // ��� ������ �����
						}
						else if(unitMy->attr().isLegionary()){
							statistics_[STAT_UNIT_MY_KILLED]++; // ����� ����� �����
							if(unitMy->attr().isHero){
								statistics_[STAT_HEROES_MY_KILLED]++;
							}
						}
					}
				if(const UnitBase* unitEnemy = eventUnit.unitEnemy()){
					if(unitEnemy->attr().excludeFromAutoAttack)
						return;
					if(unitEnemy->player() == player){
						if(unitEnemy->attr().isPad())
							return;
						statistics_[STAT_OBJECTS_ENEMY_KILLED]++; 
						statistics_[STAT_OBJECTS_ENEMY_KILLED_RESOURCES] += unitMy->attr().installValue.dot(GlobalAttributes::instance().resourseStatisticsFactors) 
							+ unitMy->attr().creationValue.dot(GlobalAttributes::instance().resourseStatisticsFactors);
						if(unitMy->attr().isBuilding()){
							statistics_[STAT_BUILDING_ENEMY_KILLED]++; // � ���� ��������� ������ 
						}
						else if(unitMy->attr().isLegionary()){
							statistics_[STAT_UNIT_ENEMY_KILLED]++; // � ���� ���������� �����
							if(unitMy->attr().isHero){
								statistics_[STAT_HEROES_ENEMY_KILLED]++;
							}
						}
					}
				}
			}
			break;
		case Event::COMPLETE_BUILDING:
			{
				const EventUnitPlayer& eventUnit = safe_cast_ref<const EventUnitPlayer&>(event);
				if (eventUnit.unit() && eventUnit.unit()->player() == player && eventUnit.unit()->attr().isBuilding()) {
					statistics_[STAT_BUILDING_MY_BUILT]++; // ����������� ��� ������
					statistics_[STAT_OBJECTS_MY_BUILT]++; 
				}
			}
			break;
		case Event::COMPLETE_UPGRADE:
			//{
			//	const EventUnitUnitAttributePlayer& eventUnit = safe_cast_ref<const EventUnitUnitAttributePlayer&>(event);
			//	if (eventUnit.unit() && eventUnit.player() == player && eventUnit.unit()->attr().isBuilding()) {
			//		statistics_[STAT_BUILDING_MY_BUILT]++; // ����������� ��� ������
			//		statistics_[STAT_OBJECTS_MY_BUILT]++; 
			//	}
			//}
			break;
		case Event::CAPTURE_UNIT:
			{
				const EventUnitMyUnitEnemy& eventUnit = safe_cast_ref<const EventUnitMyUnitEnemy&>(event);
				if(eventUnit.unitMy())
					if (eventUnit.unitMy()->player() == player){ 
						statistics_[STAT_OBJECTS_MY_CAPTURED]++;
						if(eventUnit.unitMy()->attr().isBuilding()){
							statistics_[STAT_BUILDING_MY_CAPTURED]++; // ��� ������ ���������
						}
						if(eventUnit.unitMy()->attr().isLegionary()){
							statistics_[STAT_UNIT_MY_CAPTURED]++; // ����� ����� ���������
						}
						if(eventUnit.unitMy()->attr().isStrategicPoint){
							statistics_[STAT_KEYPOINTS_LOST]++;
						}
					}
				if(eventUnit.unitEnemy()) 
					if(eventUnit.unitEnemy()->player() == player){
						statistics_[STAT_OBJECTS_ENEMY_CAPTURED]++;
						if(eventUnit.unitMy()->attr().isBuilding()){ 
							statistics_[STAT_BUILDING_ENEMY_CAPTURED]++; // � �������� ��������� ������
						}	
						if(eventUnit.unitMy()->attr().isLegionary()){
							statistics_[STAT_UNIT_ENEMY_CAPTURED]++; // � �������� ���������� �����
						}
						if(eventUnit.unitMy()->attr().isStrategicPoint){
							statistics_[STAT_KEYPOINTS_CAPTURED]++;
						}
					}
			}
			break;
		case Event::ADD_RESOURCE:
			{
				const EventParameters& eventParameters = safe_cast_ref<const EventParameters&>(event);
				statistics_[STAT_RESOURCES_COLLECTED] += 100 * eventParameters.parameters().dot(GlobalAttributes::instance().resourseStatisticsFactors);
			}
			break;
		case Event::SUB_RESOURCE:
			{
				const EventParameters& eventParameters = safe_cast_ref<const EventParameters&>(event);
				statistics_[STAT_RESOURCES_SPENT] += 100 * eventParameters.parameters().dot(GlobalAttributes::instance().resourseStatisticsFactors);
			}
			break;
		case Event::PICK_ITEM:
			statistics_[STAT_PICKED_ITEMS]++;
			break;

	}
}

void PlayerStatistics::finalize(bool win)
{
	finalized_ = true;
	statistics_[TOTAL_TIME_PLAYED] += global_time()/1000;
	statistics_[TOTAL_WINS] = win ? 1 : 0;
	statistics_[TOTAL_LOSSES] = win ? 0 : 1;
}

void PlayerStatistics::serialize(Archive& ar)
{
	ar.serialize(finalized_, "finalized", 0);
	ar.serializeArray(statistics_, "statistics", 0);
}

void PlayerStatistics::showDebugInfo()
{
	XBuffer msg(2048, 1);
	for(int i = 0; i < STAT_LAST_ENUM; i++)
		msg < getEnumNameAlt(StatisticType(i)) < ": " <= statistics_[i] < "\n";
	show_text2d(Vect2f(10, 150), msg, Color4c::WHITE);
}


