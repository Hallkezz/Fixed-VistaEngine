#ifndef __SHILD_SOURCE_H__
#define __SHILD_SOURCE_H__

#include "SourceEffect.h"

class FieldSource;

class SourceShield : public SourceDamage
{
public:
	SourceShield();
	SourceShield(const SourceShield& src);
	~SourceShield();
	SourceType type() const { return SOURCE_SHIELD; }
	SourceBase* clone() const { return new SourceShield(*this); }

	void quant();
	void serialize(Archive& ar);

	bool inZone(const Vect3f& coord) const{
		xxassert(height_ <= radius(), "������ ��������� ������ �� ����� ���� ������ ������� ���������");
		return sqr(position().x - coord.x) + sqr(position().y - coord.y) + sqr(position().z - centerDeep_ - coord.z) < sqr(centerDeep_ + height_);
	}
	
	void setRadius(float _radius);

	void showDebug() const;

	bool canApply(const UnitBase* target) const;

	float height() const { return height_; }
	float sphereRadius() const { return centerDeep_ + height_; }
	Vect3f sphereCenter() const { return Vect3f(position().x, position().y, position().z - centerDeep_); }

	/** ���������� �������� �� ���������� ��������� � �������� ����� � ������� (������������ ����� �����) �������.
	��� ���� ������ ���� ����� ������������� */
	static bool traceShieldsCoherence(const Vect3f& point_start, const Vect3f& point_finish, const Player* owner, Vect3f* intersect = 0);
	//! ����������� ������� ���� ����� �������
	static bool traceShieldsThrough(const Vect3f& point_start, const Vect3f& point_finish, const Player* owner, Vect3f* intersect = 0, Vect3f* sphereCenter = 0);
	//! ����������� ������� ���� ����� �������, ��� ���� ��������� ��� ����� ����� � ����������� ���� ������ � ����������� �������� �����
	static bool traceShieldsDelta(const Vect3f& start, const Vect3f& finish, const Player* owner, Vect3f& intersection, Vect3f& center);
	//! ������� ��������������� ������ �� ����������� ������
	static void shieldExplodeEffect(const Vect3f& center, const Vect3f& pos, const EffectAttribute& effect);

protected:
	void start();
	void stop();
	bool killRequest();
private:
	// ������ ������ ��� ������(������� ���������), �� ����� ���� ������ �������
	float height_;
	// �������(��������) ������ �������� ����� �� ������ ���������
	float centerDeep_;

	// ���� ������
	Color4c color_;
	float activateDTime_;
	float activatePhase_;
	float targetZ_;
	bool deactivate_;
	bool activate_;

	LogicTimer pfMapTimer_;

	FieldSource* fieldSource_;
};


#endif //__SHILD_SOURCE_H__
