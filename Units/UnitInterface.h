#ifndef __UNIT_INTERFACE_H__
#define __UNIT_INTERFACE_H__

#include "BaseUnit.h"
#include "UnitCommand.h"
#include "Units\Triggers.h"
#include "Units\AttributeCache.h"

////////////////////////////////////////////////
// ��������� ��� �������� ����������
// ���������� �����, ������, �������� � ������
////////////////////////////////////////////////

typedef vector<UnitCommand> CommandList;

class UnitItemInventory;
class UnitItemResource;
class UnitReal;
class InventorySet;
struct UI_ShowModeSprite;
enum RequestResourceType;

class UnitInterface : public UnitBase, public AttributeCache
{
public:
	UnitInterface(const UnitTemplate& data);
	~UnitInterface();

	void Quant();
	void graphQuant(float dt);

	//------------------------------------------------
	// �������
	void sendCommand(const UnitCommand& command); // MTG
	void receiveCommand(const UnitCommand& command); // MTL
	
	virtual bool canSuspendCommand(const UnitCommand& command) const { return true; }
	virtual void executeCommand(const UnitCommand& command);

	void setPose(const Se3f& pose, bool initPose);

	virtual UnitReal* getUnitReal() = 0;
	virtual const UnitReal* getUnitReal() const = 0;
	virtual const AttributeBase* selectionAttribute() const { return &attr(); }
	virtual int selectionListPriority() const { return attr().selectionListPriority; }

	virtual bool requestResource(const ParameterSet& resource, RequestResourceType requestResourceType) const;
	virtual void subResource(const ParameterSet& resource);

	virtual void applyParameterArithmeticsImpl(const ArithmeticsData& arithmetics);

	float health() const;
	float sightRadius() const;
	float noiseRadius() const;
	float hearingRadius() const;

	void showSelect(const Vect2f& pos) const;

	void hover() { hoverTimer_ = 2; }
	bool signHovered() { return hoverTimer_ > 0; }

	virtual bool selectAble() const { return false; }
	virtual int impassability() const { return 0; }

	virtual const Se3f& interpolatedPose() const = 0;// ������������ ������ ������ graphQuant()
	
	virtual const UI_ShowModeSprite* getSelectSprite() const = 0;

	//------------------------------------------------
	// ���� �������
	virtual bool isUnseen() const { return false; }

	/// ���� ����� ����������� ����������� ��������?
	virtual bool isWorking() const { return false; }

	//------------------------------------------------

	/// ������� ������� ������, [0, 1]
	virtual float weaponChargeLevel(int weapon_id = 0) const { return 0; }
	/// �������� ������������, [0, 1]
	virtual float productionProgress() const { return 0; }
	/// �������� ������������ ����������, [0, 1]
	virtual float productionParameterProgress() const { return 0; }

	//------------------------------------------------

	virtual bool canFire(int weaponID, RequestResourceType triggerAction = NEED_RESOURCE_SILENT_CHECK) const { return false; }
	virtual bool fireDistanceCheck(const WeaponTarget& target, bool check_fow = false) const { return false; }
	virtual bool canPutToInventory(const UnitItemInventory* item) const { return false; }
	virtual bool putToInventory(const UnitItemInventory* item) const { return false; }
	virtual bool putToInventory(const InventoryItem& item){ return false; }
	virtual bool canExtractResource(const UnitItemResource* item) const { return false; }
	virtual bool canBuild(const UnitReal* building) const { return false; }
	virtual bool canRun() const { return false; }
	virtual Accessibility canUpgrade(int upgradeNumber, RequestResourceType triggerAction = NEED_RESOURCE_SILENT_CHECK) const { return DISABLED; }
	virtual Accessibility canProduction(int number) const { return DISABLED; }
	virtual Accessibility canProduceParameter(int number) const { return DISABLED; }

	virtual bool canDetonateMines() const { return false; }

	//------------------------------------------------

	const InventorySet* inventory() const;

	virtual class UnitSquad* getSquadPoint() { return 0; }
	virtual const class UnitSquad* getSquadPoint() const { return 0; }
	
	virtual bool isSuspendCommand(CommandID commandID) = 0;
	virtual bool isSuspendCommandWork() = 0;

	const CommandList& suspendCommandList() const { return suspendCommandList_; }

	virtual bool getManualMovingPosition(Vect2f& point) { return false; }

	bool usedByTrigger(int priority = 10) const { return usedByTriggerPriority_ && priority >= usedByTriggerPriority_; } 
	virtual bool checkUsage(void* action) const { return action == usedByTriggerAction_; }
	void setUsedByTrigger(int priority, const void* action = 0, int time = 0);  
	const char* usedTriggerName() const { return usedTriggerName_.c_str(); }

	void setAimed() { aimedTimer_.start(1000); }
	bool aimed() const { return aimedTimer_.busy(); }

	virtual bool uniform(const UnitReal* unit = 0) const;
	virtual bool prior(const UnitInterface* unit) const;

	void serialize(Archive& ar);

	void setDebugString(const char* debugString) { debugString_ = debugString; }

protected:
	string debugString_;
	int selectingTeamIndex_;

private:
	CommandList suspendCommandList_; // ������� ���������� �������.

	int usedByTriggerPriority_;
	int usedByTriggerResetCounter_;
	const void* usedByTriggerAction_;
	string usedTriggerName_;
	LogicTimer usedByTriggerTimer_;

	int hoverTimer_;

	LogicTimer aimedTimer_;
};

#endif //__UNIT_INTERFACE_H__
