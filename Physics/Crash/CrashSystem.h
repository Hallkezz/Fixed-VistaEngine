#ifndef __CRASH_SYSTEM_H_INCLUDED__
#define __CRASH_SYSTEM_H_INCLUDED__

class CrashModel3dx;
class CrashSystem;

class CrashBody : public BaseUniverseObject, public RigidBodyBox
{
public:
	CrashBody(cSimply3dx* model, float mass, ExplodeProperty* property_);
	~CrashBody();
	void initPose(const Se3f& pose);
	bool evolve(float dt);
	void createEffect( const EffectAttribute* effect) { effectController.effectStart(effect); }
	void removeEffect() { effectController.release(); }
	void updateRegion(float x1, float y1, float x2, float y2); // ��������� ������� ���� ������ ���������.
	void setOpacity(float opacity);
	cSimply3dx* model() { return model_; }

	friend CrashModel3dx;

private:
	cSimply3dx* model_;
	ExplodeProperty* property;
	EffectController effectController;
};

//=======================================================
// ��������� ������ �� ������� � �������� �� �� �����������.
// ������ �������� � ������ 3DX ������..
//=======================================================
class CrashModel3dx {
	
	friend CrashSystem;
	
	vector<CrashBody*> crashBodyes;
	vector<int> bodyWeights;

	ExplodeProperty property;

	float liveTime_; // �������� ����� �����. ���� 0 �� ���� �������...
	float liveTimeMax_;
	bool enableFantomMode_;
	float explodeFactor_;

public:

	CrashModel3dx(vector<cSimply3dx*>& debrises, const ExplodeProperty& property_, const EffectAttribute* effect, float explodeFactor, float liveTime, bool enableFantomMode = false);
	~CrashModel3dx();

	void setVelocity(Vect3f velocity);
	void addExplode(Vect3f point, float power, const Vect3f& explodeFactors);
	void evolve(float dt);
	bool dead() {return !enableFantomMode_ && liveTime_ < 0; }
	void updateRegion(float x1, float y1, float x2, float y2);
	void showDebugInfo();
};

//=======================================================
// ��������� ��� CrashModel3dx..
//=======================================================
class CrashSystem {
public:
	CrashSystem() {}
	~CrashSystem();
	// � ����������: ����, ����� � �������� �������� ������ ��������������� �� ����.
	// ���� ������������ �����. �� ��������� ��������� ������ ���������� �����.
	void addCrashModel(const DeathAttribute& deathAttribute, const c3dx* model, const Vect3f& point, float power, float liveTime, const Vect3f& velocity = Vect3f::ZERO);
	void moveQuant(float dt);
	void updateRegion(float x1, float y1, float x2, float y2);
	void showDebugInfo();

private:
	list<CrashModel3dx*> crashModels;
};

#endif
