#ifndef __WEAPON_ENUMS_H__
#define __WEAPON_ENUMS_H__

/// ������ ����������� ������
enum AffectMode
{
	/// ������ �� ����� ������
	AFFECT_OWN_UNITS,
	/// �� ����� � ������� ������
	AFFECT_FRIENDLY_UNITS,
	/// ������� ������
	AFFECT_ALLIED_UNITS,
	/// �� ��������� ������
	AFFECT_ENEMY_UNITS,
	/// �� ���� ������
	AFFECT_ALL_UNITS,
	/// �� ������������ �� �����
	AFFECT_NONE_UNITS
};

/// ������ ��������.
enum WeaponMode
{
	LONG_RANGE,		///< �������� ������� �������� ���
	SHORT_RANGE,	///< �������� ������� �������� ���
	ANY_RANGE,		///< �������� ����� �������
};

/// ����� ������ ������ � ������ ����������.
enum WeaponDirectControlMode
{
	WEAPON_DIRECT_CONTROL_DISABLE,		///< �� �������� ������
	WEAPON_DIRECT_CONTROL_NORMAL,		///< �������� ���� �������
	WEAPON_DIRECT_CONTROL_ALTERNATE,	///< �������� �� ������� �����
	WEAPON_DIRECT_CONTROL_AUTO			///< ������������� �������� ����
};

/// ����� ������ ������ � ������ ����������.
enum WeaponSyndicateControlMode
{
	WEAPON_SYNDICATE_CONTROL_DISABLE,		///< �� �������� ������
	WEAPON_SYNDICATE_CONTROL_NORMAL,		///< �������� ���� �������
	WEAPON_SYNDICATE_CONTROL_FORCE,			///< �������� �� ����� ���� ��������
	WEAPON_SYNDICATE_CONTROL_AUTO			///< ������������� �������� ����
};

/// ��������� ������� �� ����� ������������.
enum WeaponAnimationMode
{
	/// �������� �� �����
	WEAPON_ANIMATION_NONE,
	/// �������� �����������
	WEAPON_ANIMATION_RELOAD,
	/// �������� ������������
	WEAPON_ANIMATION_AIM,
	/// �������� ��������
	WEAPON_ANIMATION_FIRE
};

/// ����� �������� ���������� �� ������/��������.
enum WeaponSourcesCreationMode
{
	WEAPON_SOURCES_CREATE_ON_TARGET_HIT,	///< ��� ��������� � ����
	WEAPON_SOURCES_CREATE_ON_GROUND_HIT,	///< ��� ��������� � �����
	WEAPON_SOURCES_CREATE_ALWAYS			///< ������
};

#endif /* __WEAPON_ENUMS_H__ */
