#include "StdAfx.h"
#include "Controls.h"

#include "Serialization.h"

WRAP_LIBRARY(ControlManager, "ControlManager", "ControlManager", "Scripts\\Content\\Controls", 0, false);

ControlManager::ControlManager()
{
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

const sKey& ControlManager::key(InterfaceGameControlID ctrl) const
{
	ControlNodeList::const_iterator it = find(controls_.begin(), controls_.end(), ctrl);
	if(it != controls_.end())
		return it->key_;

	static sKey empty;
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

void ControlManager::unRegisterHotKey(CallWrapperBase* func)
{
	MTAuto autolock(updateLock_);

	HotKeyList::iterator it;
	FOR_EACH(hotKeyHandlers_, it)
		if(it->handler_ == func){
			hotKeyHandlers_.erase(it);
			break;
		}
}

void ControlManager::registerHotKey(int fullKey, CallWrapperBase* func)
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

void ControlManager::setKey(InterfaceGameControlID ctrl, const sKey& key)
{
	ControlNodeList::iterator it = find(controls_.begin(), controls_.end(), ctrl);
	if(it == controls_.end())
		controls_.push_back(ControlNode(ctrl, key));
	else
		it->key_ = key;
}

void ControlManager::defaultSettings()
{
	setKey(CTRL_CAMERA_MOVE_UP, sKey(VK_UP));
	setKey(CTRL_CAMERA_MOVE_DOWN, sKey(VK_DOWN));
	setKey(CTRL_CAMERA_MOVE_LEFT, sKey(VK_LEFT));
	setKey(CTRL_CAMERA_MOVE_RIGHT, sKey(VK_RIGHT));
	setKey(CTRL_CAMERA_ROTATE_UP, sKey(VK_HOME));
	setKey(CTRL_CAMERA_ROTATE_DOWN, sKey(VK_END));
	setKey(CTRL_CAMERA_ROTATE_LEFT, sKey(VK_DELETE));
	setKey(CTRL_CAMERA_ROTATE_RIGHT, sKey(VK_NEXT));
	setKey(CTRL_CAMERA_ZOOM_INC, sKey(VK_ADD));
	setKey(CTRL_CAMERA_ZOOM_DEC, sKey(VK_SUBTRACT));
	setKey(CTRL_CAMERA_MOUSE_LOOK, sKey(VK_MBUTTON));
	setKey(CTRL_DIRECT_UP, sKey(VK_NUMPAD8));
	setKey(CTRL_DIRECT_DOWN, sKey(VK_NUMPAD5));
	setKey(CTRL_DIRECT_LEFT, sKey(VK_NUMPAD4));
	setKey(CTRL_DIRECT_RIGHT, sKey(VK_NUMPAD6));
	setKey(CTRL_DIRECT_STRAFE_LEFT, sKey(VK_NUMPAD7));
	setKey(CTRL_DIRECT_STRAFE_RIGHT, sKey(VK_NUMPAD9));
	setKey(CTRL_PAUSE, sKey(VK_PAUSE));
	setKey(CTRL_MAKESHOT, sKey(VK_F11));
	setKey(CTRL_SHOW_UNIT_PARAMETERS, sKey(VK_MENU));
	setKey(CTRL_TOGGLE_MUSIC, sKey(KBD_SHIFT|'M'));
	setKey(CTRL_TOGGLE_SOUND, sKey(KBD_SHIFT|'S'));
	setKey(CTRL_TIME_NORMAL, sKey(KBD_SHIFT|'A'));
	setKey(CTRL_TIME_DEC, sKey('Z'));
	setKey(CTRL_TIME_INC, sKey('A'));
	
	setKey(CTRL_CLICK_ACTION, sKey(VK_LBUTTON));
	setKey(CTRL_ATTACK, sKey(VK_RBUTTON));
	setKey(CTRL_SELECT_ALL, sKey(VK_LDBL));
	setKey(CTRL_SELECT, sKey(VK_LBUTTON));
	setKey(CTRL_MOVE, sKey(VK_RBUTTON));

	setKey(CTRL_KILL_UNIT, sKey('D'));

	//setKey(CTRL_LOAD, sKey(KBD_CTRL|'O'));
	//setKey(CTRL_SAVE, sKey(KBD_CTRL|'S'));

	//setKey(CTRL_CAMERA_RESTORE1, sKey(VK_F1));
	//setKey(CTRL_CAMERA_RESTORE2, sKey(VK_F2));
	//setKey(CTRL_CAMERA_RESTORE3, sKey(VK_F3));
	//setKey(CTRL_CAMERA_RESTORE4, sKey(VK_F4));
	//setKey(CTRL_CAMERA_RESTORE5, sKey(VK_F5));
	//setKey(CTRL_CAMERA_SAVE1, sKey(VK_F1|KBD_SHIFT));
	//setKey(CTRL_CAMERA_SAVE2, sKey(VK_F2|KBD_SHIFT));
	//setKey(CTRL_CAMERA_SAVE3, sKey(VK_F3|KBD_SHIFT));
	//setKey(CTRL_CAMERA_SAVE4, sKey(VK_F4|KBD_SHIFT));
	//setKey(CTRL_CAMERA_SAVE5, sKey(VK_F5|KBD_SHIFT));
	
	//setKey(CTRL_CAMERA_TO_EVENT, sKey(VK_SPACE));

	compatibleControls_.reserve(16);

	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CAMERA_MOVE_UP, CTRL_DIRECT_UP));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CAMERA_MOVE_DOWN, CTRL_DIRECT_DOWN));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CAMERA_MOVE_LEFT, CTRL_DIRECT_LEFT));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CAMERA_MOVE_RIGHT, CTRL_DIRECT_RIGHT));

	compatibleControls_.push_back(CompatibleControlsNode(CTRL_SELECT, CTRL_CLICK_ACTION));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_SELECT, CTRL_ATTACK));
	compatibleControls_.push_back(CompatibleControlsNode(CTRL_CLICK_ACTION, CTRL_ATTACK));
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
REGISTER_ENUM(CTRL_TOGGLE_MUSIC, "���/���� ������")
REGISTER_ENUM(CTRL_TOGGLE_SOUND, "���/���� �������� �������")
REGISTER_ENUM(CTRL_LOAD, "��������� �����")
REGISTER_ENUM(CTRL_SAVE, "��������� �����")
REGISTER_ENUM(CTRL_CLICK_ACTION, "���� �� ����")
REGISTER_ENUM(CTRL_ATTACK, "���������")
REGISTER_ENUM(CTRL_SELECT, "��������")
REGISTER_ENUM(CTRL_SELECT_ALL, "�������� ����")
REGISTER_ENUM(CTRL_MOVE, "����")
REGISTER_ENUM(CTRL_KILL_UNIT, "����� ���������� ������")
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

