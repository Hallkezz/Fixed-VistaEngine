#ifndef __WEAPON_TARGET_H__
#define __WEAPON_TARGET_H__

class UnitInterface;
/// ���� ��� ������, ���� ��� ����������.
class WeaponTarget
{
public:
	WeaponTarget(UnitInterface* unit = 0, int weapon_id = 0);
	WeaponTarget(const Vect3f& position, int weapon_id = 0);
	WeaponTarget(UnitInterface* unit, const Vect3f& position, int weapon_id = 0);

	UnitInterface* unit() const { return unit_; }
	const Vect3f& position() const { return position_; }
	void setPosition(const Vect3f& pos){ position_ = pos; }

	bool valid() const { return terrainAttackClass_ != -1 || unit(); } 

	int terrainAttackClass() const { /*xassert(terrainAttackClass_ != -1);*/ return terrainAttackClass_; }
	void updateTerrainAttackClass();

	int attackClass() const;

	int weaponID() const { return weaponID_; }
	void setWeaponID(int weapon_id){ weaponID_ = weapon_id; }

private:

	/// ����-����
	UnitLink<UnitInterface> unit_;
	/// ���������� ����
	Vect3f position_;

	/// ����� ����������� � ����� ����
	/// ����������� ��� ������ ������ \a terrainAttackClass()
	int terrainAttackClass_;

	/// ������, ��� �������� ������������� ����
	/// ���� ��������, ��� ������� �����, ������� ����� ��������� ��� ����
	int weaponID_;
};

#endif /* __WEAPON_TARGET_H__ */
