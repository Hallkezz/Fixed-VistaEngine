#include "StdAfx.h"
#include "UI_Actions.h"
#include "UserInterface\SelectManager.h"
#include "VistaRender\postEffects.h"
#include "Serialization\RangedWrapper.h"

// ------------------- UI_ActionDataFactory

UI_ActionDataFactory::UI_ActionDataFactory()
{
	add<UI_ActionDataInstrumentary>(UI_ACTION_INVERT_SHOW_PRIORITY);
	add<UI_DataStateMark>(UI_ACTION_STATE_MARK);
	add<UI_ActionDataLocString>(UI_ACTION_LOCALIZE_CONTROL);
	add<UI_ActionDataHoverInfo>(UI_ACTION_HOVER_INFO);
	add<UI_ActionDataLinkToAnchor>(UI_ACTION_LINK_TO_ANCHOR);
	add<UI_ActionDataLinkToMouse>(UI_ACTION_LINK_TO_MOUSE);
	add<UI_ActionDataLinkToParent>(UI_ACTION_LINK_TO_PARENT);
	add<UI_ActionDataHotKey>(UI_ACTION_HOT_KEY);
	add<UI_ActionExternalControl>(UI_ACTION_EXTERNAL_CONTROL);
	add<UI_ActionTriggerVariable>(UI_ACTION_GLOBAL_VARIABLE);
	add<UI_ActionAutochangeState>(UI_ACTION_AUTOCHANGE_STATE);
	add<UI_ActionDataFull>(UI_ACTION_PROFILES_LIST);
	add<UI_ActionDataFull>(UI_ACTION_ONLINE_LOGIN_LIST);
	add<UI_ActionDataEdit>(UI_ACTION_PROFILE_INPUT);
	add<UI_ActionDataEdit>(UI_ACTION_CDKEY_INPUT);
	add<UI_ActionDataAction>(UI_ACTION_PROFILE_DIFFICULTY);
	add<UI_ActionDataAction>(UI_ACTION_PROFILE_CREATE);
	add<UI_ActionDataAction>(UI_ACTION_PROFILE_DELETE);
	add<UI_ActionDataAction>(UI_ACTION_PROFILE_SELECT);
	add<UI_ActionDataAction>(UI_ACTION_DELETE_ONLINE_LOGIN_FROM_LIST);
	add<UI_ActionDataAction>(UI_ACTION_GAME_SAVE);
	add<UI_ActionDataAction>(UI_ACTION_GAME_START);
	add<UI_ActionDataAction>(UI_ACTION_GAME_RESTART);
	add<UI_ActionDataAction>(UI_ACTION_DELETE_SAVE);
	add<UI_ActionDataAction>(UI_ACTION_RESET_MISSION);
	add<UI_ActionDataSaveGameListFixed>(UI_ACTION_MISSION_LIST);
	add<UI_ActionDataAction>(UI_ACTION_MISSION_SELECT_FILTER);
	add<UI_ActionDataAction>(UI_ACTION_MISSION_QUICK_START_FILTER);
	add<UI_ActionDataAction>(UI_ACTION_QUICK_START_FILTER_RACE);
	add<UI_ActionDataAction>(UI_ACTION_QUICK_START_FILTER_POPULATION);
	add<UI_ActionDataSaveGameList>(UI_ACTION_BIND_SAVE_LIST);
	add<UI_ActionDataFull>(UI_ACTION_LAN_CHAT_CHANNEL_LIST);
	add<UI_ActionDataAction>(UI_ACTION_LAN_CHAT_CHANNEL_ENTER);
	add<UI_ActionDataAction>(UI_ACTION_LAN_CHAT_CLEAR);
	add<UI_ActionDataUpdate>(UI_ACTION_LAN_CHAT_USER_LIST);
	add<UI_ActionDataHostList>(UI_ACTION_LAN_GAME_LIST);
	add<UI_ActionDataAction>(UI_ACTION_LAN_DISCONNECT_SERVER);
	add<UI_ActionDataAction>(UI_ACTION_LAN_ABORT_OPERATION);
	add<UI_ActionDataPlayer>(UI_ACTION_BIND_PLAYER);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_JOIN_TEAM);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_LEAVE_TEAM);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_NAME);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_STATISTIC_NAME);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_COLOR);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_SIGN);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_RACE);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_DIFFICULTY);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_CLAN);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_READY);
	add<UI_ActionDataPlayer>(UI_ACTION_LAN_PLAYER_TYPE);
	add<UI_ActionDataFull>(UI_ACTION_LAN_USE_MAP_SETTINGS);
	add<UI_ActionDataFull>(UI_ACTION_LAN_GAME_TYPE);
	add<UI_ActionDataEdit>(UI_ACTION_LAN_GAME_NAME_INPUT);
	add<UI_ActionDataEdit>(UI_ACTION_SAVE_GAME_NAME_INPUT);
	add<UI_ActionDataEdit>(UI_ACTION_REPLAY_NAME_INPUT);
	add<UI_ActionDataAction>(UI_ACTION_REPLAY_SAVE);
	add<UI_ActionDataPlayerDefParameter>(UI_ACTION_PLAYER_UNITS_COUNT);
	add<UI_ActionPlayerStatistic>(UI_ACTION_PLAYER_STATISTICS);
	add<UI_ActionDataUpdate>(UI_ACTION_PROFILE_CURRENT);
	add<UI_ActionDataUpdate>(UI_ACTION_MISSION_DESCRIPTION);
	add<UI_ActionDataBindGameType>(UI_ACTION_BIND_GAME_TYPE);
	add<UI_ActionDataUpdate>(UI_ACTION_BIND_GAME_LOADED);
	add<UI_ActionDataUpdate>(UI_ACTION_BIND_NET_PAUSE);
	add<UI_ActionDataPause>(UI_ACTION_BIND_GAME_PAUSE);
	add<UI_ActionDataUpdate>(UI_ACTION_NET_PAUSED_PLAYER_LIST);
	add<UI_ActionDataBindErrorStatus>(UI_ACTION_BIND_ERROR_TYPE);
	add<UI_ActionDataPlayerParameter>(UI_ACTION_PLAYER_PARAMETER);
	add<UI_ActionDataUnitParameter>(UI_ACTION_UNIT_PARAMETER);
	add<UI_ActionDataUnitHint>(UI_ACTION_UNIT_HINT);
	add<UI_ActionDataControlHint>(UI_ACTION_UI_HINT);
	add<UI_ActionDataClickMode>(UI_ACTION_CLICK_MODE);
	add<UI_ActionDataAction>(UI_ACTION_CONTEX_APPLY_CLICK_MODE);
	add<UI_ActionDataAction>(UI_ACTION_CANCEL);
	add<UI_ActionDataUnitOrBuildingUpdate>(UI_ACTION_BIND_TO_UNIT);
	add<UI_ActionDataUnitOrBuildingRef>(UI_ACTION_UNIT_ON_MAP);
	add<UI_ActiobDataUnitHovered>(UI_ACTION_BIND_TO_HOVERED_UNIT);
	add<UI_ActionDataSquadRef>(UI_ACTION_SQUAD_ON_MAP);
	add<UI_ActionDataUnitOrBuildingRef>(UI_ACTION_UNIT_IN_TRANSPORT);
	add<UI_ActionUnitState>(UI_ACTION_BIND_TO_UNIT_STATE);
	add<UI_ActionDataParams>(UI_ACTION_UNIT_HAVE_PARAMS);
	add<UI_ActionDataFindUnit>(UI_ACTION_FIND_UNIT);
	add<UI_ActionDataIdleUnits>(UI_ACTION_BIND_TO_IDLE_UNITS);
	add<UI_ActionDataUnitCommand>(UI_ACTION_UNIT_COMMAND);
	add<UI_ActionDataAction>(UI_ACTION_CLICK_FOR_TRIGGER);
	add<UI_ActionDataBuildingAction>(UI_ACTION_BUILDING_INSTALLATION);
	add<UI_ActionDataUnitOrBuildingUpdate>(UI_ACTION_BUILDING_CAN_INSTALL);
	add<UI_ActionDataSelectUnit>(UI_ACTION_UNIT_SELECT);
	add<UI_ActionDataAction>(UI_ACTION_UNIT_DESELECT);
	add<UI_ActionDataProduction>(UI_ACTION_PRODUCTION_PROGRESS);
	add<UI_ActionDataProduction>(UI_ACTION_PRODUCTION_PARAMETER_PROGRESS);
	add<UI_ActionDataUpdate>(UI_ACTION_PRODUCTION_CURRENT_PROGRESS);
	add<UI_ActionDataWeaponActivate>(UI_ACTION_ACTIVATE_WEAPON);
	add<UI_ActionDataWeapon>(UI_ACTION_DEACTIVATE_WEAPON);
	add<UI_ActionDataWeaponRef>(UI_ACTION_WEAPON_STATE);
	add<UI_ActionDataWeaponReload>(UI_ACTION_RELOAD_PROGRESS);
	add<UI_ActionOption>(UI_ACTION_OPTION);
	add<UI_ActionDataUpdate>(UI_ACTION_BIND_NEED_COMMIT_SETTINGS);
	add<UI_ActionDataUpdate>(UI_ACTION_NEED_COMMIT_TIME_AMOUNT);
	add<UI_ActionDataAction>(UI_ACTION_OPTION_PRESET_LIST);
	add<UI_ActionDataAction>(UI_ACTION_OPTION_APPLY);
	add<UI_ActionDataAction>(UI_ACTION_OPTION_CANCEL);
	add<UI_ActionDataAction>(UI_ACTION_COMMIT_GAME_SETTINGS);
	add<UI_ActionDataAction>(UI_ACTION_ROLLBACK_GAME_SETTINGS);
	add<UI_ActionKeys>(UI_ACTION_SET_KEYS);
	add<UI_ActionBindEx>(UI_ACTION_BINDEX);
	add<UI_ActionDataUpdate>(UI_ACTION_LOADING_PROGRESS);
	add<UI_ActionDataStateChange>(UI_ACTION_CHANGE_STATE);
	add<UI_ActionDataFull>(UI_ACTION_MERGE_SQUADS);
	add<UI_ActionDataFull>(UI_ACTION_SPLIT_SQUAD);
	add<UI_ActionDataAction>(UI_ACTION_SET_SELECTED);
	add<UI_ActionDataSelectionOperate>(UI_ACTION_SELECTION_OPERATE);
	add<UI_ActionDataProduction>(UI_BIND_PRODUCTION_QUEUE);
	add<UI_ActionDataMessageList>(UI_ACTION_MESSAGE_LIST);
	add<UI_ActionDataTaskList>(UI_ACTION_TASK_LIST);
	add<UI_ActionDataUnitFace>(UI_ACTION_UNIT_FACE);
	add<UI_ActionDataSourceOnMouse>(UI_ACTION_SOURCE_ON_MOUSE);
	add<UI_ActionDataUpdate>(UI_ACTION_GET_MODAL_MESSAGE);
	add<UI_ActionDataModalMessage>(UI_ACTION_OPERATE_MODAL_MESSAGE);
	add<UI_ActionDataFull>(UI_ACTION_MINIMAP_ROTATION);
	add<UI_ActionDataFull>(UI_ACTION_MINIMAP_DRAW_WATER);
	add<UI_ActionDataUpdate>(UI_ACTION_SHOW_TIME);
	add<UI_ActionDataUpdate>(UI_ACTION_SHOW_COUNT_DOWN_TIMER);
	add<UI_ActionDataUpdate>(UI_ACTION_INET_NAT_TYPE);
	add<UI_ActionDataAction>(UI_ACTION_INET_CREATE_ACCOUNT);
	add<UI_ActionDataAction>(UI_ACTION_INET_CHANGE_PASSWORD);
	add<UI_ActionDataAction>(UI_ACTION_INET_LOGIN);
	add<UI_ActionDataAction>(UI_ACTION_INET_QUICK_START);
	add<UI_ActionDataAction>(UI_ACTION_INET_REFRESH_GAME_LIST);
	add<UI_ActionDataFull>(UI_ACTION_LAN_CREATE_GAME);
	add<UI_ActionDataFull>(UI_ACTION_LAN_JOIN_GAME);
	add<UI_ActionDataFull>(UI_ACTION_INET_DIRECT_JOIN_GAME);
	add<UI_ActionDataAction>(UI_ACTION_INET_DELETE_ACCOUNT);
	add<UI_ActionDataEdit>(UI_ACTION_INET_NAME);
	add<UI_ActionDataEdit>(UI_ACTION_INET_PASS);
	add<UI_ActionDataEdit>(UI_ACTION_INET_PASS2);
	add<UI_ActionDataEdit>(UI_ACTION_INET_DIRECT_ADDRESS);
	add<UI_ActionDataAction>(UI_ACTION_INET_FILTER_PLAYERS_COUNT);
	add<UI_ActionDataAction>(UI_ACTION_INET_FILTER_GAME_TYPE);
	add<UI_ActionDataAction>(UI_ACTION_STATISTIC_FILTER_RACE);
	add<UI_ActionDataAction>(UI_ACTION_STATISTIC_FILTER_POPULATION);
	add<UI_ActionDataEdit>(UI_ACTION_CHAT_EDIT_STRING);
	add<UI_ActionDataAction>(UI_ACTION_CHAT_SEND_MESSAGE);
	add<UI_ActionDataAction>(UI_ACTION_CHAT_SEND_CLAN_MESSAGE);
	add<UI_ActionDataUpdate>(UI_ACTION_CHAT_MESSAGE_BOARD);
	add<UI_ActionDataUpdate>(UI_ACTION_GAME_CHAT_BOARD);
	add<UI_ActionDataGlobalStats>(UI_ACTION_INET_STATISTIC_QUERY);
	add<UI_ActionDataStatBoard>(UI_ACTION_INET_STATISTIC_SHOW);
	add<UI_ActionDataDirectControlCursor>(UI_ACTION_DIRECT_CONTROL_CURSOR);
	add<UI_ActionDataDirectControlWeaponLoad>(UI_ACTION_DIRECT_CONTROL_WEAPON_LOAD);
	add<UI_ActionDataFull>(UI_ACTION_DIRECT_CONTROL_TRANSPORT);
	add<UI_ActionDataConfirmDiskOp>(UI_ACTION_CONFIRM_DISK_OP);
	add<UI_ActionDataPostEffect>(UI_ACTION_POST_EFFECT);
	add<UI_ActionDataPauseGame>(UI_ACTION_PAUSE_GAME);
	add<UI_ActionDataFull>(UI_ACTION_INVENTORY_QUICK_ACCESS_MODE);
}

// ------------------- UI_ActionData

void UI_ActionData::serialize(Archive& ar)
{
}

template<ActionMode controlMode>
void UI_ActionDataBase<controlMode>::serialize(Archive& ar)
{
	if(controlMode & UI_USER_ACTION_MASK){
		BitVector<UI_UserSelectEvent> events = actionModeTypes_ & UI_USER_ACTION_MASK;
		ar.serialize(events, "mouseEvents", "�������");
		actionModeTypes_ = actionModeTypes_ & ~UI_USER_ACTION_MASK | events;

		BitVector<UI_UserEventMouseModifers> modifers = actionModeModifers_;
		ar.serialize(modifers, "mouseEventsModifers", "������������");
		actionModeModifers_ = modifers;
	}
}

// -------------------  UI_DataStateMark

void UI_DataStateMark::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(type_, "type", "��� �������");
	switch(type_){
	case UI_STATE_MARK_UNIT_SELF_ATTACK:{
		AutoAttackMode mode = AutoAttackMode(enumValue_);
		ar.serialize(mode, "mode", "�����");
		enumValue_ = mode;
		break;
										}
	case UI_STATE_MARK_UNIT_WEAPON_MODE:{
		WeaponMode mode = WeaponMode(enumValue_);
		ar.serialize(mode, "mode", "�����");
		enumValue_ = mode;
		break;
										}
	case UI_STATE_MARK_UNIT_WALK_ATTACK_MODE:{
		WalkAttackMode mode = WalkAttackMode(enumValue_);
		ar.serialize(mode, "mode", "�����");
		enumValue_ = mode;
		break;
											 }
	case UI_STATE_MARK_UNIT_AUTO_TARGET_FILTER:{
		AutoTargetFilter mode = AutoTargetFilter(enumValue_);
		ar.serialize(mode, "mode", "�����");
		enumValue_ = mode;
		break;
											   }
	case UI_STATE_MARK_DIRECT_CONTROL_CURSOR:{
		UI_DirectControlCursorType mode = UI_DirectControlCursorType(enumValue_);
		ar.serialize(mode, "mode", "�����");
		enumValue_ = mode;
		break;
											 }
	}
}

void UI_ActionDataHoverInfo::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(type_, "type", "��� ���������");
	if(ar.isEdit())
		ar.serialize(text_, "text", "����");
	else
		text_.serialize(ar);
	ar.serialize(cursor_, "cursor", "������ ��� ���������");
}


void UI_ActionDataLinkToAnchor::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(linkToSelected_, "linkToSelected", "��������� � �����������");
	if(!linkToSelected_)
		ar.serialize(linkName_, "linkName", "����� ������� ��� ��������");
}

void UI_ActionDataLinkToMouse::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(shift_, "shift", "����� � ��������");
}

void UI_ActionDataLinkToParent::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(shift_, "shift", 0);
}

void UI_ActionDataHotKey::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(hotKey_, "hotKey", "������");
	ar.serialize(compatibleHotKey_, "compatibleHotKey", "M���� ��������� � �������� ����������");
}

// ------------------- UI_ActionDataSaveGameList

UI_ActionDataSaveGameList::UI_ActionDataSaveGameList() 
: gameType_(GAME_TYPE_SCENARIO)
{
	shareList_ = true;
	resetSaveGameName_ = false;
	autoType_ = false;
}

void UI_ActionDataSaveGameList::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(shareList_, "shareList", "����� ������");
	if(shareList_)
		autoType_ = false;
	else
		ar.serialize(autoType_, "autoType", "�������������� ��� �����");
	if(!autoType_)
		ar.serialize(gameType_, "gameType", "��� ����");
	ar.serialize(resetSaveGameName_, "resetSaveGameName", "��� ������ ���������� � ���� ����� �����");
}

UI_ActionDataSaveGameListFixed::UI_ActionDataSaveGameListFixed()
: UI_ActionDataSaveGameList()
{
	inheritPredefine_ = true;
	setPredefine_ = false;;
}

void UI_ActionDataSaveGameListFixed::serialize(Archive& ar)
{
	__super::serialize(ar);

	if(!autoType_){
		ar.serialize(inheritPredefine_, "inheritPredefine", "����������� �������� ��������");
		if(!inheritPredefine_)
			ar.serialize(setPredefine_, "setPredefine", "��������� predefine ��������� ��� ������");
	}
	else
		inheritPredefine_ = true;
}

// ------------------- UI_ActionDataSaveGameList

void UI_ActionDataHostList::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(format_, "showGameListFormat", "������� ���������� �� �����");
	ar.serialize(startedGameColor_, "startedGameColor", "���� ������� � ���������� �����");
}


// -------------------  UI_ActionDataPlayer

UI_ActionDataPlayer::UI_ActionDataPlayer()
{
	playerIndex_ = 0;
	teamIndex_ = 0;
	readOnly_ = false;
	useLoadedMission_ = false;
	onlyOperateControl_ = false;
}

void UI_ActionDataPlayer::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(useLoadedMission_, "useLoadedMission", "������������ ����������� ������");
	ar.serialize(playerIndex_, "playerIndex", "&����� �������");
	ar.serialize(RangedWrapperi(teamIndex_, 0, NETWORK_TEAM_MAX), "teamIndex", "&����� � �������");
	ar.serialize(readOnly_, "nameInput", "Read only");
	ar.serialize(onlyOperateControl_, "onlyOperateControl", "������ ��������� ���������� ������");
}

// -------------------  UI_ActionPlayerStatistic

UI_ActionPlayerStatistic::UI_ActionPlayerStatistic()
{
	statisticType_ = LOCAL;
	playerIndex_ = 0;
	type_ = STAT_UNIT_MY_KILLED;
}

void UI_ActionPlayerStatistic::serialize(Archive& ar)
{
	ar.serialize(statisticType_, "statisticType", "�������� ����������");
	if(statisticType_ == LOCAL)
		ar.serialize(playerIndex_, "playerIndex", "&����� ������");
	ar.serialize(type_, "type", "&�������� ����������");
}

// -------------------  UI_ActionDataSelectPlayer

void UI_ActionDataSelectPlayer::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(byRaceName_, "byRaceName", "�������� �� ����� ����");
}

// -------------------  UI_ActionDataPlayerParameter

void UI_ActionDataPlayerParameter::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(parameter_, "parameter", "&��������");
	ar.serialize(maxValue_, "maxValue", "������������ �������� ��� �������� ����");
}

// -------------------  UI_ActionDataPlayerDefParameter

void UI_ActionDataPlayerDefParameter::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(parameter_, "parameter", "&��������");
}

// -------------------  UI_ActionDataUnitParameter

UI_ActionDataUnitParameter::UI_ActionDataUnitParameter()
{
	unitType_ = SELECTED;
	type_ = LOGIC;
	maxValue_ = -1.f;

	onIncrease_ = false;
	onDecrease_ = false;
	showTime_ = 1.f;

	lastShowId_ = -1;
	lastShowValue_ = 0;
}

bool UI_ActionDataUnitParameter::updateAndCheck(int showId, float parameterValue) const
{
	if(!onIncrease_ && !onDecrease_)
		return true;

	float lastVal = lastShowValue_;
	lastShowValue_ = parameterValue;

	if(showId != lastShowId_){
		lastShowId_ = showId;
		showTimer_.stop();
		return false;
	}
	
	if(onIncrease_ && parameterValue > lastVal){
		showTimer_.start(showTime_ * 1000);
		return true;
	}

	if(onDecrease_ && parameterValue < lastVal){
		showTimer_.start(showTime_ * 1000);
		return true;
	}

	return showTimer_.busy();
}

void UI_ActionDataUnitParameter::serialize(Archive& ar)
{
	__super::serialize(ar);

	///  CONVERSION 2007-10-16
	if(!ar.serialize(unitType_, "unitType", "� ���� ����� ��������")){
		ar.serialize(attributeReference_, "attributeReference", "&����");
	
		if(!attributeReference_.get()){
			bool forPlayerUnit;
			ar.serialize(forPlayerUnit, "forPlayerUnit", "��� �����-������");
			if(!forPlayerUnit){
				bool useUnitList;
				ar.serialize(useUnitList, "useUnitList", "����� ����� �� ������ �������");
				if(useUnitList)
					unitType_ = UNIT_LIST;
				else
					unitType_ = SELECTED;
			}
			else
				unitType_ = PLAYER;
		}
		else
			unitType_ = SPECIFIC;
	}
	else 
	/// ^^^^^
		if(unitType_ == SPECIFIC)
			ar.serialize(attributeReference_, "attributeReference", "&����");
	
	ar.serialize(type_, "type", "&��� ���������");
	if(type_ == LOGIC){
		ar.serialize(parameter_, "parameter", "&������ ��������");
		ar.serialize(maxValue_, "maxValue", "������������ �������� ��� �������� ����");
	}

	ar.serialize(onIncrease_, "onIncrease", "���������� ��� ����������");
	ar.serialize(onDecrease_, "onDecrease", "���������� ��� ����������");
	if(onIncrease_ || onDecrease_)
		ar.serialize(showTime_, "showTime", "����� ������ ����� �������");
}

// -------------------  UI_ActionDataUnitHint

void UI_ActionDataUnitHint::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(hintType_, "hintType", "��� ���������");
	ar.serialize(unitType_, "unitType", "��� �����");
	ar.serialize(showDelay, "showDelay", "�������� ������");
	ar.serialize(line_, "line", "����� ������ ��������");
}

// -------------------  UI_ActionDataControlHint

void UI_ActionDataControlHint::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(type_, "type", "��� ���������");
	ar.serialize(showDelay, "showDelay", "�������� ������");
}

// -------------------  UI_ActionDataBindGameType

void UI_ActionDataBindGameType::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(gameType_, "type", "��� ����");
	ar.serialize(replay_, "replay_", "��� ������");
}

// -------------------  UI_ActionDataPause

void UI_ActionDataPause::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(type_, "type", "���� ����");
}

// -------------------  UI_ActionDataBindErrorStatus

void UI_ActionDataBindErrorStatus::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(type_, "type", "��������� ���������� ��������");
}

// -------------------  UI_ActionDataClickMode

void UI_ActionDataClickMode::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(modeID_, "modeID", "&�����");
	if(modeID_ == UI_CLICK_MODE_ATTACK || modeID_ == UI_CLICK_MODE_PLAYER_ATTACK)
		ar.serialize(weaponPrmReference_, "weaponPrmReference", "��� ������ (��� �����)");
}

// -------------------  UI_ActionDataUnitRef

template<ActionMode mode, int refEditType>
void UI_ActionDataUnitRef<mode, refEditType>::serialize(Archive& ar)
{
	__super::serialize(ar);

	if(ar.isEdit())
		switch(refEditType){
		case 1:{
			AttributeUnitReference unit = attributeReference_;
			ar.serialize(unit, "attributeReference", "&����");
			attributeReference_ = unit;
			break;
			   }
		case 2:{
			AttributeBuildingReference building = attributeReference_;
			ar.serialize(building, "attributeReference", "&����");
			attributeReference_ = building;
			break;
			   }
		case 3:
			ar.serialize(attributeReference_, "attributeReference", "&����");
			break;
		}
	else
		ar.serialize(attributeReference_, "attributeReference", "&����");

}

// -------------------  UI_ActiobDataUnitHovered

void UI_ActiobDataUnitHovered::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(ownUnit_, "ownUnit", "������ �����������");
}

// -------------------  UI_ActionDataUnitOrBuildingUpdate

void UI_ActionDataUnitOrBuildingUpdate::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(attributeReference_, "attributeReference", "&����");

	ar.serialize(singleSelect_, "singleSelect_", "��������� ������");
	ar.serialize(uniformSelection_, "uniformSelection", "���������� ������");
}

// -------------------  UI_ActionDataSelectUnit

void UI_ActionDataSelectUnit::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(attributeReference_, "attributeReference", "&����");
	if(attributeReference_.key() < 0)
		ar.serialize(squadRef_, "squadRef", "��� ������");
	ar.serialize(onlyPowered_, "onlyPowered", "������ ������������");
}

// -------------------  UI_ActionDataFindUnit

UI_ActionDataFindUnit::UI_ActionDataFindUnit()
{
	select_ = true;
	targeting_ = true;
	showCount_ = false;
	referenceIdx_ = 0;
	unitIdx_ = 0;
}

void UI_ActionDataFindUnit::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(unitReferences_, "unitReferences", "���� ������");
	ar.serialize(select_, "select", "��������");
	ar.serialize(targeting_, "targeting", "���������� ������");
	ar.serialize(showCount_, "showCount", "�������� ���������� �� ����, ������� ����� ����");
}

// -------------------  UI_ActionDataSquadRef

void UI_ActionDataSquadRef::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(squadRef_, "squadRef", "&��� ������");
	ar.serialize(showCount_, "showCount", "�������� ���������� ������� �� ����");
}

// -------------------  UI_ActionUnitState

UI_ActionUnitState::UI_ActionUnitState()
{
	data_ = 0;
	type_ = UI_UNITSTATE_RUN;
	checkOnlyMainUnitInSquad_ = false;
	forPlayerUnit_ = false;
}

void UI_ActionUnitState::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(type_, "type", "��� ���������");
	switch(type_) {
	case UI_UNITSTATE_CAN_UPGRADE:
	case UI_UNITSTATE_IS_UPGRADING:
		ar.serialize(data_, "data", "����� ��������");
		break;
	case UI_UNITSTATE_SQUAD_CAN_QUERY_UNITS:
	case UI_UNITSTATE_IS_BUILDING:
		ar.serialize(attributeReference_, "attributeReference", "������������ ����");
		break;
	case UI_UNITSTATE_CAN_BUILD:
	case UI_UNITSTATE_CAN_PRODUCE_PARAMETER:
		ar.serialize(data_, "data", "����� ������������");
		break;
	}
	
	switch(type_){
	case UI_UNITSTATE_CAN_BUILD:
	case UI_UNITSTATE_CAN_PRODUCE_PARAMETER:
		ar.serialize(forPlayerUnit_, "forPlayerUnit", "��� �����-������");
	case UI_UNITSTATE_CAN_DETONATE_MINES:
	case UI_UNITSTATE_IS_IDLE:
	case UI_UNITSTATE_CAN_UPGRADE:
		if(!forPlayerUnit_)
			ar.serialize(checkOnlyMainUnitInSquad_, "checkOnlyMainUnitInSquad", "��������� ������ �������� ����� � ������");
	}
}

const AttributeBase* UI_ActionUnitState::getAttribute(const AttributeBase* context) const
{
	switch(type_) {
	case UI_UNITSTATE_CAN_BUILD:
		if(context && context->producedUnits.exists(data_))
			return context->producedUnits[data_].unit;
		return 0;
	case UI_UNITSTATE_CAN_UPGRADE:
	case UI_UNITSTATE_IS_UPGRADING:
		if(context && context->upgrades.exists(data_))
			return context->upgrades[data_].upgrade;
		return 0;
	case UI_UNITSTATE_IS_BUILDING:
		return attributeReference_.get();
	}
	return 0;
}

// -------------------  UI_ActionDataParams

UI_ActionDataParams::UI_ActionDataParams()
{
	type_ = SELECTED_UNIT_THEN_PLAYER;
	thatDoThanExist_ = HIDE;
}

void UI_ActionDataParams::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(type_, "type", "���� ���������");
	ar.serialize(thatDoThanExist_, "thatDoThanExist", "��� ������");
	ar.serialize(param_, "prms", "���������");
}

// -------------------  UI_ActionDataIdleUnits

void UI_ActionDataIdleUnits::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(type_, "type", "��� �����");
}

// -------------------  UI_ActionDataUnitCommand

UI_ActionDataUnitCommand::UI_ActionDataUnitCommand()
{
	forPlayerUnit_ = false;
	sendForAll_ = false;
}

void UI_ActionDataUnitCommand::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(command_, "command", "�������");
	ar.serialize(forPlayerUnit_, "forPlayerUnit", "��� �����-������");
	if(!forPlayerUnit_)
		ar.serialize(sendForAll_, "sendForAll", "�������� ���� � �������");
}

// -------------------  UI_ActionDataWeaponRef

UI_ActionDataWeaponRef::UI_ActionDataWeaponRef()
{
	forPlayerUnit_ = false;
}

void UI_ActionDataWeaponRef::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(forPlayerUnit_, "forPlayerUnit", "��� �����-������");
	ar.serialize(weaponPrmReference_, "weaponPrmReference", "��� ������");
}

// -------------------  UI_ActionDataWeapon

UI_ActionDataWeapon::UI_ActionDataWeapon()
{
	forPlayerUnit_ = false;
}

void UI_ActionDataWeapon::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(forPlayerUnit_, "forPlayerUnit", "��� �����-������");
	ar.serialize(weaponPrmReference_, "weaponPrmReference", "��� ������");
}

// -------------------  UI_ActionDataWeaponActivate

void UI_ActionDataWeaponActivate::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(fireOnce_, "fireOnce", "��������� ����������");
}

// -------------------  UI_ActionDataWeaponReload

void UI_ActionDataWeaponReload::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(weaponPrmReference_, "weaponPrmReference", "��� ������");
	ar.serialize(forPlayerUnit_, "forPlayerUnit", "��� �����-������");
	ar.serialize(show_only_not_full_, "show_only_not_full_", "���������� ������ �� ��������� ����������");
}


// -------------------  UI_ActionOption

UI_ActionOption::UI_ActionOption(){
	action_type_ = UI_OPTION_UPDATE;
	option_ = (GameOptionType)0;
}

UI_ActionOption::UI_ActionOption(UI_OptionType type, GameOptionType op){
	action_type_ = type;
	option_ = op;
}

void UI_ActionOption::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(option_, "option_", "���������");

}

// -------------------  UI_ActionKeys

UI_ActionKeys::UI_ActionKeys(){
	action_type_ = UI_OPTION_UPDATE;
	option_ = (InterfaceGameControlID)0;
}

UI_ActionKeys::UI_ActionKeys(UI_OptionType type, InterfaceGameControlID option){
	action_type_ = type;
	option_ = option;
}

void UI_ActionKeys::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(action_type_, "action_type", "��� �������");
	if(action_type_ == UI_OPTION_UPDATE)
		ar.serialize(option_, "option", "�������"); 

}

// ------------------- UI_ActionGlobalVariable

void UI_ActionTriggerVariable::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(type_, "type", "��� ����������");
	if(type_ == MISSION_DESCRIPTION){
		ar.serialize(number_, "variableNumber", "����� ����������");
		number_ = clamp(number_, 0, 31);
	}
	else
		ar.serialize(variableName_, "variableName", "��� ����������");
		
}

// ------------------- UI_ActionBindEx

UI_ActionBindEx::UI_ActionBindEx()
{
	uniformSelection_ = false;
	type_ = UI_BIND_PRODUCTION;
	selSize_ = UI_SEL_ANY;
	queueSize_ = UI_QUEUE_ANY;
}

void UI_ActionBindEx::serialize(Archive& ar)
{
	__super::serialize(ar);
	
	ar.serialize(type_, "type_", "��� ��������");

	if(type_ != UI_BIND_ANY)
		ar.serialize(queueSize_, "queueSize_", "��������� �������");
	
	ar.serialize(selSize_, "selSize_", "������ ���������");
	ar.serialize(uniformSelection_, "uniformSelection", "���������� ���������");
}

// -------------------  UI_ActionDataStateChange

void UI_ActionDataStateChange::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(reverseDirection_, "reverseDirection_", "����������� � �������� �������");
}

// -------------------  UI_ActionDataProduction

void UI_ActionDataProduction::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(forPlayerUnit_, "forPlayerUnit", "��� �����-������");
	ar.serialize(productionNumber_, "productionNumber", "����� ������������"); 
}

// -------------------  UI_ActionDataProduction

UI_ActionDataSelectionOperate::UI_ActionDataSelectionOperate()
{
	selectonID_ = 0;
	command_ = SAVE_SELECT;
}

void UI_ActionDataSelectionOperate::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(command_, "command", "�������"); 
	ar.serialize(RangedWrapperi(selectonID_, 0, SAVED_SELECTION_MAX, 1), "selectonID", "����� ����� ����������/�������������� �������"); 
}

// -------------------  UI_ActionAutochangeState

void UI_ActionAutochangeState::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(interval_, "interval", "����� ������");
}

// -------------------  UI_ActionDataLocString

void UI_ActionDataLocString::serialize(Archive& ar)
{
	__super::serialize(ar);

	if(ar.isEdit())
		ar.serialize(locText_, "key", "����");
	else
		locText_.serialize(ar);

	ar.serialize(expand_, "expand_", "���������� �������");
	ar.serialize(forHovered_, "forHovered", "������������ ����� ��� ������");
}

// -------------------  UI_ActionExternalControl

void UI_ActionExternalControl::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(actions_, "actions", "�������� � ��������");
}

// -------------------  UI_ActionDataSourceOnMouse

void UI_ActionDataSourceOnMouse::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(sourceReference_, "sourceReference", "��������");
}

// -------------------  UI_ActionDataModalMessage

void UI_ActionDataModalMessage::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(type_, "type", "��������");
}

// -------------------  UI_ActionDataMessageList

void UI_ActionDataMessageList::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(reverse_, "reverse", "�������� � �������� �������");
	ar.serialize(firstOnly_, "firstOnly", "�������� ������ ������");
	ar.serialize(ignoreMessageDisabling_, "ignoreMessageDisabling", "������������ ������ �� ���������");
	ar.serialize(types_, "types", "���� ��������� ��������");
}

// -------------------  UI_ActionDataTaskList

void UI_ActionDataTaskList::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(reverse_, "reverse", "�������� � �������� �������");
}

// -------------------  UI_ActionDataUnitFace

void UI_ActionDataUnitFace::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(num_, "face_num", "����� ��������");
}

// -------------------  UI_ActionDataDirectControlCursor

UI_ActionDataDirectControlCursor::UI_ActionDataDirectControlCursor() : scale_(1.f, 1.5f),
	scaleDist_(0.f, 300.f)
{
}

void UI_ActionDataDirectControlCursor::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(scale_, "scale", "�������� ��������� ������� �������");
	ar.serialize(scaleDist_, "scaleDist", "�������� ���������� ��� ��������� ������� �������");
}

float UI_ActionDataDirectControlCursor::cursorScale(float aim_distance) const
{
	if(scaleDist_.length() > FLT_EPS){
		aim_distance = scaleDist_.clip(aim_distance);
		float phase = (aim_distance - scaleDist_.minimum()) / scaleDist_.length();
		float scale = scale_.minimum() + scale_.length() * (1.f - phase);
		return scale;
	}

	return 1.f;
}

// -------------------  UI_ActionDataDirectControlWeaponLoad

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(UI_ActionDataDirectControlWeaponLoad, WeaponType, "UI_ActionDataDirectControlWeaponLoad::WeaponType")
REGISTER_ENUM_ENCLOSED(UI_ActionDataDirectControlWeaponLoad, WEAPON_PRIMARY, "�������� ������")
REGISTER_ENUM_ENCLOSED(UI_ActionDataDirectControlWeaponLoad, WEAPON_SECONDARY, "�������������� ������")
END_ENUM_DESCRIPTOR_ENCLOSED(UI_ActionDataDirectControlWeaponLoad, WeaponType)

UI_ActionDataDirectControlWeaponLoad::UI_ActionDataDirectControlWeaponLoad() : weaponType_(WEAPON_PRIMARY)
{
}

void UI_ActionDataDirectControlWeaponLoad::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(weaponType_, "weaponType", "������");
}

// ------------------- UI_ActionDataConfirmDiskOp

void UI_ActionDataConfirmDiskOp::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(confirmDiskOp_, "confirmDiskOp", "����������� ����������");
}


// ------------------- UI_ActionDataStatBoard

void UI_ActionDataStatBoard::serialize(Archive& ar)
{
	ar.serialize(format_, "format", "�������");
}

// ------------------- UI_ActionDataPostEffect

UI_ActionDataPostEffect::UI_ActionDataPostEffect() 
	: enable_(false)
	, type_(PE_MONOCHROME)
{
}

void UI_ActionDataPostEffect::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(type_, "type", "������");
	ar.serialize(enable_, "enable", "��������");
}

// ------------------- UI_ActionDataPauseGame

void UI_ActionDataPauseGame::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(enable_, "enable", "��������");
	ar.serialize(onlyLocal_, "onlyLocal", "���������� ��� ������� ����");
}

// ------------------- UI_ActionDataGlobalStats

void UI_ActionDataGlobalStats::serialize(Archive& ar)
{
	__super::serialize(ar);

	ar.serialize(task_, "task", "��������");
}

