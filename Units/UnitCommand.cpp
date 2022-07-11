#include "StdAfx.h"
#include "UnitCommand.h"
#include "UnitInterface.h"

BEGIN_ENUM_DESCRIPTOR(CommandID, "CommandID")
REGISTER_ENUM(COMMAND_ID_POINT, "���� � �����");
REGISTER_ENUM(COMMAND_ID_ATTACK, "��������� �����");
REGISTER_ENUM(COMMAND_ID_OBJECT, "����������� ������");
REGISTER_ENUM(COMMAND_ID_FIRE, "�������� � �����");
REGISTER_ENUM(COMMAND_ID_FIRE_OBJECT, "�������� � ������");
REGISTER_ENUM(COMMAND_ID_TALK, "�������� ����");

REGISTER_ENUM(COMMAND_ID_UPGRADE, "�������");
REGISTER_ENUM(COMMAND_ID_PRODUCE, "������������ ������");
REGISTER_ENUM(COMMAND_ID_CANCEL_PRODUCTION, "������ ������������ ������");
REGISTER_ENUM(COMMAND_ID_PRODUCE_PARAMETER, "������������ ����������");
REGISTER_ENUM(COMMAND_ID_PRODUCTION_INC, "�������� ����� � �����");
REGISTER_ENUM(COMMAND_ID_PRODUCTION_DEC, "������� ����������� ����� �� ������");

REGISTER_ENUM(COMMAND_ID_SELF_ATTACK_MODE, "������� ����� �����");
REGISTER_ENUM(COMMAND_ID_WALK_ATTACK_MODE, "������� ����� ��������");
REGISTER_ENUM(COMMAND_ID_WEAPON_MODE, "������� ����� ������");
REGISTER_ENUM(COMMAND_ID_AUTO_TARGET_FILTER, "������� ����� ��������������� ������ �����");
REGISTER_ENUM(COMMAND_ID_AUTO_TRANSPORT_FIND, "������� ����� ��������������� ������ ����������");
REGISTER_ENUM(COMMAND_ID_SET_FORMATION, "���������� �������� ������");
REGISTER_ENUM(COMMAND_ID_SET_MAIN_UNIT, "���������� �������� ����� � ������");
REGISTER_ENUM(COMMAND_ID_SET_MAIN_UNIT_BY_INDEX, "���������� �������� ����� � ������ �� ������");
REGISTER_ENUM(COMMAND_ID_MAKE_STATIC, "�������� ��������� �����");
REGISTER_ENUM(COMMAND_ID_MAKE_DYNAMIC, "��������� ��������� �����");
REGISTER_ENUM(COMMAND_ID_CHANGE_MOVEMENT_MODE, "���/������/�� ���������/����");

REGISTER_ENUM(COMMAND_ID_CAMERA_FOCUS, "��������������� ������ �� �����");
REGISTER_ENUM(COMMAND_ID_CAMERA_MOVE, "��������������� ������ �� �����");
REGISTER_ENUM(COMMAND_ID_SELECT_SELF, "����������� ����");
REGISTER_ENUM(COMMAND_ID_DIRECT_CONTROL, "������ ����������");
REGISTER_ENUM(COMMAND_ID_SYNDICAT_CONTROL, "������ ���������� ����� �����")
REGISTER_ENUM(COMMAND_ID_DIRECT_KEYS, "������� ������� ����������");

REGISTER_ENUM(COMMAND_ID_EXPLODE_UNIT, "���� ����");
REGISTER_ENUM(COMMAND_ID_KILL_UNIT, "����� ����� ����");
REGISTER_ENUM(COMMAND_ID_UNINSTALL, "���������������� ������");

REGISTER_ENUM(COMMAND_ID_PUT_OUT_TRANSPORT, "������� ������ �� ����������");
REGISTER_ENUM(COMMAND_ID_PUT_UNIT_OUT_TRANSPORT, "������� ����� �� ����������");

END_ENUM_DESCRIPTOR(CommandID)

UnitCommand::UnitCommand()
{
	commandID_ = COMMAND_ID_UPGRADE;
	commandData_ = 0;
	position_ = Vect3f::ZERO;
	angleZ_ = 0;
	shiftModifier_ = false;
	miniMap_ = false;
	unitIndex_ = 0;
}

UnitCommand::UnitCommand(CommandID commandID, int commandData)
{
	commandID_ = commandID;
	commandData_ = commandData;
	position_ = Vect3f::ZERO;
	angleZ_ = 0;
	shiftModifier_ = false;
	miniMap_ = false;
	unitIndex_ = 0;
}

UnitCommand::UnitCommand(CommandID commandID, const Vect3f& position, int commandData)
{
	commandID_ = commandID;
	commandData_ = commandData;
	position_ = position;
	angleZ_ = 0;
	shiftModifier_ = false;
	miniMap_ = false;
	unitIndex_ = 0;
}

UnitCommand::UnitCommand(CommandID commandID, UnitInterface* unit, int commandData)
{
	commandID_ = commandID;
	commandData_ = commandData;
	position_ = Vect3f::ZERO;
	angleZ_ = 0;
	shiftModifier_ = false;
	miniMap_ = false;
	unitIndex_ = unit ? unit->unitID().index() : 0;
}

UnitCommand::UnitCommand(CommandID commandID, const AttributeBase* attribute, const Vect3f& position, UnitInterface* unit)
: attributeReference_(attribute)
{
	commandID_ = commandID;
	commandData_ = 0;
	position_ = position;
	angleZ_ = 0;
	shiftModifier_ = false;
	miniMap_ = false;
	unitIndex_ = unit ? unit->unitID().index() : 0;
}

UnitCommand::UnitCommand(CommandID commandID, UnitInterface* unit, const Vect3f& position, int commandData)
{
	commandID_ = commandID;
	commandData_ = commandData;
	position_ = position;
	angleZ_ = 0;
	shiftModifier_ = false;
	miniMap_ = false;
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
	unsigned char useFlag = 0;
	if(commandData_)
		if(commandData_ <= 255)
			useFlag |= USE_SHORT_DATA;
		else
			useFlag |= USE_DATA;
	if(!position_.eq(Vect3f::ZERO))
		useFlag |= USE_POSITION;
	if(unitIndex_)
		useFlag |= USE_UNIT;
	if(attributeReference_)
		useFlag |= USE_ATTRIBUTE;
	if(shiftModifier_)
		useFlag |= USE_SHIFT;
	if(fabsf(angleZ_) > FLT_EPS)
		useFlag |= USE_ANGLE;
	if(miniMap_)
		useFlag |= USE_MINIMAP;

	char commandID = commandID_;
	out.write(commandID);

	out.write(useFlag);
	if(useFlag & USE_SHORT_DATA)
		out.write((unsigned char)commandData_);
	else if(useFlag & USE_DATA)
		out.write(commandData_);
	if(useFlag & USE_POSITION)
		out.write(position_);
	if(useFlag & USE_UNIT)
		out.write(unitIndex_);
	if(useFlag & USE_ATTRIBUTE)
		out.write(attributeReference_);
	if(useFlag & USE_ANGLE)
		out.write(angleZ_);
}

void UnitCommand::read(XBuffer& in) 
{
	char commandID;
	in.read(commandID);
	commandID_ = (CommandID)commandID;
	
	unsigned char useFlag;
	in.read(useFlag);
	if(useFlag & USE_SHORT_DATA){
		unsigned char cdata;
		in.read(cdata);
		commandData_ = cdata;
	}
	else if(useFlag & USE_DATA)
		in.read(commandData_);
	if(useFlag & USE_POSITION)
		in.read(position_);
	if(useFlag & USE_UNIT)
		in.read(unitIndex_);
	if(useFlag & USE_ATTRIBUTE)
		in.read(attributeReference_);
	if(useFlag & USE_ANGLE)
		in.read(angleZ_);

	shiftModifier_ = (useFlag & USE_SHIFT) != 0;
	miniMap_ = (useFlag & USE_MINIMAP) != 0;
}

void UnitCommand::serialize(Archive& ar)
{
	AutoAttackMode unitAttackMode;
	WalkAttackMode unitWalkMode;
	WeaponMode unitWeaponMode;
	AutoTargetFilter unitTargetFilter;
	MovementMode movementMode;
	ar.serialize(commandID_, "commandID", "&�������");
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
		case COMMAND_ID_WEAPON_ACTIVATE:
			ar.serialize(commandData_, "weaponID", 0);
			ar.serialize(shiftModifier_, "shiftModifier", "����������� ���������");
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
		case COMMAND_ID_SET_MAIN_UNIT_BY_INDEX:
			ar.serialize(commandData_, "commandData", "����� ����� � ������");
			break;
		case COMMAND_ID_SET_MAIN_UNIT:
		case COMMAND_ID_PRODUCTION_INC:
		case COMMAND_ID_PRODUCTION_DEC:{
			AttributeUnitReference reference = attributeReference_;
			ar.serialize(reference, "attribute", "����");
			attributeReference_ = reference;
			break; }
		//case COMMAND_ID_OBJECT:
		//	ar.serialize(unitIndex_, "unitID", 0);
		case COMMAND_ID_FIRE:
		case COMMAND_ID_ATTACK:
		case COMMAND_ID_WEAPON_DEACTIVATE:
			ar.serialize(commandData_, "weaponID", 0);
			break;
		case COMMAND_ID_TALK:
			ar.serialize(commandData_, "commandData", "����� �������");
			break;
		case COMMAND_ID_CHANGE_MOVEMENT_MODE:
			movementMode  = (MovementMode)commandData_;
			if(ar.serialize(movementMode, "commandData", "����� ��������"))
				commandData_ = movementMode;
			else
				commandData_ = MODE_WALK;
			break;
	}
}

bool UnitCommand::isUnitValid() const 
{
	if(!unitIndex_)
		return true;
	return unit();
}


