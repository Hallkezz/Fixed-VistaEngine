#include "StdAfx.h"
#include "Controls.h"
#include "CommonLocText.h"
#include "GameOptions.h"
#include "Serialization\Serialization.h"
#include "Serialization\EnumDescriptor.h"

WRAP_LIBRARY(ControlManager, "ControlManager", "ControlManager", "Scripts\\Content\\Controls", 0, 0);

ControlManager::ControlManager()
{
	interpret_ = GameOptions::instance().getTranslate();
	defaultSettings();
	xassert(controls_.size() >= (int)(CTRL_MAX));
}

void ControlManager::serialize(Archive &ar)
{
	for(InterfaceGameControlID i = InterfaceGameControlID(0); i < CTRL_MAX; i = static_cast<InterfaceGameControlID>(i + 1))
		ar.serialize(controls_[i].key_, getEnumName(controls_[i].command_), getEnumNameAlt(controls_[i].command_));
}

bool ControlManager::ControlNode::serialize(Archive& ar, const char* name, const char* nameAlt)
{
	bool nodeExists = ar.serialize(command_, "command", "�������");
	ar.serialize(key_, "key", "&�������");
	return nodeExists;
}

InterfaceGameControlID ControlManager::control(int fullKey) const
{
	ControlNodeList::const_iterator it = find(controls_.begin(), controls_.end(), fullKey);
	if(it != controls_.end())
		return it->command_;

	return CTRL_MAX;
}

const UI_Key& ControlManager::key(InterfaceGameControlID ctrl) const
{
	ControlNodeList::const_iterator it = find(controls_.begin(), controls_.end(), ctrl);
	if(it != controls_.end())
		return it->key_;

	static UI_Key empty;
	return empty;
}

bool ControlManager::isCompatible(InterfaceGameControlID ctrl1, InterfaceGameControlID ctrl2) const
{
	int sz = compatibleControls_.size();
	for(int i = 0; i < sz; i++){
		if((compatibleControls_[i].first == ctrl1 && compatibleControls_[i].second == ctrl2) ||
			(compatibleControls_[i].first == ctrl2 && compatibleControls_[i].second == ctrl1))
				return true;
	}

	return false;
}

void ControlManager::unRegisterHotKey(const CallWrapperBase* func)
{
	MTAuto autolock(updateLock_);

	HotKeyList::iterator it;
	FOR_EACH(hotKeyHandlers_, it)
		if(it->handler_ == func){
			hotKeyHandlers_.erase(it);
			break;
		}
}

void ControlManager::registerHotKey(int fullKey, const CallWrapperBase* func)
{
	MTAuto autolock(updateLock_);

	unRegisterHotKey(func);
	hotKeyHandlers_.push_back(HotKeyNode(fullKey, func));
}

bool ControlManager::checkHotKey(int fullKey)
{
	MTAuto autolock(updateLock_);

	return find(hotKeyHandlers_.begin(), hotKeyHandlers_.end(), fullKey) == hotKeyHandlers_.end();
}

void ControlManager::handleKeypress(int fullKey)
{
	MTAuto autolock(updateLock_);

	HotKeyList::const_iterator it;
	FOR_EACH(hotKeyHandlers_, it)
		if(it->key_ == fullKey)
			if(it->handler_->call())
				break;
}

void ControlManager::setKey(InterfaceGameControlID ctrl, const UI_Key& key)
{
	ControlNodeList::iterator it = find(controls_.begin(), controls_.end(), ctrl);
	if(it == controls_.end())
		controls_.push_back(ControlNode(ctrl, key));
	else
		it->key_ = key;
}

UI_Key ControlManager::defaultKey(InterfaceGameControlID ctrl) const
{
	switch(ctrl){
	case CTRL_CAMERA_MOVE_UP:
		return interpret(UI_Key(VK_UP));
	case CTRL_CAMERA_MOVE_DOWN:
		return interpret(UI_Key(VK_DOWN));
	case CTRL_CAMERA_MOVE_LEFT:
		return interpret(UI_Key(VK_LEFT));
	case CTRL_CAMERA_MOVE_RIGHT:
		return interpret(UI_Key(VK_RIGHT));
	case CTRL_CAMERA_ROTATE_UP:
		return interpret(UI_Key(VK_NUMPAD8));
	case CTRL_CAMERA_ROTATE_DOWN:
		return interpret(UI_Key(VK_NUMPAD2));
	case CTRL_CAMERA_ROTATE_LEFT:
		return interpret(UI_Key(VK_NUMPAD4));
	case CTRL_CAMERA_ROTATE_RIGHT:
		return interpret(UI_Key(VK_NUMPAD6));
	case CTRL_CAMERA_ZOOM_INC:
		return interpret(UI_Key(VK_ADD));
	case CTRL_CAMERA_ZOOM_DEC:
		return interpret(UI_Key(VK_SUBTRACT));
	case CTRL_CAMERA_MOUSE_LOOK:
		return interpret(UI_Key(VK_MBUTTON));
	case CTRL_DIRECT_UP:
		return interpret(UI_Key('W'));
	case CTRL_DIRECT_DOWN:
		return interpret(UI_Key('S'));
	case CTRL_DIRECT_LEFT:
		return interpret(UI_Key('A'));
	case CTRL_DIRECT_RIGHT:
		return interpret(UI_Key('D'));
	case CTRL_DIRECT_STRAFE_LEFT:
		return interpret(UI_Key('Q'));
	case CTRL_DIRECT_STRAFE_RIGHT:
		return interpret(UI_Key('E'));
	case CTRL_PAUSE:
		return interpret(UI_Key(VK_PAUSE));
	case CTRL_MAKESHOT:
		return interpret(UI_Key(VK_F11));
	case CTRL_SHOW_UNIT_PARAMETERS:
		return interpret(UI_Key(VK_MENU));
	case CTRL_SHOW_ITEMS_HINT:
		return interpret(UI_Key('X'));
	case CTRL_TOGGLE_MUSIC:
		return interpret(UI_Key(KBD_SHIFT|'M'));
	case CTRL_TOGGLE_SOUND:
		return interpret(UI_Key(KBD_SHIFT|'S'));
	case CTRL_TIME_NORMAL:
		return interpret(UI_Key(KBD_SHIFT|KBD_CTRL|'A'));
	case CTRL_TIME_DEC:
		return interpret(UI_Key(KBD_SHIFT|'Z'));
	case CTRL_TIME_INC:
		return interpret(UI_Key(KBD_SHIFT|'A'));
	case CTRL_CAMERA_TO_EVENT:
		return interpret(UI_Key(VK_SPACE));
	case CTRL_CLICK_ACTION:
		return interpret(UI_Key(VK_LBUTTON));
	case CTRL_ATTACK:
		return interpret(UI_Key(VK_RBUTTON));
	case CTRL_ATTACK_ALTERNATE:
		return interpret(UI_Key(VK_RBUTTON));
	case CTRL_SELECT_ALL:
		return interpret(UI_Key(VK_LDBL));
	case CTRL_SELECT:
		return interpret(UI_Key(VK_LBUTTON));
	case CTRL_MOVE:
		return interpret(UI_Key(VK_RBUTTON));

	}

	return UI_Key();
}

UI_Key ControlManager::interpret(UI_Key out) const
{
	switch(interpret_){
	case UI_COMMON_TEXT_LANG_GERMAN:
		switch(out.key){
		case 'Z': out.key = 'Y'; break;
		case 'Y': out.key = 'Z'; break;
		}
		break;
	case UI_COMMON_TEXT_LANG_FRENCH:
		switch(out.key){
		case 'W': out.key = 'Z'; break;
		case 'Z': out.key = 'W'; break;
		case 'Q': out.key = 'A'; break;
		case 'A': out.key = 'Q'; break;
			}
		break;
	//case UI_COMMON_TEXT_LANG_SPANISH:
	//case UI_COMMON_TEXT_LANG_ITALIAN:
	}
	return out;
}

void ControlManager::defaultSettings()
{
	for(int i = 0; i < CTRL_MAX; ++i)
		setKey(InterfaceGameControlID(i), defaultKey(InterfaceGameControlID(i)));

	compatibleControls_.clear();
	compatibleControls_.reserve(16);

	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CAMERA_MOVE_UP, CTRL_DIRECT_UP));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CAMERA_MOVE_DOWN, CTRL_DIRECT_DOWN));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CAMERA_MOVE_LEFT, CTRL_DIRECT_LEFT));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CAMERA_MOVE_RIGHT, CTRL_DIRECT_RIGHT));

	compatibleControls_.push_back(CompatibleControlsNode(CTRL_SELECT, CTRL_CLICK_ACTION));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_SELECT, CTRL_ATTACK));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CLICK_ACTION, CTRL_ATTACK));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_MOVE, CTRL_ATTACK));
}

BEGIN_ENUM_DESCRIPTOR(InterfaceGameControlID, "InterfaceGameControlID")
REGISTER_ENUM(CTRL_TIME_NORMAL, "���������� �������� ����")
REGISTER_ENUM(CTRL_TIME_DEC, "��������� ����")
REGISTER_ENUM(CTRL_TIME_INC, "�������� ����")
REGISTER_ENUM(CTRL_CAMERA_MOVE_UP, "�������� ������ �����")
REGISTER_ENUM(CTRL_CAMERA_MOVE_DOWN, "�������� ������ ����")
REGISTER_ENUM(CTRL_CAMERA_MOVE_LEFT, "�������� ������ �����")
REGISTER_ENUM(CTRL_CAMERA_MOVE_RIGHT, "�������� ������ ������")
REGISTER_ENUM(CTRL_CAMERA_ROTATE_UP, "���������� ������ �����")
REGISTER_ENUM(CTRL_CAMERA_ROTATE_DOWN, "���������� ������ ����")
REGISTER_ENUM(CTRL_CAMERA_ROTATE_LEFT, "���������� ������ �����")
REGISTER_ENUM(CTRL_CAMERA_ROTATE_RIGHT, "���������� ������ ������")
REGISTER_ENUM(CTRL_CAMERA_ZOOM_INC, "���������� ������")
REGISTER_ENUM(CTRL_CAMERA_ZOOM_DEC, "���������� ������")
REGISTER_ENUM(CTRL_CAMERA_MOUSE_LOOK, "������� ������ �����")
REGISTER_ENUM(CTRL_CAMERA_MAP_SHIFT, "������� ������ �����")
REGISTER_ENUM(CTRL_DIRECT_UP, "������ ���������� - ������")
REGISTER_ENUM(CTRL_DIRECT_DOWN, "������ ���������� - �����")
REGISTER_ENUM(CTRL_DIRECT_LEFT, "������ ���������� - �����")
REGISTER_ENUM(CTRL_DIRECT_RIGHT, "������ ���������� - ������")
REGISTER_ENUM(CTRL_DIRECT_STRAFE_LEFT, "������ ���������� - ������ �����")
REGISTER_ENUM(CTRL_DIRECT_STRAFE_RIGHT, "������ ���������� - ������ ������")
REGISTER_ENUM(CTRL_PAUSE, "�����")
REGISTER_ENUM(CTRL_MAKESHOT, "��������� �����")
REGISTER_ENUM(CTRL_SHOW_UNIT_PARAMETERS, "�������� ��������� ������")
REGISTER_ENUM(CTRL_SHOW_ITEMS_HINT, "�������� ������� ��������")
REGISTER_ENUM(CTRL_TOGGLE_MUSIC, "���/���� ������")
REGISTER_ENUM(CTRL_TOGGLE_SOUND, "���/���� �������� �������")
REGISTER_ENUM(CTRL_LOAD, "��������� �����")
REGISTER_ENUM(CTRL_SAVE, "��������� �����")
REGISTER_ENUM(CTRL_CLICK_ACTION, "���� �� ����")
REGISTER_ENUM(CTRL_ATTACK, "���������")
REGISTER_ENUM(CTRL_ATTACK_ALTERNATE, "��������� �������������� �������")
REGISTER_ENUM(CTRL_SELECT, "��������")
REGISTER_ENUM(CTRL_SELECT_ALL, "�������� ����")
REGISTER_ENUM(CTRL_MOVE, "����")
REGISTER_ENUM(CTRL_MAX, "������������")
REGISTER_ENUM(CTRL_CAMERA_SAVE1, "��������� ��������� ������ 1")
REGISTER_ENUM(CTRL_CAMERA_SAVE2, "��������� ��������� ������ 2")
REGISTER_ENUM(CTRL_CAMERA_SAVE3, "��������� ��������� ������ 3")
REGISTER_ENUM(CTRL_CAMERA_SAVE4, "��������� ��������� ������ 4")
REGISTER_ENUM(CTRL_CAMERA_SAVE5, "��������� ��������� ������ 5")
REGISTER_ENUM(CTRL_CAMERA_RESTORE1, "������������ ��������� ������ 1")
REGISTER_ENUM(CTRL_CAMERA_RESTORE2, "������������ ��������� ������ 2")
REGISTER_ENUM(CTRL_CAMERA_RESTORE3, "������������ ��������� ������ 3")
REGISTER_ENUM(CTRL_CAMERA_RESTORE4, "������������ ��������� ������ 4")
REGISTER_ENUM(CTRL_CAMERA_RESTORE5, "������������ ��������� ������ 5")
REGISTER_ENUM(CTRL_CAMERA_TO_EVENT, "����������� ������ �� �������")
END_ENUM_DESCRIPTOR(InterfaceGameControlID)

