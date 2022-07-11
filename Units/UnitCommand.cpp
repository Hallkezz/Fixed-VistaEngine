#include "StdAfx.h"
#include "UnitCommand.h"
#include "UnitInterface.h"

BEGIN_ENUM_DESCRIPTOR(CommandID, "CommandID")
REGISTER_ENUM(COMMAND_ID_UPGRADE, "�������");
REGISTER_ENUM(COMMAND_ID_SELF_ATTACK_MODE, "������� ����� �����");
REGISTER_ENUM(COMMAND_ID_WALK_ATTACK_MODE, "������� ����� ��������");
REGISTER_ENUM(COMMAND_ID_WEAPON_MODE, "������� ����� ������");
REGISTER_ENUM(COMMAND_ID_AUTO_TARGET_FILTER, "������� ����� ��������������� ������ �����");
REGISTER_ENUM(COMMAND_ID_AUTO_TRANSPORT_FIND, "������� ����� ��������������� ������ ����������");
REGISTER_ENUM(COMMAND_ID_PRODUCE, "������������ ������");
REGISTER_ENUM(COMMAND_ID_CANCEL_PRODUCTION, "������ ������������ ������");
REGISTER_ENUM(COMMAND_ID_PRODUCE_PARAMETER, "������������ ����������");
REGISTER_ENUM(COMMAND_ID_SET_FORMATION, "���������� �������� ������");
REGISTER_ENUM(COMMAND_ID_GO_RUN, "������� �� ���");
REGISTER_ENUM(COMMAND_ID_STOP_RUN, "������� �� ������");
REGISTER_ENUM(COMMAND_ID_PRODUCTION_INC, "�������� ����� � �����");
REGISTER_ENUM(COMMAND_ID_PRODUCTION_DEC, "������� ����������� ����� �� ������");
REGISTER_ENUM(COMMAND_ID_PRODUCTION_PAUSE_ON, "���������� ������������ ����� � �����");
REGISTER_ENUM(COMMAND_ID_PRODUCTION_PAUSE_OFF, "���������� ������������ ����� � �����");
REGISTER_ENUM(COMMAND_ID_PUT_OUT_TRANSPORT, "������� ������ �� ����������");
REGISTER_ENUM(COMMAND_ID_PUT_UNIT_OUT_TRANSPORT, "������� ����� �� ����������");
REGISTER_ENUM(COMMAND_ID_CAMERA_FOCUS, "��������������� ������ �� �����");
REGISTER_ENUM(COMMAND_ID_EXPLODE_UNIT, "���� ����");
REGISTER_ENUM(COMMAND_ID_KILL_UNIT, "����� ����� ����");
REGISTER_ENUM(COMMAND_ID_DIRECT_CONTROL, "������ ����������");
REGISTER_ENUM(COMMAND_ID_SYNDICAT_CONTROL, "������ ���������� ����� �����")
REGISTER_ENUM(COMMAND_ID_DIRECT_KEYS, "������� ������� ����������");
REGISTER_ENUM(COMMAND_ID_UNINSTALL, "���������������� ������");
REGISTER_ENUM(COMMAND_ID_POINT, "���� � �����");
REGISTER_ENUM(COMMAND_ID_ATTACK, "��������� �����");
REGISTER_ENUM(COMMAND_ID_OBJECT, "����������� ������");
END_ENUM_DESCRIPTOR(CommandID)

UnitCommand::UnitCommand()
{
	commandID_ = COMMAND_ID_UPGRADE;
	commandData_ = 0;
	position_ = Vect3f::ZERO;
	shiftModifier_ = false;
	unitIndex_ = 0;
}

UnitCommand::UnitCommand(CommandID commandID, int commandData)
{
	commandID_ = commandID;
	commandData_ = commandData;
	position_ = Vect3f::ZERO;
	shiftModifier_ = false;
	unitIndex_ = 0;
}

UnitCommand::UnitCommand(CommandID commandID, const Vect3f& position, int commandData)
{
	commandID_ = commandID;
	commandData_ = commandData;
	position_ = position;
	shiftModifier_ = false;
	unitIndex_ = 0;
}

UnitCommand::UnitCommand(CommandID commandID, UnitInterface* unit, int commandData)
{
	commandID_ = commandID;
	commandData_ = commandData;
	position_ = Vect3f::ZERO;
	shiftModifier_ = false;
	unitIndex_ = unit ? unit->unitID().index() : 0;
}

UnitCommand::UnitCommand(CommandID commandID, const AttributeBase* attribute, const Vect3f& position, UnitInterface* unit)
: attributeReference_(attribute)
{
	commandID_ = commandID;
	commandData_ = 0;
	position_ = position;
	shiftModifier_ = false;
	unitIndex_ = unit ? unit->unitID().index() : 0;
}

UnitCommand::UnitCommand(CommandID commandID, UnitInterface* unit, const Vect3f& position, int commandData)
{
	commandID_ = commandID;
	commandData_ = commandData;
	position_ = position;
	shiftModifier_ = false;
	unitIndex_ = unit ? unit->unitID().index() : 0;
}

UnitInterface* UnitCommand::unit() const
{
	return safe_cast<UnitInterface*>(UnitID::get(unitIndex_));
}

bool UnitCommand::operator==(const UnitCommand& command) const 
{ 
	return commandID() == command.commandID()
	&& commandData() == command.commandData() 
	&& command.position().eq(position())
	&& attribute() == command.attribute()
	&& unitIndex_ == command.unitIndex_;
}

void UnitCommand::write(XBuffer& out) const
{
	char useFlag = 0;
	if(commandData_)
		useFlag |= USE_DATA;
	if(!position_.eq(Vect3f::ZERO))
		useFlag |= USE_POSITION;
	if(unitIndex_)
		useFlag |= USE_UNIT;
	if(attributeReference_)
		useFlag |= USE_ATTRIBUTE;
	if(shiftModifier_)
		useFlag |= USE_SHIFT;

	char commandID = commandID_;
	out.write(commandID);

	out.write(useFlag);
	if(useFlag & USE_DATA)
		out.write(commandData_);
	if(useFlag & USE_POSITION)
		out.write(position_);
	if(useFlag & USE_UNIT)
		out.write(unitIndex_);
	if(useFlag & USE_ATTRIBUTE)
		out.write(attributeReference_);
	if(useFlag & USE_SHIFT)
		out.write(shiftModifier_);
}

void UnitCommand::read(XBuffer& in) 
{
	char commandID;
	in.read(commandID);
	commandID_ = (CommandID)commandID;
	
	char useFlag;
	in.read(useFlag);
	if(useFlag & USE_DATA)
		in.read(commandData_);
	if(useFlag & USE_POSITION)
		in.read(position_);
	if(useFlag & USE_UNIT)
		in.read(unitIndex_);
	if(useFlag & USE_ATTRIBUTE)
		in.read(attributeReference_);
	if(useFlag & USE_SHIFT)
		in.read(shiftModifier_);
}

void UnitCommand::serialize(Archive& ar)
{
	AutoAttackMode unitAttackMode;
	WalkAttackMode unitWalkMode;
	WeaponMode unitWeaponMode;
	AutoTargetFilter unitTargetFilter;

	ar.serialize(commandID_, "commandID", "�������");
	switch(commandID_){
		case COMMAND_ID_SELF_ATTACK_MODE:
			unitAttackMode = (AutoAttackMode)commandData_;
			ar.serialize(unitAttackMode, "commandData", "����� �����");
			commandData_ = unitAttackMode;
			break;
		case COMMAND_ID_WALK_ATTACK_MODE:
			unitWalkMode = (WalkAttackMode)commandData_;
			ar.serialize(unitWalkMode, "commandData", "����� ��������");
			commandData_ = unitWalkMode;
			break;
		case COMMAND_ID_WEAPON_MODE:
			unitWeaponMode = (WeaponMode)commandData_;
			ar.serialize(unitWeaponMode, "commandData", "����� ������");
			commandData_ = unitWeaponMode;
			break;
		case COMMAND_ID_AUTO_TARGET_FILTER:
			unitTargetFilter = (AutoTargetFilter)commandData_;
			ar.serialize(unitTargetFilter, "commandData", "����� ��������������� ������ �����");
			commandData_ = unitTargetFilter;
			break;
		case COMMAND_ID_AUTO_TRANSPORT_FIND:{
			bool setAutoFind = commandData_;
			ar.serialize(setAutoFind, "commandData", "������ ��������� �������������");
			commandData_ = setAutoFind;
			break;
											}
		case COMMAND_ID_UPGRADE:
			ar.serialize(commandData_, "commandData", "����� ��������");
			break;
		case COMMAND_ID_PRODUCE:
			ar.serialize(commandData_, "commandData", "����� ������������");
			ar.serialize(shiftModifier_, "shiftModifier", "�������� �����");
			break;
		case COMMAND_ID_PRODUCE_PARAMETER:
			ar.serialize(commandData_, "commandData", "����� ������������");
			break;
		case COMMAND_ID_SET_FORMATION:
			ar.serialize(commandData_, "commandData", "����� ��������");
			break;
		case COMMAND_ID_PRODUCTION_INC:
		case COMMAND_ID_PRODUCTION_DEC:
		case COMMAND_ID_PRODUCTION_PAUSE_ON:
		case COMMAND_ID_PRODUCTION_PAUSE_OFF: {
			AttributeUnitReference reference = attributeReference_;
			ar.serialize(reference, "attribute", "����");
			attributeReference_ = reference;
			break; }
		//case COMMAND_ID_OBJECT:
		//	ar.serialize(unitIndex_, "unitID", 0);
		case COMMAND_ID_ATTACK:
			ar.serialize(commandData_, "weaponID", "����� ������");
		case COMMAND_ID_POINT:
			ar.serialize(position_, "position", "���������� �����");
			break;
	}
}

bool UnitCommand::isUnitValid() const 
{
	if(!unitIndex_)
		return true;
	return unit();
}

