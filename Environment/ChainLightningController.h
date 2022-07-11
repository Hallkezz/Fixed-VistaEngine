#ifndef __CHAIN_LIGHTNING_CONTROLLER_H_INCLUDED__
#define __CHAIN_LIGHTNING_CONTROLLER_H_INCLUDED__

#include "..\Units\UnitLink.h"
#include "Timers.h"
#include <set>

#include "EffectReference.h"
#include "..\Units\AbnormalStateAttribute.h"

typedef set<int> UnitCache;

class UnitBase; 
class UnitActing;

class Archive;

typedef vector<UnitActing*> UnitActingList;

struct ChainLightningOwnerInterface
{
	virtual Vect3f	position() const = 0;
	virtual bool	canApply(UnitActing*) const = 0;
	virtual ParameterSet damage() const  = 0;
	virtual UnitBase* owner() const = 0;
};


struct ChainLightningAttribute{
	/// ������� ��� ������ ����� �������������
	int chainFactor_;
	/// ������� ������� ����� ���� �� ������ ����� 
	int unitChainFactor_;

	/// � ����� ������� ��������� ������ ������
	float chainRadius_;
	/// ������������ ����� ���������� ������
	float unitChainRadius_;

	/// ��������� ��������� ������ ��� �����
	EffectReference  strike_effect_;

	/// ����� ��������������� ������ �� ��� ��������� ������
	float spreadingTime_;

	/// ����������� �� �����
	AbnormalStateAttribute abnormalState_;

	ChainLightningAttribute();
	void serialize(Archive& ar);
};

class ChainLightningController
{
public:
	ChainLightningController(ChainLightningOwnerInterface* owner = 0);
	~ChainLightningController();

	// update ����� �� ������ ������ �� ��������
	bool needChainUpdate() const { return !grafValid_ || chainUpdateTimer_(); }
	// ����������� ������ ������
	void update(const UnitActingList& lightningsEmitters);

	void start(const ChainLightningAttribute* attr);
	void stop();
	bool active() const { return phase_ != NOT_STARTED; }

	// ��������� � ��������� �����
	void quant();

	// ����������� ���������
	void release();

	void showDebug() const;

private:
	const ChainLightningAttribute* attr_;
	ChainLightningOwnerInterface* owner_;

	/// �����������, ��������� �� �����
	ParameterSet damage_;

	bool grafValid_;
	DelayTimer chainUpdateTimer_;
	
	enum Phase {
		NOT_STARTED,
		FADE_ON,
		WORKING,
		FADE_OFF
	};
	Phase phase_;
	MeasurementTimer phaseTime_;
	bool checkLevel(int level) const;

	struct ChainNode;
	typedef list<ChainNode> ChainNodes;

	typedef UnitLink<UnitActing> UnitActingLink;

	struct ChainNode{
		UnitActingLink unitNode;
		int strikeIndex;
		int size;
		ChainNodes graf;

		ChainNode(UnitActing* unit)
			: unitNode(unit)
			, strikeIndex(-1)
			, size(0) {}

			Vect3f center() const;
	};
	typedef vector<ChainNode*> ChainNodesLinks;

	ChainNodes chainGraf_;
	UnitCache unitCache_;

	typedef vector<cEffect*> Effects;
	Effects effects;

	void turnOff(cEffect*& eff);

	bool chainStrikeRecurse(ChainNode& node, int deep, bool release = false);
	void showDebugChainRecurse(const ChainNode& node, int deep) const;

	void apply(UnitActing* target, int levelFactor);
};

#endif // #ifndef __CHAIN_LIGHTNING_CONTROLLER_H_INCLUDED__
