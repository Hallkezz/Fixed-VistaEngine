#include "stdafx.h"

#include "Serialization\Serialization.h"
#include "Serialization\RangedWrapper.h"
#include "SourceBase.h"
#include "RenderObjects.h"
#include "Game\Universe.h"
#include "Water\Water.h"
#include "Water\CircleManager.h"
#include "Serialization\Dictionary.h"
#include "Units\BaseUnit.h"
#include "Environment\SourceManager.h"
#include "Environment\Environment.h"
#include "Physics\WindMap.h"
#include "UserInterface\UserInterface.h"
#include "UserInterface\UI_Logic.h"
#include "EditorVisual.h"
#include "Terra\vMap.h"
#include "Terra\TerrainType.h"

#pragma warning(disable: 4355)

UNIT_LINK_GET(SourceBase)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(SourceBase, TargetClass, "ZoneEffectType");
REGISTER_ENUM_ENCLOSED(SourceBase, SOURCE_ATTACK_ALL, "����������� �� ��� ���� ������");
REGISTER_ENUM_ENCLOSED(SourceBase, SOURCE_ATTACK_GROUND, "����������� ������ �� ��������");
REGISTER_ENUM_ENCLOSED(SourceBase, SOURCE_ATTACK_AIR, "����������� ������ �� ��������");
END_ENUM_DESCRIPTOR_ENCLOSED(SourceBase, TargetClass);

BEGIN_ENUM_DESCRIPTOR(SourceType, "SourceType");
REGISTER_ENUM(SOURCE_WATER,     "�������� ����");
REGISTER_ENUM(SOURCE_BUBBLE,    "�������� ���������");
REGISTER_ENUM(SOURCE_LIGHT,     "�������� �����");
REGISTER_ENUM(SOURCE_BREAK,     "�������� ������");
REGISTER_ENUM(SOURCE_GEOWAVE,   "�������� ��� ����");
REGISTER_ENUM(SOURCE_TERROOL,   "�������� ��������");
REGISTER_ENUM(SOURCE_ICE,       "�������� ����");
REGISTER_ENUM(SOURCE_FREEZE,    "�������� ���������");
REGISTER_ENUM(SOURCE_ZONE,      "���� �� ����");
REGISTER_ENUM(SOURCE_IMPULSE,   "�������� ���������");
REGISTER_ENUM(SOURCE_TORNADO,   "�������� �������");
REGISTER_ENUM(SOURCE_BLAST,     "�������� �������� �����");
REGISTER_ENUM(SOURCE_LIGHTING,  "�������� ������");
REGISTER_ENUM(SOURCE_CAMERA_SHAKING, "�������� ������ ������");
REGISTER_ENUM(SOURCE_FLASH,     "�������� �������");
REGISTER_ENUM(SOURCE_DETECTOR,  "�������� �����������");
REGISTER_ENUM(SOURCE_WATER_WAVE,"�������� ����� �� ����");
REGISTER_ENUM(SOURCE_SHIELD,	"�������� ��������� ����");
REGISTER_ENUM(SOURCE_TELEPORT,	"�������� ������������");
REGISTER_ENUM(SOURCE_DELETE_GRASS, "�������� �������� �����");
REGISTER_ENUM(SOURCE_FLOCK,     "�������� - ���� �� ����");
END_ENUM_DESCRIPTOR(SourceType);

BEGIN_ENUM_DESCRIPTOR(SurfaceClass, "SurfaceClass")
REGISTER_ENUM(SOURCE_SURFACE_GROUND, "�� �����")
REGISTER_ENUM(SOURCE_SURFACE_WATER, "�� ����")
REGISTER_ENUM(SOURCE_SURFACE_ICE, "�� ����")
REGISTER_ENUM(SOURCE_SURFACE_ANY, "�� ����� �����������")
END_ENUM_DESCRIPTOR(SurfaceClass)

BEGIN_ENUM_DESCRIPTOR_ENCLOSED(SourceBase, PlacementMode, "SourceBase::PlacementMode")
REGISTER_ENUM_ENCLOSED(SourceBase, PLACE_NONE, "�������� ��� ��������")
REGISTER_ENUM_ENCLOSED(SourceBase, PLACE_TO_GROUND, "������� �� �����")
REGISTER_ENUM_ENCLOSED(SourceBase, PLACE_TO_WATER,  "������� �� ����")
END_ENUM_DESCRIPTOR_ENCLOSED(SourceBase, PlacementMode)

BEGIN_ENUM_DESCRIPTOR(SurfaceKind, "SurfaceKind")
REGISTER_ENUM(SURFACE_KIND_1, "����������� 1 ����")
REGISTER_ENUM(SURFACE_KIND_2, "����������� 2 ����")
REGISTER_ENUM(SURFACE_KIND_3, "����������� 3 ����")
REGISTER_ENUM(SURFACE_KIND_4, "����������� 4 ����")
END_ENUM_DESCRIPTOR(SurfaceKind)

void fCircleSourceAdd(XBuffer& stream)
{
	SourceBase* source;
	stream.read(source);
	Vect2f pos2d;
	stream.read(pos2d);
	universe()->circleManager()->addCircle(pos2d, source->radius(), source->radiusCircle());
}

void SourceBase::serialize(Archive& ar) 
{
	if(!ar.isEdit()){
		if(ar.isInput()){
			unitID_.unregisterUnit();
			xassert(!unitID_.numRefs());
		}
		unitID_.serialize(ar);
	}
	
	if(enabled() || !ar.isEdit())
		ar.serialize(active_, "active", "��������");
	
	if(ar.isOutput())
		libraryKey_ = SourceReference(this).c_str();
	ar.serialize(libraryKey_, "libraryKey", 0);

	ar.serialize(label_, "label", "��� �����");
	if(!ar.isEdit())
		ar.serialize(pose_, "pose", "�������");
	ar.serialize(RangedWrapperf (radius_, 1.0f, 2000.0f, 1.0f), "radius", "������");

	if(ar.openBlock("", "�������������� �� ������")){
		ar.serialize(surfaceInstallClass_, "surfaceClass", "����� ���������");
		ar.serialize(surfaceClass_, "workSurfaceClasses", "�������� ��");
		if(!ar.serialize(surfaceKind_, "surfaceKind16", "����� ��������� (��� �����������)")){
			BitVector<SurfaceKind> tmp;
			ar.serialize(tmp, "surfaceKind", "����� ��������� (��� �����������)");
			surfaceKind_=SurfaceKindBitVector((int)tmp);
		}
		ar.serialize(placementMode_, "placementMode", "��������� �� �����������");
		ar.serialize(move_by_wind_, "move_by_wind", "�������� ������");
		ar.serialize(windSensitivity_, "windSensitivity", "����������� �������� �� �����");
		ar.serialize(move_by_water_, "move_by_water", "�������� �����");
		ar.serialize(mapUpdateDeactivate_, "mapUpdateDeactivate", "��������� ��� ��������� �����������");
		ar.serialize(mapUpdateActivate_, "mapUpdateActivate", "�������� ��� ��������� �����������");
		if(mapUpdateDeactivate_){
			mapUpdateActivate_ = false;
			waiting_height_ = false;
		}
		if(!mapUpdateDeactivate_)
			ar.serialize(waiting_height_, "waiting_height", "�������������� ���� ���� ��� ���� �������� ������");
		else
			waiting_height_ = false;
		if(waiting_height_){
			ar.serialize(activationHeight_, "activationHeight", "������������, ���� ����� ����");
			ar.serialize(activationBottomHeight_, "activationBottomHeight", "������������, ���� ����� ����");
		}
		ar.closeBlock();
	}

	if(ar.openBlock("", "�������������")){
		ar.serialize(waiting_target_, "waiting_target", "�������������� ��� ���� � ����");
		if(waiting_target_){
			ar.serialize(activate_by_detonator_, "activate_by_detonator", "�������������� ������ �����������");
			if(!activate_by_detonator_){
				ar.serialize(activatorMode_, "activatorMode", "��� ���� ��� ���������");
				ar.serialize(activate_by_projectile_, "activate_by_projectile", "�������������� ��� ��������� ������");
			}
			ar.serialize(killOwner_, "killOwner", "������� ������� ��� ���������");
		}
		ar.serialize(waitingEffectAttribute_, "waitingEffectAttribute", "������ � ������ ��������");
		ar.serialize(visibleTime_, "visibleTime", "����� ��������� ����� �����������");
		ar.closeBlock();
	}
	
	ar.serialize(targetClass_, "targetClass_", "����� ��������� ������");

	ar.serialize(radiusCircle_, "radiusCircle", "����������� ������� �� ����");

	ar.serialize(soundReference_, "soundReference", "����");

	ar.serialize(followPath_, "followPath", "��������� �� ����������");
	if(!ar.isEdit()) {
		ar.serialize(path_, "path", "����������");
		ar.serialize(origin_, "origin", "����� ����������");
	} else {
        ar.serialize(path_.cycled_, "cycledPath", "��������� ����������");
        ar.serialize(path_.speed_, "speed", "�������� �� ����������");
	}

	ar.serialize(childSources_, "childSources_", "������������ ���������");

	ar.serialize(interfaceLightIndex_, "interfaceLightIndex", "�������� ����� � ����������");

	if(ar.isOutput() && isUnderEditor())
		activation_started_ = false;

	ar.serialize(activationTimer_, "activationTimer_", 0);
	ar.serialize(activation_started_, "activation_started_", 0);
	ar.serialize(killTimer_, "killTimer_", 0);
	ar.serialize(lifeTime_, "lifeTime_", 0);
	ar.serialize(showTimer_, "showTimer_", 0);
	ar.serialize(deadQuant_, "deadQuant_", 0);
	ar.serialize(affectMode_, "affectMode_", 0);

	int player_id = player_ ? player_->playerID() : -1;
	ar.serialize(player_id, "player_id", "�����");
	if(ar.isInput()){
		if(!universe() || player_id >= universe()->Players.size() || player_id < 0)
			player_id = -1;
		player_ = (player_id != -1 && universe()) ? universe()->findPlayer(player_id) : 0;
	}
	ar.serialize(owner_, "owner", 0);

	if(ar.isInput() && enabled()){
		soundInit();
		if(active())
			waitingEffectStop();
	}

	ar.serialize(velocity_, "velocity_", 0);

	ar.serialize(breakWhenApply_, "breakWhenAffect", "�������� ��������� ��� ������� ��� �����������");
}

void SourceBase::enable()
{ 
	xassert(!enabled_);
	BaseUniverseObject::enable();

	enabled_ = true; 
	soundInit();

	if(!inserted())
		sourceManager->sourceGrid.Insert(*this, position().xi(), position().yi(), round(radius()));

	delete environmentPoints_;
	environmentPoints_ = new Vect3f[ENVIRONMENT_POINTS_COUNT + 1];
	recalcEnvironmentPoints();
}

void SourceBase::showEditor() const
{
	if(editorVisual().isVisible(objectClass())){
		string txtOut = getEnumName(type());
		if(strlen(label())){
			txtOut += " - ";
			txtOut += label();
		}

		editorVisual().drawText(position(), txtOut.c_str(), EditorVisual::TEXT_LABEL);
		editorVisual().drawRadius(position(), radius(), EditorVisual::RADIUS_OBJECT, selected());
		editorVisual().drawOrientationArrow(pose(), selected());
	}
}

void SourceBase::setPose(const Se3f& _pos, bool init)
{
	Se3f pos(_pos);
	if(placementMode_ != PLACE_NONE){
		pos.trans().z = vMap.getApproxAlt(pos.trans().xi(), pos.trans().yi());
		if(placementMode_ == PLACE_TO_WATER)
			pos.trans().z = max(environment->water()->GetZ(pos.trans().xi(), pos.trans().yi()), pos.trans().z);
	}
	if (init) {
		origin_ += pos.trans() - pose_.trans();
	}
	BaseUniverseObject::setPose(pos, init);
	sound_.setPosition(position());
	if(inserted())
		sourceManager->sourceGrid.Move(*this, position().xi(), position().yi(), round(radius()));
}

void SourceBase::setRadius(float _radius)
{
	xassert(enabled_);
	__super::setRadius(_radius);
	xassert(environmentPoints_);
	recalcEnvironmentPoints();
}

class UnitScanOperator
{
public:
	UnitScanOperator(SourceBase* zone = 0){
		if(zone)
			setOwner(zone);
	}

	void setOwner(SourceBase* zone){
		zone_ = zone;
		radius_ = zone_->radius();
		position_ = Vect2f(zone_->position());
	}

	void operator()(UnitBase* p){
		if(zone_->canApply(p) && checkTargetMode(p) && p->position2D().distance2(position_) < sqr(radius_ + p->radius()))
			zone_->apply(p);
	}

private:

	float radius_;
	Vect2f position_;
	SourceBase* zone_;

	bool checkTargetMode(UnitBase* p){
		switch(zone_->targetClass()){
		case SourceBase::SOURCE_ATTACK_AIR:
			return WeaponBase::unitMode(p) == WeaponPrm::ON_AIR;
		case SourceBase::SOURCE_ATTACK_GROUND:
			return WeaponBase::unitMode(p) != WeaponPrm::ON_AIR;
		}
		return true;
	}
};

void SourceBase::quant()
{
	xassert (enabled_);

	if (followPath_ && active_) {
		path_.quant();

        Vect3f pos = path_.currentPosition();
        pose().rot().xform(pos);
		setPose(Se3f(orientation(), origin_ + pos), false);
    }
	
	sound_.quant();

	if(!active())
		killTimer_.pause();

	if(killTimer_.finished() && isUseKillTimer())
		kill();

	if(!isAlive()){
		unitID_.unregisterUnit();
		return;
	}

	if((scanEnvironment_ || (waiting_target_ && !activation_started_))){
		start_timer(unitScan);
		static UnitScanOperator unitScanOperator;
		unitScanOperator.setOwner(this);
		targetInZone = false;
		universe()->unitGrid.Scan(position().xi(), position().yi(), round(radius()), unitScanOperator);
		stop_timer(unitScan);
	}

	if(!isUnderEditor()){
		if(!activation_started_){
			if(!active() && waiting_target_)
				if(targetInZone){
					activation_started_ = true;
					if(!activationTimer_.started())
						setActivity(true);
					if(killOwner_ && owner()){
						owner()->Kill();
						setOwner(0);
					}
				}
				else {
					activationTimer_.pause();

					if(player()->active())
						waitingEffectStart();
					else if(!showTimer_.busy())
						waitingEffectStop();
				}

				if(!active() && waiting_height_){
					int altitude = vMap.getApproxAlt(position().xi(), position().yi());
					if(activationHeight_ > 0 && altitude >= activationHeight_)
						setActivity(true);
					else if(activationBottomHeight_ > 0 && altitude <= activationBottomHeight_)
						setActivity(true);
				}
		}

		if(active() || !waiting_target_ || (activation_started_ && (!activationTimer_.started() || activationTimer_.finished()))){
			if(surfaceClass_ != SOURCE_SURFACE_ANY)
				setActivity(sourceManager->checkEnvironment(position(), surfaceClass_));
		}
	}

	if(active()){
		if(move_by_wind_ || move_by_water_)
			environmentAnalysis();

		if(radiusCircle_.color.a && UI_Dispatcher::instance().isEnabled())
			streamLogicInterpolator.set(fCircleSourceAdd) << this << position2D();

		ChildSources::iterator it;
		FOR_EACH(childSources_, it){
			ChildSource& src = *it;
			if(src.source_.isEmpty()) continue;
			if(!src.activationTimer_.started() || (src.activationTimer_.finished() && src.generationDelay_ > FLT_EPS)){
				src.activationTimer_.start(src.generationDelay_ * 1000);
				if(SourceBase* source = sourceManager->createSource(&src.source_, currentPose(), false)){
					source->setPlayer(player_);
					source->setOwner(owner());
				}
			}
		}
	}
}

void SourceBase::recalcEnvironmentPoints()
{
	xassert(environmentPoints_);
	float R = 0.9f * radius();
	for(int pn = 0; pn < ENVIRONMENT_POINTS_COUNT; pn++){
		float angle = 2.f * M_PI * pn / ENVIRONMENT_POINTS_COUNT;
		environmentPoints_[pn] = Vect3f(R * cosf(angle), R * sinf(angle), 0.f);
	}
	environmentPoints_[ENVIRONMENT_POINTS_COUNT] = Vect3f::ZERO;
}

void SourceBase::environmentAnalysis()
{
	xassert(environmentPoints_);
	xassert(move_by_wind_ || move_by_water_);

	Vect3f V = Vect3f::ZERO; // �������� ��������
	Vect3f Rot = Vect3f::ZERO; //��������

	Se3f currPose = pose();

	if(move_by_water_){
		for(int pn = 0; pn <= ENVIRONMENT_POINTS_COUNT; ++pn){
			Vect3f point = clampWorldPosition(position() + environmentPoints_[pn], 0) - position();
			Vect3f vel(environment->water()->GetVelocity(position() + point));
			V += vel;
			Rot += vel.precross(point);
		}
		if(abs(Rot.z) > FLT_EPS){
			QuatF rotate(0.001 * Rot.z / ENVIRONMENT_POINTS_COUNT * logicPeriodSeconds, Vect3f::K);
			currPose.rot().postmult(rotate);
		}
		V *= (logicPeriodSeconds / ENVIRONMENT_POINTS_COUNT);
	}

	if(move_by_wind_){
		Vect2f wnd = windMap->getBilinear(clampWorldPosition(position2D(), 2));
		wnd *= logicRNDinterval(windSensitivity_.minimum(), windSensitivity_.maximum());
		QuatF rndRotate(logicRNDfabsRndInterval(-M_PI_2, M_PI_2), Vect3f::K, 0);
		Vect3f windVelocity(wnd, 0.f);
		rndRotate.xform(windVelocity);
		V.x += windVelocity.x;
		V.y += windVelocity.y;
	}

	velocity_.interpolate(velocity_, V, 0.7f);
	currPose.trans() += velocity_;
	setPose(currPose, false);
}

void SourceBase::apply(UnitBase* target)
{
	targetInZone = true;
}

bool SourceBase::checkTarget(AffectMode mode, const UnitBase* target) const
{
	switch(mode){
	case AFFECT_NONE_UNITS:
		return false;
	case AFFECT_OWN_UNITS:
		return (player() == target->player());
	case AFFECT_FRIENDLY_UNITS:
		return !player()->isEnemy(target);
	case AFFECT_ALLIED_UNITS:
		return !player()->isEnemy(target) && (player() != target->player());
	case AFFECT_ENEMY_UNITS:
		return player()->isEnemy(target);
	case AFFECT_ALL_UNITS:
		return true;
	}
	return false;
}

bool SourceBase::isDetonator(const UnitBase* target) const
{
	// ���� ��������� ������ �� ����������, �� ��� � ����
	bool thisDetonator = (target->attr().unitClass() == UNIT_CLASS_ZONE && player() == target->player());
	if(activate_by_detonator_ || thisDetonator)
		return thisDetonator;

	if(target->attr().isProjectile()) // �� ��������� ����������� ��������
		if(activate_by_projectile_)
			return !target->alive(); // �������� ������ - ������� ������
		else
			return false; // ������ �� ���� ��� ��� ���������

	if(target->attr().isActing()) // �� ��������� �� ���������
		return checkTarget(activatorMode_, target);
	
	return false;
}

bool SourceBase::canApply(const UnitBase* target) const
{
	if(active_)
		return checkTarget(affectMode_, target);
	else if(waiting_target_)
		return isDetonator(target);
	return false;
}

float SourceBase::Path::length() const
{
    float len = 0.0f;
    for(int i = 0; i < size() - 1; ++i) {
        len += (*this)[i].distance((*this)[i+1]);
    }
	if(cycled_) {
        len += front().distance(back());
	}
    return len;
}

float SourceBase::Path::period() const
{
    float time = 0.0f;
    for(int i = 0; i < size() - 1; ++i) {
		float vel = ((*this)[i].velocity_ + (*this)[i+1].velocity_) * 0.5f;
		time += (*this)[i].distance((*this)[i+1]) / vel;
    }
	if(cycled_) {
		float vel = (back().velocity_ + front().velocity_) * 0.5f;
		time += back().distance(front()) / vel;
	}
	return time;
}

void SourceBase::Path::init()
{
}

Vect3f SourceBase::Path::currentPosition() const
{
	if(empty())
		return Vect3f::ZERO;

	if(size() == 1)	 {
		return front();
	} else {
		const Path& self = *this;
		Vect3f dir = self[(segment_ + 1)%size()] - self[segment_];
		return self[segment_] + dir * position_;
	}
}

void SourceBase::Path::quant()
{
	if(size() <= 1)
		return;

	int segmentsCount = size();

	const float timeElapsed = logicPeriodSeconds;
	const PathPosition& last = (*this)[segment_];
	const PathPosition& next = (*this)[(segment_ + 1)%segmentsCount];

    Vect3f dir = next - last;
    float segmentLength = dir.norm();
    float velocity = speed_ * next.velocity_ * position_ + speed_ * last.velocity_ * (1.0f - position_);

	position_ += logicPeriodSeconds * speed_ * velocity / segmentLength;
	while (position_ > 1.0f) { 
		position_ -= 1.0f;
		if(cycled_) {
			segment_ = (segment_ + 1) % segmentsCount;	
		} else {
			if((++segment_) == segmentsCount - 1)
				segment_ = 0;
		}
	}
}

void SourceBase::PathPosition::serialize(Archive& ar)
{
    Vect3f::serialize(ar);
    ar.serialize(velocity_, "velocity", "��������");
}

void SourceBase::Path::serialize(Archive& ar)
{
	ar.serialize(static_cast< std::vector<SourceBase::PathPosition>& >(*this), "nodes", "����");
    ar.serialize(cycled_, "cycled", "��������");
    ar.serialize(speed_, "speed", "��������");
    ar.serialize(position_, "segmentPosition", "������� �� �������");
    ar.serialize(segment_, "segment", "�������");
}

string SourceBase::getDisplayName(SourceType type)
{
	xassert(type>=0 && type<SOURCE_MAX);
	return TRANSLATE(getEnumNameAlt(type));
}

void SourceBase::soundInit(){
	xassert(enabled_);
	sound_.init(soundReference_, this);
	if(active())
		sound_.start();
}

void SourceBase::start()
{
	xxassert(pose_.trans().xi() || pose_.trans().yi(), "�������� � ����");
	if(!sound_.isInited())
		soundInit();
	else
		sound_.start();
	
	activation_started_ = true;

	waitingEffectStop();
	
	activationTimer_.stop();

	setRadius(radius());

	if(interfaceLightIndex_ != -1)
		UI_LogicDispatcher::instance().addLight(interfaceLightIndex_, pose_.trans());

	if(!label_.empty())
		universe()->checkEvent(EventSourceBase(Event::ACTIVATE_SOURCE, this));
}

void SourceBase::stop()
{
	sound_.stop();
}

void SourceBase::setActivity(bool _active){
	xxassert(enabled_ || !_active, "������ �� ������������ ���������");
	xxassert(isAlive() || !_active, "������ �������� ���������");
	if(active_ != _active){
		active_ = _active;
		if(active_)
			start();
		else
			stop();
	}
}

void SourceBase::serializationApply(const Archive& ar)
{
	if(enabled() && ar.isInput())
		if(active())
			start();
		else
			stop();
}

void SourceBase::setAffectMode(AffectMode mode)
{
	affectMode_ = mode;
	if(activatorMode_ == AFFECT_NONE_UNITS)
		activatorMode_ = mode;
}

void SourceBase::setDetected()
{
	if(active() || player()->active() || visibleTime_ < FLT_EPS)
		return;
	
	if(!showTimer_.started())
		waitingEffectStart();

	showTimer_.start(visibleTime_ * 1000);
}

Player* SourceBase::worldPlayer() const
{
	return universe()->worldPlayer();
}

UnitBase* SourceBase::owner() const 
{
	return owner_;
}

void SourceBase::setOwner(UnitBase* unit)
{
	xassert(!player_ || !unit || unit->player() == player_);
	owner_ = unit;
}

void SourceBase::waitingEffectStart()
{
	if(!waitingEffectController_.isEnabled()){
		start_timer_auto();
		waitingEffectController_.effectStart(&waitingEffectAttribute_);
	}
}

void SourceBase::waitingEffectStop()
{
	waitingEffectController_.release();
	showTimer_.stop();
}

SourceBase::SourceBase(const SourceBase& src) : waitingEffectController_(this)
{
	libraryKey_ = src.libraryKey_;
    label_ = src.label_;
	pose_ = src.pose_;
    radius_ = src.radius_;

	lifeTime_ = src.lifeTime_;

	interfaceLightIndex_ = src.interfaceLightIndex_;

    origin_ = pose_.trans();
    followPath_ = src.followPath_;
    path_ = src.path_;
	path_.segment_ = 0;
	path_.position_ = 0.0f;

	affectMode_ = src.affectMode_;
	activatorMode_ = src.activatorMode_;
    enabled_ = false;
    deadQuant_ = 0;
    active_ = false;
    targetInZone = false;
	activation_started_ = false;
    waiting_target_ = src.waiting_target_;
	waiting_height_ = src.waiting_height_;
	activationHeight_ = src.activationHeight_;
	activationBottomHeight_ = src.activationBottomHeight_;
	killOwner_  = src.killOwner_;
	activate_by_detonator_ = src.activate_by_detonator_;
	activate_by_projectile_ = src.activate_by_projectile_;
    targetClass_ = src.targetClass_;
	surfaceInstallClass_ = src.surfaceInstallClass_;
	surfaceClass_ = src.surfaceClass_;
	surfaceKind_ = src.surfaceKind_;
	placementMode_ = src.placementMode_;

	radiusCircle_ = src.radiusCircle_;
    
	player_ = src.player_;
	visibleTime_ = src.visibleTime_;
	
	childSources_ = src.childSources_;

    soundReference_ = src.soundReference_;

	waitingEffectAttribute_ = src.waitingEffectAttribute_;
	scanEnvironment_ = true;
	environmentPoints_ = 0;
	velocity_ = src.velocity_;
	move_by_wind_ = src.move_by_wind_;
	windSensitivity_ = src.windSensitivity_;
	move_by_water_ = src.move_by_water_;
	mapUpdateDeactivate_ = src.mapUpdateDeactivate_;
	mapUpdateActivate_ = src.mapUpdateActivate_;
	setScanEnvironment(false);

	breakWhenApply_ = src.breakWhenApply_;
}

SourceBase::SourceBase() : waitingEffectController_(this)
{
    pose_ = Se3f::ID;
    radius_ = 100.0f;

    origin_ = Vect3f::ZERO;
    followPath_ = false;

	affectMode_ = AFFECT_ENEMY_UNITS;
	activatorMode_ = AFFECT_NONE_UNITS;

	interfaceLightIndex_ = -1;

	lifeTime_ = 0;

    enabled_ = false;
    active_ = false;
    deadQuant_ = 0;
    targetInZone = false;
	activate_by_detonator_ = false;
    waiting_target_ = false;
	waiting_height_ = false;
	activationHeight_ = 75;
	activationBottomHeight_ = -1;
	killOwner_ = false;
	activation_started_ = false;
	activate_by_projectile_ = false;
    targetClass_ = SOURCE_ATTACK_ALL;
	surfaceInstallClass_ = SOURCE_SURFACE_ANY;
	surfaceClass_ = SOURCE_SURFACE_ANY;
	surfaceKind_ = TERRAIN_TYPEALL;//	//SURFACE_KIND_1 | SURFACE_KIND_2 | SURFACE_KIND_3 | SURFACE_KIND_4;
	placementMode_ = PLACE_NONE;

	radiusCircle_.color.set(1.f, 1.f, 1.f, 0.f);

	player_ = 0;
	visibleTime_ = 0.f;
	scanEnvironment_ = true;
	environmentPoints_ = 0;
	velocity_ = Vect3f::ZERO;
	move_by_wind_ = false;
	windSensitivity_.set(0.9f, 1.1f);
	move_by_water_ = false;
	mapUpdateDeactivate_ = false;
	mapUpdateActivate_ = false;
	setScanEnvironment(false);

	breakWhenApply_ = false;
}

SourceBase::~SourceBase()
{
	dassert(!active() && "�������� �� �������������� ���������");
	sound_.release();
	waitingEffectController_.release();
	if(inserted())
		sourceManager->sourceGrid.Remove(*this);
	delete environmentPoints_;
}

void SourceBase::showDebug() const
{
	XBuffer info;

	if(showDebugSource.type)
		info < getEnumNameAlt(type()) < "\n";

	if(showDebugSource.state)
		info < (active() ? "active" : (activation_started_ ? "activation started" : "disabled")) < ", z = " <= vMap.getApproxAlt(position().xi(), position().yi()) < "\n";

	if(showDebugSource.name)
		info < libraryKey_.c_str() < "\n";

	if(showDebugSource.label)
		info < label() < "\n";

	if(info.tell())
		show_text(position(), info.c_str(), Color4c::MAGENTA);

	if(showDebugSource.radius)
		show_vector(position(), radius(), Color4c::MAGENTA);

	if(showDebugSource.showEnvironmentPoints)
		if(environmentPoints_){
			for(int pn = 0; pn < ENVIRONMENT_POINTS_COUNT; pn++)
				show_vector(position()+environmentPoints_[pn], 2.f, Color4c::GREEN);
		}

	if(showDebugSource.axis){
		MatXf X(pose());
		Vect3f delta = X.rot().xcol();
		delta.normalize(15);
		show_vector(X.trans(), delta, Color4c::RED);

		delta = X.rot().ycol();
		delta.normalize(15);
		show_vector(X.trans(), delta, Color4c::BLUE);

		delta = X.rot().zcol();
		delta.normalize(15);
		show_vector(X.trans(), delta, Color4c::GREEN);
	}
}

void SourceBase::kill() 
{
	if(isAlive() && (killRequest() || isUnderEditor())){
		incDeadCnt();
		setActivity(false);
	}
}

void SourceBase::mapUpdate(float x0,float y0,float x1,float y1) 
{
	if(isAlive() && (mapUpdateDeactivate_ || mapUpdateActivate_) && position2D().distance2(Vect2f(x0 + x1, y0 + y1)/2) < sqr(radius()) + sqr((x1 - x0)/2) + sqr((y1 - y0)/2))
		setActivity(mapUpdateDeactivate_ ? false : true);
}

void SourceBase::ChildSource::serialize(Archive &ar){
	ar.serialize(generationDelay_, "generationDelay", "�������� ���������");
	
	source_.serialize(ar);
	
	ar.serialize(activationTimer_, "activationTimer_", 0);
}

const SourceAttribute* SourceBase::getChildSourceByKey(const char* key) const
{
	ChildSources::const_iterator it;
	FOR_EACH(childSources_, it)
		if(const SourceAttribute* satt = it->source_.getByKey(key))
			return satt;
	return 0;
}

SourceAttribute::SourceAttribute()
{
	activationDelay_ = lifeTime_ = 0;
}

void SourceAttribute::serialize(Archive& ar)
{
	ar.serialize(activationDelay_, "activationDelay", "�������� ������");
	ar.serialize(lifeTime_, "lifeTime", "����� ��������");
	ar.serialize(key_, "key", "���� ��� ������");
	ar.serialize(sourceReference_, "sourceReference", "&��������");
}

SourceWeaponAttribute::SourceWeaponAttribute()
{
	isAutonomous_ = false;
	positionDelta_ = Vect2f(0,0);

	showColor_.color.set(0, 0, 200, 0);
}

const SourceAttribute* SourceAttribute::getByKey(const char* key) const
{
	if(key_ != key)
		return source()->getChildSourceByKey(key);
	return this;
}

void SourceWeaponAttribute::serialize(Archive& ar)
{
	__super::serialize(ar);
	ar.serialize(showColor_, "showColor", "����������� ������� ���������");
	ar.serialize(isAutonomous_, "isAutonomous", "�� ��������� �� ��������� ������� �������� ���������");
	ar.serialize(positionDelta_, "positionDelta", "��������");
}