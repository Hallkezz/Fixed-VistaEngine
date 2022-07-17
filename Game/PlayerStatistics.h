#ifndef __PLAYER_STATISTICS_H__
#define __PLAYER_STATISTICS_H__

class Player;
class UnitBase;
class Scores;
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

	void set(const Scores& scores);
	void get(Scores& scores) const;

	int operator[](StatisticType type) const;
	void registerEvent(const Event& event, Player* player);
	void finalize(bool win);
	void serialize(Archive& ar);

	void showDebugInfo();

private:

	int statistics_[STAT_LAST_ENUM];
	bool finalized_;
	
};

#endif /* __PLAYER_STATISTICS_H__ */
