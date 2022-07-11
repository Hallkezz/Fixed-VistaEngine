#include "StdAfx.h"

#include "FileUtils\FileUtils.h"
#include "Serialization\Factory.h"
#include "Serialization\Serialization.h"
#include "Serialization\XPrmArchive.h"
#include "Serialization\ResourceSelector.h"
#include "Serialization\SerializationFactory.h"
#include "Serialization\StringTableImpl.h"
#include "Serialization\Decorators.h"
#include "WBuffer.h"

#include "Sound.h"
#include "PlayOgg.h"
#include "Game\SoundApp.h"
#include "Units\UnitAttribute.h"

#include "UI_Render.h"
#include "UI_Types.h"
#include "UI_Effect.h"
#include "UserInterface.h"
#include "UI_Controls.h"

#include "UI_Actions.h"
#include "UI_Inventory.h"
#include "FileUtils\FileUtils.h"
#include "Util\Serialization\RangedWrapper.h"

BEGIN_ENUM_DESCRIPTOR(ActivationMove, "ActivationMove")
REGISTER_ENUM(ACTIVATION_MOVE_LEFT, "�����");
REGISTER_ENUM(ACTIVATION_MOVE_BOTTOM, "�����");
REGISTER_ENUM(ACTIVATION_MOVE_RIGHT, "������");
REGISTER_ENUM(ACTIVATION_MOVE_TOP, "������");
REGISTER_ENUM(ACTIVATION_MOVE_CENTER, "�� ������");
END_ENUM_DESCRIPTOR(ActivationMove)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(UI_ControlBase, ResizeShiftHorizontal, "UI_ControlBase::ResizeShiftHorizontal")
REGISTER_ENUM_ENCLOSED(UI_ControlBase, RESIZE_SHIFT_LEFT, "�����");
REGISTER_ENUM_ENCLOSED(UI_ControlBase, RESIZE_SHIFT_CENTER_H, "� �����");
REGISTER_ENUM_ENCLOSED(UI_ControlBase, RESIZE_SHIFT_RIGHT, "������");
END_ENUM_DESCRIPTOR_ENCLOSED(UI_ControlBase, ResizeShiftHorizontal)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(UI_ControlBase, ResizeShiftVertical, "UI_ControlBase::ResizeShiftVertical")
REGISTER_ENUM_ENCLOSED(UI_ControlBase, RESIZE_SHIFT_UP, "�����");
REGISTER_ENUM_ENCLOSED(UI_ControlBase, RESIZE_SHIFT_CENTER_V, "� �����");
REGISTER_ENUM_ENCLOSED(UI_ControlBase, RESIZE_SHIFT_DOWN, "����");
END_ENUM_DESCRIPTOR_ENCLOSED(UI_ControlBase, ResizeShiftVertical)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(UI_Transform, Type, "UI_Transform::Type")
REGISTER_ENUM_ENCLOSED(UI_Transform, TRANSFORM_COORDS, "�����������")
REGISTER_ENUM_ENCLOSED(UI_Transform, TRANSFORM_SCALE_X, "��������������� �� �����������")
REGISTER_ENUM_ENCLOSED(UI_Transform, TRANSFORM_SCALE_Y, "��������������� �� ���������")
REGISTER_ENUM_ENCLOSED(UI_Transform, TRANSFORM_ALPHA, "������������")
END_ENUM_DESCRIPTOR_ENCLOSED(UI_Transform, Type)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(UI_Transform, ScaleMode, "UI_Transform::ScaleMode")
REGISTER_ENUM_ENCLOSED(UI_Transform, SCALE_CENTER, "�� ������")
REGISTER_ENUM_ENCLOSED(UI_Transform, SCALE_LEFT, "�� ������ ����")
REGISTER_ENUM_ENCLOSED(UI_Transform, SCALE_TOP, "�� �������� ����")
REGISTER_ENUM_ENCLOSED(UI_Transform, SCALE_RIGHT, "�� ������� ����")
REGISTER_ENUM_ENCLOSED(UI_Transform, SCALE_BOTTOM, "�� ������� ����")
END_ENUM_DESCRIPTOR_ENCLOSED(UI_Transform, ScaleMode)

REGISTER_CLASS(UI_Text, UI_Text, "�����");
WRAP_LIBRARY(UI_TextLibrary, "UI_TextLibrary", "���������", "Scripts\\Content\\UI_TextLibrary", 0, LIBRARY_EDITABLE);

REGISTER_CLASS(UI_Font, UI_Font, "�����");
WRAP_LIBRARY(UI_FontLibrary, "UI_FontLibrary", "������", "Scripts\\Content\\UI_FontAttributes", 0, LIBRARY_EDITABLE);

REGISTER_CLASS(UI_Cursor, UI_Cursor, "������");
WRAP_LIBRARY(UI_CursorLibrary, "UI_CursorLibrary", "�������", "Scripts\\Content\\UI_CursorLibrary", 0, LIBRARY_EDITABLE);

WRAP_LIBRARY(UI_ShowModeSpriteTable, "UI_ShowModeSpriteTable", "������� ��� ������", "Scripts\\Content\\UI_ShowModeSpriteTable", 0, LIBRARY_EDITABLE);

REGISTER_CLASS(UI_SpriteLibrary, UI_SpriteLibrary, "������");
WRAP_LIBRARY(UI_SpriteLibrary, "UI_SpriteLibrary", "�������� ��� ������� � �����", "Scripts\\Content\\UI_TextSprite", 0, LIBRARY_EDITABLE);

REGISTER_CLASS(UI_MessageTypeLibrary, UI_MessageTypeLibrary, "��� ���������");
WRAP_LIBRARY(UI_MessageTypeLibrary, "UI_MessageTypeLibrary", "���� ��������� ���������", "Scripts\\Content\\UI_MessageTypes", 0, LIBRARY_EDITABLE);

REGISTER_CLASS(UI_ControlShowMode, UI_ControlShowMode, "����� ���������");

REGISTER_CLASS(UI_EffectAttribute, UI_EffectAttribute, "����������� ������");

REGISTER_CLASS(UI_ControlBase, UI_ControlButton, "������� ������");
REGISTER_CLASS(UI_ControlBase, UI_ControlSlider, "���������");
REGISTER_CLASS(UI_ControlBase, UI_ControlTab, "����� �������");
REGISTER_CLASS(UI_ControlBase, UI_ControlStringList, "������");
REGISTER_CLASS(UI_ControlBase, UI_ControlStringCheckedList, "������ � ���������");
REGISTER_CLASS(UI_ControlBase, UI_ControlComboList, "���������� ������");
REGISTER_CLASS(UI_ControlBase, UI_ControlEdit, "���� �����");
REGISTER_CLASS(UI_ControlBase, UI_ControlHotKeyInput, "���� ������� �������");
REGISTER_CLASS(UI_ControlBase, UI_ControlProgressBar, "��������� ���������");
REGISTER_CLASS(UI_ControlBase, UI_ControlCustom, "����������� ������");
REGISTER_CLASS(UI_ControlBase, UI_ControlTextList, "������ �������");
REGISTER_CLASS(UI_ControlBase, UI_ControlUnitList, "������ ������");
REGISTER_CLASS(UI_ControlBase, UI_ControlCustomList, "������ ���������");
REGISTER_CLASS(UI_ControlBase, UI_ControlInventory, "���������");
REGISTER_CLASS(UI_ControlBase, UI_ControlVideo, "�����");

// ------------------- UI_Text

UI_Text::UI_Text()
{
}

void UI_Text::serialize(Archive& ar)
{
	ar.serialize(text_, "text", "&����� (���)");
	ar.serialize(voice_, "voiceAttribute", "��������� ���������");
}

// ------------------- UI_Mask

UI_Mask::UI_Mask()
{
}

UI_Mask::~UI_Mask()
{
}

void UI_Mask::serialize(Archive& ar)
{
	ar.serialize(polygon_, "polygon", "������");
}

bool UI_Mask::hitTest(const Vect2f& p) const
{
	int intersections_lt = 0;
	int intersections_gt = 0;

	for(int i = 0; i < polygon_.size(); i ++){
		Vect2f p0 = polygon_[i];
		Vect2f p1 = (i < polygon_.size() - 1) ? polygon_[i + 1] : polygon_[0];

		if(p0.y != p1.y){
			if((p0.y < p.y && p1.y >= p.y) || (p0.y >= p.y && p1.y < p.y)){
				if(p0.x < p.x && p1.x < p.x)
					intersections_lt++;
				else if(p0.x > p.x && p1.x > p.x)
					intersections_gt++;
				else {
					float x = (p.y - p0.y) * (p1.x - p0.x) / (p1.y - p0.y) + p0.x;

					if(x > p.x)
						intersections_gt++;
					else
						intersections_lt++;
				}
			}
		}
	}

	return ((intersections_lt & 1) && intersections_gt != 0);
}

void UI_Mask::draw(const Vect2f& base_pos, const Color4f& color) const
{
	for(int i = 0; i < polygon_.size() - 1; i++)
		UI_Render::instance().drawLine(base_pos + polygon_[i], base_pos + polygon_[i + 1], color);

	UI_Render::instance().drawLine(base_pos + polygon_[polygon_.size() - 1], base_pos + polygon_[0], color);
}

// ---------------------- UI_Cursor

UI_Cursor::UI_Cursor() :
	cursor_(NULL),
	fileName_("")
{
}

UI_Cursor::~UI_Cursor()
{
	releaseCursor();
}

void UI_Cursor::serialize(Archive& ar)
{
	static ResourceSelector::Options options("*.cur", "Resource\\Cursors", "Cursors");
	ar.serialize(ResourceSelector(fileName_, options), "fileName", "��� �����");
	ar.serialize(effectRef_, "effect", "������");
}

bool UI_Cursor::createCursor(const char* fname/*=*/)
{
	releaseCursor();
	// ��������� ���� � ����� ������ ���� �� �� NULL
	if (NULL != fname) fileName_ = fname;
	if (fileName_.empty()) return true; // ���� ������ => ������ NULL - ��� ��� ����

	releaseCursor();
	cursor_ = (HCURSOR)LoadImage(0, fileName_.c_str(), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);

	return (NULL != cursor_);
}

void UI_Cursor::releaseCursor()
{
	if (NULL != cursor_) 
	{
		DestroyCursor(cursor_);
		cursor_ = NULL;
	}
}

// ------------------- UI_TextFormat

UI_TextFormat UI_TextFormat::WHITE(Color4c(255, 255, 255, 255));

UI_TextFormat::UI_TextFormat()
{
	textColor_ = Color4c(255, 255, 255, 255);
	shadowColor_ = Color4c(0, 0, 0, 0);
}

UI_TextFormat::UI_TextFormat(const Color4c& color, const Color4c& shadow)
: textColor_(color)
, shadowColor_(shadow)
{}

void UI_TextFormat::serialize(Archive& ar)
{
	ar.serialize(textColor_, "textColor", "���� ������");
	ar.serialize(shadowColor_, "shadowColor", "���� ����");
}

// ------------------- UI_ControlShowMode

void UI_ControlShowMode::ActivateSound::serialize(Archive& ar)
{
	ar.serialize(sourceMode_, "sourceModes", "�� ������ �������������");
	ar.serialize(soundReference_, "soundReference", "���� ���������");
}

UI_ControlShowMode::UI_ControlShowMode()
{
	effect_ = 0;
	addRefFlag_ = false;
	spriteTiled_ = false;
}

void UI_ControlShowMode::preLoad()
{
	if(!addRefFlag_){
		sprite_.addRef();
		addRefFlag_ = true;
	}
}

void UI_ControlShowMode::release()
{
	if(addRefFlag_){
		sprite_.decRef();
		sprite_.release();
		addRefFlag_ = false;
	}
}

void UI_ControlShowMode::serialize(Archive& ar)
{
	ar.serialize(sprite_, "sprite_", "<��������");
	ar.serialize(spriteTiled_, "spriteTiled", "��������� ��������");
	if(ar.isEdit()){
		ar.serialize(static_cast<OptionalPtr<UI_EffectAttribute>&>(effect_), "effect", "������");
		textFormat_.serialize(ar);
	}
	else{
		ar.serialize(effect_, "effect", "������");
		ar.serialize(textFormat_, "textFormat", "��������� ������");
	}
	ar.serialize(activateSounds_, "activateSounds", "����� ���������");
}

const SoundAttribute* UI_ControlShowMode::sound(UI_ControlShowModeID srcID) const
{
	ActivateSounds::const_iterator it = find(activateSounds_.begin(), activateSounds_.end(), srcID);
	if(it != activateSounds_.end())
		return it->soundReference_;
	return 0;
}

// ------------------- UI_ControlState


UI_ControlState::UI_ControlState(const char* name, bool withDefaultShowMode)
: name_(name)
{
	//showModes_.resize(getEnumDescriptor(UI_SHOW_NORMAL).comboStrings().size());
	if(withDefaultShowMode)
		showModes_[UI_SHOW_NORMAL] = new UI_ControlShowMode();
}

void UI_ControlState::preLoad()
{
	ShowModes::iterator it;
	FOR_EACH(showModes_, it)
		if(UI_ControlShowMode* sm = *it)
			sm->preLoad();
}

void UI_ControlState::release()
{
	ShowModes::iterator it;
	FOR_EACH(showModes_, it)
		if(UI_ControlShowMode* sm = *it)
			sm->release();
}

void UI_ControlState::serialize(Archive& ar)
{
	ar.serialize(name_, "name_", "&���");


	if(ar.isEdit()){
		typedef EnumTable<UI_ControlShowModeID, OptionalPtr<UI_ControlShowMode> > ShowModesOptional;
		reinterpret_cast<ShowModesOptional&>(showModes_).serialize(ar); // HINT
	}
	else
		ar.serialize(showModes_, "showModes", "������ ���������");
	ar.serialize(actions_, "actions", "����������");
}


bool UI_ControlState::operator == (const UI_DataStateMark& mark) const
{
	UI_ControlActionList::const_iterator it;
	FOR_EACH(actions_, it)
		if(it->key() == UI_ACTION_STATE_MARK 
			&& safe_cast<const UI_DataStateMark*>(it->type())->type() == mark.type()
			&& safe_cast<const UI_DataStateMark*>(it->type())->enumValue() == mark.enumValue())
			return true;
	return false;
}

void UI_ControlState::setSprites(const UI_ShowModeSprite& sprites)
{
	UI_ControlShowMode* mode = showModes_[UI_SHOW_NORMAL];
	if(mode)
		mode->setSprite(sprites.sprite(UI_SHOW_NORMAL));
	
	if(mode = showModes_[UI_SHOW_DISABLED])
		mode->setSprite(sprites.sprite(UI_SHOW_DISABLED));
	
	if(mode = showModes_[UI_SHOW_HIGHLITED])
		mode->setSprite(sprites.sprite(UI_SHOW_HIGHLITED));
	
	if(mode = showModes_[UI_SHOW_ACTIVATED])
		mode->setSprite(sprites.sprite(UI_SHOW_ACTIVATED));
}

// ------------------- UI_ShowModeSprite

const UI_ShowModeSprite UI_ShowModeSprite::EMPTY;

void UI_ShowModeSprite::StateSprite::serialize(Archive& ar)
{
	ar.serialize(mode_, "mode", "&����� ������");
	ar.serialize(sprite_, "sprite", "������");
}

void UI_ShowModeSprite::serialize(Archive& ar)
{
	ar.serialize(sprites_, "sprites", "�������");
}

void UI_ShowModeSprite::preLoad()
{
	StateSprites::iterator it;
	FOR_EACH(sprites_, it)
		it->sprite_.addRef();
}

void UI_ShowModeSprite::release()
{
	StateSprites::iterator it;
	FOR_EACH(sprites_, it){
		it->sprite_.decRef();
		it->sprite_.release();
	}
}

// ------------------- UI_Transform

const UI_Transform UI_Transform::ID;

UI_Transform::UI_Transform(float time, bool reversed) :
	type_(0),
	workTime_(time),
	trans_(0,0),
	scale_(1,1),
	scaleMode_(SCALE_CENTER),
	alpha_(1.f),
	phase_(0),
	reversed_(reversed)
{
	if(workTime_ < FLT_EPS)
		phase_ = 1.f;
}

void UI_Transform::clear()
{
	*this = ID;
}

void UI_Transform::quant(float dt)
{
	if(workTime_ > FLT_EPS)
		phase_ = clamp(phase_ + dt/workTime_, 0.f, 1.f);
	else
		phase_ = 1.f;
}

void UI_Transform::apply(UI_ControlBase* control, bool append)
{
	if(checkType(UI_Transform::TRANSFORM_COORDS))
		control->transfPosition_ = (append ? control->transfPosition_ : control->position_) + trans() * phase();
	else if(!append)
		control->transfPosition_ = control->position_;

	if(checkType(UI_Transform::TRANSFORM_SCALE)){
		Vect2f center = control->transfPosition_.center();
		Vect2f scal = append ? Vect2f::ID + (scale() - Vect2f::ID) * phase() : scale() * (1.f - phase());

		if(!checkType(UI_Transform::TRANSFORM_SCALE_X))
			scal.x = 1.f;
		if(!checkType(UI_Transform::TRANSFORM_SCALE_Y))
			scal.y = 1.f;

		control->transfPosition_ = control->transfPosition_.scaled(scal, center);

		if(!append){
			switch(scaleMode_){
				case SCALE_LEFT:
					control->transfPosition_.left(control->transfPosition_.left() - (control->position_.width() - control->transfPosition_.width()) / 2.f);
					break;
				case SCALE_TOP:
					control->transfPosition_.top(control->transfPosition_.top() - (control->position_.height() - control->transfPosition_.height()) / 2.f);
					break;
				case SCALE_RIGHT:
					control->transfPosition_.left(control->transfPosition_.left() + (control->position_.width() - control->transfPosition_.width()) / 2.f);
					break;
				case SCALE_BOTTOM:
					control->transfPosition_.top(control->transfPosition_.top() + (control->position_.height() - control->transfPosition_.height()) / 2.f);
					break;
			}
		}

		if(append)
			control->isScaled_ |= !isFinished();
		else
			control->isScaled_  = !isFinished();
	}

	if(checkType(UI_Transform::TRANSFORM_ALPHA)){
		if(append)
			control->alpha_ *= alpha() * phase();
		else
			control->alpha_  = 1.f - alpha() * phase();
	}
	else {
		if(!append)
			control->alpha_ = 1.f;
	}
}

void UI_Transform::getDebugString(XBuffer& out) const
{
	out < "type: " < getEnumDescriptor(Type()).nameCombination(type_).c_str()
		< (reversed_ ? "  Reversed, " : "  ")
		< ", worktime:" <= workTime_ < ", phase: " <= phase()
		< (isFinished() ? "" : ", (in work)");
}

// ------------------- UI_MessageSetup

UI_MessageSetup::UI_MessageSetup()
{
	syncroBySound_ = false;
	displayTime_ = 0.f;
	isVoiceInterruptable_ = false;
	enableVoice_ = true;
	isInterruptOnDestroy_ = true;
	isCanInterruptVoice_ = true;
	isCanPaused_ = true;
	isPlayVoiceAlways_ = false;
}

float UI_MessageSetup::displayTime() const
{
	if(syncroBySound_){
		if(const UI_Text* txt = textReference_)
			return txt->voice().duration();
	}

	return displayTime_;
}

const wchar_t* UI_MessageSetup::textData() const
{
	if(const UI_Text* txt = textReference_)
		return txt->text();

	return 0;
}

bool UI_MessageSetup::playVoice(bool interrupt) const
{
	if((!voiceManager().enabled() && !isPlayVoiceAlways_) || (!interrupt && voiceManager().isPlaying()))
		return false;

	if(const UI_Text* txt = textReference_){
		const VoiceAttribute& v = txt->voice();
		if(!v.isEmpty() && enableVoice_){
			v.getNextVoiceFile();
			return voiceManager().Play(v.voiceFile(), false, isCanPaused_, isPlayVoiceAlways_);
		}
	}

	return false;
}

void UI_MessageSetup::stopVoice() const
{
	if(!voiceManager().enabled() && !isPlayVoiceAlways_)
		return;

	voiceManager().Stop();
}

float UI_MessageSetup::voiceDuration() const
{
	if(const UI_Text* txt = textReference_){
		const VoiceAttribute& v = txt->voice();
		if(!v.isEmpty())
			return v.duration();
	}

	return 0.f;
}

void UI_MessageSetup::serialize(Archive& ar)
{
	ar.serialize(messageType_, "messageType", "��� ���������");
	ar.serialize(textReference_, "textReference", "���������");
	ar.serialize(syncroBySound_, "syncroBySound", "���������������� �� ������");
	ar.serialize(isVoiceInterruptable_, "isVoiceInterruptable", "����� �������� �����");
	ar.serialize(isInterruptOnDestroy_, "isInterruptOnDestroy", "��������� ��� ���������� ����");
	ar.serialize(isCanInterruptVoice_, "isCanInterruptVoice", "�� ����� ��������� ��������� ���������");
	ar.serialize(isCanPaused_, "isCanPaused", "������� �� ����� �� ����� �����");
	ar.serialize(isPlayVoiceAlways_, "isPlayVoiceAlways", "����������� ���� ������");
    if(!syncroBySound_){
		ar.serialize(displayTime_, "displayTime", "����� ������, �������");
		ar.serialize(enableVoice_, "enableVoice", "����������� ����");
	}		
}

// ------------------- UI_Message

UI_Message::UI_Message()
{
}

UI_Message::UI_Message(const UI_MessageSetup& message_setup)
: setup_(message_setup)
{
	alpha_ = 1.f;
	start();
}

void UI_Message::setMessagePrefix(const wchar_t* prefix)
{
	if(prefix && *prefix)
		customPrefix_ = prefix;
	else
		customPrefix_.clear();
}

bool UI_Message::getText(wstring& out) const
{
	out.clear();

	if(setup_.hasText()){
		if(alpha_ < 0.998f)
			out = (WBuffer() < L"<alpha=" <= round(alpha_ * 255.f) < L">").c_str();
		out += customPrefix_;
		out += setup_.textData();
		return true;
	}
	return false;
}

void UI_Message::start()
{
	if(!setup_.isEmpty()){
		float time = setup_.displayTime();
		if(time > FLT_EPS)
			aliveTime_.start(time * 1000);
		else
			aliveTime_.start(10000000.f);
	}
}

void UI_Message::serialize(Archive& ar)
{
	ar.serialize(alpha_, "alpha", 0);
	ar.serialize(aliveTime_, "aliveTimer", 0);
	ar.serialize(customPrefix_, "customText", 0);
	ar.serialize(setup_, "setup", 0);
}

// -------------------

void UI_TaskColor::serialize(Archive& ar)
{
	ar.serialize(color, "color", "���� ������");
	ar.serialize(tag, "tag", "������� ������");
}

// ------------------- UI_Task

UI_TaskColor UI_Task::taskColors_[UI_TASK_STATE_COUNT * 4];

void UI_Task::serializeColors(Archive& ar)
{
	if(ar.isEdit()){
		if(ar.openBlock("taskColors", "���� ������ �����")){
			for(int i = 0; i < UI_TASK_STATE_COUNT; i++)
				ar.serialize(taskColors_[i], getEnumName(UI_TaskStateID(i)), getEnumNameAlt(UI_TaskStateID(i)));
			ar.closeBlock();
		}

		if(ar.openBlock("secondaryTaskColors", "���� ������ �������������� �����")){
			for(int i = 0; i < UI_TASK_STATE_COUNT; i++)
				ar.serialize(taskColors_[i + UI_TASK_STATE_COUNT], getEnumName(UI_TaskStateID(i)), getEnumNameAlt(UI_TaskStateID(i)));
			ar.closeBlock();
		}

		if(ar.openBlock("taskColorsOnScreen", "���� ������ ����� � ������ ���������")){
			for(int i = 0; i < UI_TASK_STATE_COUNT; i++)
				ar.serialize(taskColors_[i + UI_TASK_STATE_COUNT * 2], getEnumName(UI_TaskStateID(i)), getEnumNameAlt(UI_TaskStateID(i)));
			ar.closeBlock();
		}

		if(ar.openBlock("secondaryTaskColorsOnScreen", "���� ������ �������������� ����� � ������ ���������")){
			for(int i = 0; i < UI_TASK_STATE_COUNT; i++)
				ar.serialize(taskColors_[i + UI_TASK_STATE_COUNT * 3], getEnumName(UI_TaskStateID(i)), getEnumNameAlt(UI_TaskStateID(i)));
			ar.closeBlock();
		}
	}
	else
		ar.serializeArray(taskColors_, "taskColors", 0);
}

void UI_Task::serialize(Archive& ar)
{
	ar.serialize(state_, "state", 0);
	ar.serialize(setup_, "setup", 0);
	ar.serialize(isSecondary_, "isSecondary", 0);
}

const wchar_t* getColorString(const Color4f& color);

bool UI_Task::getMessagePrefix(wstring& out) const
{
	out.clear();

	if(state_ == UI_TASK_DELETED)
		return false;

	if(const wchar_t* message = setup_.textData()){
		int col_idx = int(state_);
		if(isSecondary_)
			col_idx += UI_TASK_STATE_COUNT;
		col_idx += UI_TASK_STATE_COUNT * 2;

		out = getColorString(taskColors_[col_idx].color);

		if(!taskColors_[col_idx].tag.empty())
			out += taskColors_[col_idx].tag.c_str();

		return true;
	}
	return false;
}

bool UI_Task::getText(wstring& out, bool message_mode) const
{
	out.clear();

	if(state_ == UI_TASK_DELETED)
		return false;

	if(const wchar_t* message = setup_.textData()){
		int col_idx = int(state_);
		if(isSecondary_)
			col_idx += UI_TASK_STATE_COUNT;

		if(message_mode)
			col_idx += UI_TASK_STATE_COUNT * 2;

		out += getColorString(taskColors_[col_idx].color);
		
		if(!taskColors_[col_idx].tag.empty())
			out += taskColors_[col_idx].tag.c_str();

		out += message;
		
		return true;
	}
	return false;
}

// --------------------- AtomAction

void AtomAction::serialize(Archive& ar)
{
	ar.serialize(type_, "type", "��� �������");
	ar.serialize(control_, "control", "� ��� �������");
}

void AtomAction::apply() const
{
	if(UI_ControlBase* ctrl = control_.control()){
		switch(type_){
		case ENABLE_SHOW:
			ctrl->showByTrigger();
			break;
		case DISABLE_SHOW:
			ctrl->hideByTrigger();
			break;
		case SHOW:
			if(!ctrl->isVisible())
				ctrl->show();
			break;
		case HIDE:
			if(ctrl->isVisible())
				ctrl->hide();
			break;
		case ENABLE:
			if(!ctrl->isEnabled())
				ctrl->enable();
			break;
		case DISABLE:
			if(ctrl->isEnabled())
				ctrl->disable();
			break;
		case VIDEO_PLAY:
			if(UI_ControlVideo* vid = dynamic_cast<UI_ControlVideo*>(ctrl))
				vid->play();
			break;
		case VIDEO_PAUSE:
			if(UI_ControlVideo* vid = dynamic_cast<UI_ControlVideo*>(ctrl))
				vid->pause();
			break;
		case SET_FOCUS:
			ctrl->activate();
			break;
		default:
			xassert(false && "������ �������� case AtomAction::apply()");
		}
	}
}

bool AtomAction::workedOut() const
{
	if(const UI_ControlBase* ctrl = control_.control())
		switch(type_){
		case ENABLE_SHOW:
		case SHOW:
		case DISABLE_SHOW:
		case HIDE:
			if(!UI_Dispatcher::instance().isActive(ctrl->screen()))
				return true;
			return !ctrl->isChangeProcess();
		}
	return true;
}

// --------------------- ShowStatisticType

void ShowStatisticType::serialize(Archive& ar)
{
	ar.serialize(type, "type", "��� ��������");
	if(type == STAT_VALUE)
		ar.serialize(statValueType, "statValueType", "��������");
}

void ShowStatisticType::getValue(WBuffer& buf, const StatisticsEntry& val) const
{
	switch(type){
	case POSITION:
		buf <= val.position;
		break;
	case NAME:
		buf < val.name.c_str();
		break;
	case RATING:
		buf <= round(double(val.rating));
		break;
	case STAT_VALUE:
		buf <= val[statValueType];
		break;
	default:
		break;
	}
}

// --------------------------------------------------------------------------

HintAttributes::HintAttributes()
{
	tipFill = Color4f::BLACK;
	tipBorder = Color4f::WHITE;
	tipTextMaxWidth = 0.25;
}

void HintAttributes::serialize(Archive& ar)
{
	ar.serialize(tipFill, "tipFill", "������� ����");
	ar.serialize(tipBorder, "tipBorder", "���������");
	ar.serialize(tipFont, "tipFont", "�����");
	ar.serialize(tipTextFormat, "tipTextFormat", "������ ��������");
	ar.serialize(tipTextFormatHover, "tipTextFormatHover", "������ �����������");
	ar.serialize(RangedWrapperf(tipTextMaxWidth, 0.1f, 0.7f), "tipTextMaxWidth", "������������ ������������� ������ ������");
}

// --------------------------------------------------------------------------

void SideSprite::merge(Vect2f crd, const Rectf& border)
{
	Vect2f center(border.center());
	Vect2f med(pos.center());
	med -= center;
	med.normalize(1.f);
	crd -= center;
	crd.normalize(1.f);
	med += crd;
	med.normalize(border.width());
	med += center;

	border.clipLine(center, med);

	Vect2f half(msprite ? UI_RenderBase::instance().relativeSize(msprite->size()) : pos.size());
	half /= 2.f;
	med.x = clamp(med.x, border.left() + half.x, border.right() - half.x);
	med.y = clamp(med.y, border.top() + half.y, border.bottom() - half.y);

	pos.size(half *= 2.f);
	pos.center(med);
}