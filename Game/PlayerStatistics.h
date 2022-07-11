#ifndef __PLAYER_STATISTICS_H__
#define __PLAYER_STATISTICS_H__
#include "DebugUtil.h"

class Player;
class UnitBase;
class Event;
struct RatingFactors;

enum StatisticType {
	TOTAL_TIME_PLAYED,
	TOTAL_GAMES_PLAYED,
	TOTAL_WINS, 
	TOTAL_LOSSES, 
	TOTAL_DISCONNECTIONS,

	STAT_UNIT_MY_KILLED, // ���������� !�����! ������ ������
	STAT_BUILDING_MY_KILLED, // ���������� !�����! ������ ������ 
	STAT_OBJECTS_MY_KILLED,
	
	STAT_UNIT_ENEMY_KILLED, // ���������� !���������! ������ ������ ����
	STAT_BUILDING_ENEMY_KILLED, // ���������� !���������! ������ ������ ����
	STAT_OBJECTS_ENEMY_KILLED,
	
	STAT_UNIT_ENEMY_CAPTURED, // ���������� ����������� !���������! ������ ����
	STAT_BUILDING_ENEMY_CAPTURED, // ���������� ����������� !���������! ������ ����
	STAT_OBJECTS_ENEMY_CAPTURED,
	
	STAT_UNIT_MY_CAPTURED, // ���������� !�����! ����������� ������
	STAT_BUILDING_MY_CAPTURED, // ��������� !�����! ����������� ������
	STAT_OBJECTS_MY_CAPTURED, 
	
	STAT_UNIT_MY_BUILT, // ���������� !�����! ����������� ������
	STAT_BUILDING_MY_BUILT, // ���������� !�����! ����������� ������
	STAT_OBJECTS_MY_BUILT, 
	
	STAT_KEYPOINTS_CAPTURED, // ���������� ����������� �������� ����� ����
	STAT_KEYPOINTS_LOST, // 

	STAT_HEROES_ENEMY_KILLED, // 
	STAT_HEROES_MY_KILLED, // 

	STAT_RESOURCES_COLLECTED, // 
	STAT_RESOURCES_SPENT, //

	STAT_OBJECTS_MY_KILLED_RESOURCES,
	STAT_OBJECTS_ENEMY_KILLED_RESOURCES,

	STAT_RATING,

	STAT_LAST_ENUM //������ enum, ������� � �����
};

class PlayerStatistics
{
public:
	PlayerStatistics();

	void set();
	void get() const;

	int operator[](StatisticType type) const;
	void registerEvent(const Event& event, Player* player);
	void finalize(bool win);
	void serialize(Archive& ar);

	void showDebugInfo();
	void writeWorldInfo(const char* title);

private:
	void outLogOut(const Player* player, const UnitBase* unit, StatisticType stType);

	int statistics_[STAT_LAST_ENUM];
	bool finalized_;
	
	static LogStream outLogStat;
	static string title_;
};

#endif /* __PLAYER_STATISTICS_H__ */
