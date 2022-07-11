#ifndef __UI_MINIMAP_H__
#define __UI_MINIMAP_H__

#include "Handle.h"

#include "..\render\inc\Umath.h"
#include "DebugUtil.h"
#include "Rect.h"
#include "UI_Types.h"

#include "..\Util\Timers.h"

class Archive;
class cTexture;
class Event;
class Anchor;
class UnitObjective;
class UI_Minimap;

class UI_MinimapEvent
{
public:
	UI_MinimapEvent(UI_MinimapSymbol symbol, const Vect2f& object_pos, const Vect2f& object_size, const sColor4f& color, int id = 0)
	: uid_(id), symbol_(symbol)
	, pos_(object_pos)
	{
		size_ = symbol.scaleByEvent() ? object_size : eventDefaultSize;
		color_ = color;
		aliveTime_.start(symbol_.lifeTime() + 1);
		animationTime_.start();
	}

	bool operator == (const UI_MinimapEvent& obj) const;

	void update(const UI_MinimapEvent& event);
	bool alive() const { return aliveTime_(); }

	void redraw() const;

private:
	friend class UI_Minimap;
	static Vect2f eventDefaultSize;
	static float currentFrameAlpha;

	/// ���������� id �������
	int uid_;
	/// ������������� �������
	UI_MinimapSymbol symbol_;
	/// ���������� ���������� � ������� �������
	Vect2f pos_;
	Vect2f size_;
	/// ��������� ��������� ������
	sColor4f color_;

	/// ����������� ����� �������
	MeasurementNonStopTimer animationTime_;
	/// ����� �����
	DurationTimer aliveTime_;
};

typedef list<UI_MinimapEvent> MinimapEvents;

/// ���������
class UI_Minimap
{
	friend void fClearOldEventsCommand(void *data);
public:
	UI_Minimap();
	~UI_Minimap();

	bool inited() const { return worldSize_.xi() && position_.width() > FLT_EPS && position_.height() > FLT_EPS; }

	void relaxLoading();

	void logicQuant();
	void quant(float dt);
	void redraw(float alpha = 1.f);

	void startDelay(){ delayTimer_.start(300); }
	bool delay() const { return delayTimer_(); }

	const Rectf& position() const { return controlPose_; }
	const Rectf& scaledPosition() const { return position_; }
	const Vect2f& worldSize() const { return worldSize_; }

	void setPosition(const Rectf& pos);
	void setRotate(float angle);

	void addEvent(const UI_MinimapEvent& event, bool backGround){
		MTG();
		if(backGround)
			events_.push_front(event);
		else
			events_.push_back(event);
	}
	/// ���� ����� �� ����, �� ������ �������� �����
	void updateEvent(const UI_MinimapEvent& event, bool bg = false);
	/// ���������� ����� �� ���������
	void addUnit(const UnitObjective* unit);
	/// ���������� ����� �� ���������
	void addAnchor(const Anchor* anchor);
	/// �������� �������, ������������� � ��������
	void checkEvent(const Event& event);
	/// �������� ������� �������
	void clearEvents();
	/// �������� ���������� �������
	void clearOldEvents();

	/// �������� �������� ����� ��� ��������� � ������ ������ ����
	void init(const Vect2f& world_size, const char* texture_file_name);
	void setViewParameters(float mapAlpha, bool drawViewZone, bool drawFogOfWar, bool drawWater, bool drawEvents, sColor4f viewZoneColor = sColor4f(WHITE), bool rotateByCamera = false, UI_Align mapAlign = UI_ALIGN_CENTER, sColor4f borderColor = sColor4f(1,1,1,0));
	void setViewStartLocationsParameters(bool viewStartLocations, UI_FontReference font);
	void toggleShowFogOfWar(bool show) { showFogOfWar_ = show; }
	bool isFogOfWar() const { return canDrawFogOfWar_ && showFogOfWar_; }
	void toggleRotateByCamera(bool rotate) { rotateByCamera_ = rotate; }
	bool isRotateByCamera() const { return canRotateByCamera_ && rotateByCamera_; }
	void releaseMapTexture();

	/// ��������� ����� �� �����, \a mouse_pos - ������������� ���������� �������
	void pressEvent(const Vect2f& mouse_pos);

	/// �������� ������������� ���������� �� ��������� {0..1, 0..1}, ���������� �������
	Vect2f minimap2world(const Vect2f& mouse) const;
	/// �������� �������, ���������� ������������� ���������� �� ��������� {0..1, 0..1}
	Vect2f world2minimap(const Vect2f& world) const;
	
	Vect2f screan2minimap(const Vect2f& scr) const;

private:
	xm_inline Vect2f xformPoint(Vect2f& pos) const{
		pos -= center_;
		pos *= scale_;
		pos *= rotation_;
		pos /= scale_;
		pos += center_;

		return pos;
	}
	
	xm_inline Vect2f invXformPoint(Vect2f& pos) const{
		pos -= center_;
		pos *= scale_;
		pos = rotation_.invXform(pos);
		pos /= scale_;
		pos += center_;

		return pos;
	}


	void calcScaled();
	void drawClippedLine(const Vect2f& p0, const Vect2f& p1, const sColor4f& color) const;

	// ��������� � ������� �������� �� ����
	Rectf controlPose_;
	// ��������������� �  ������
	Vect2f scale_;
	// ��������� � ������� �������� ���������
	Rectf position_;
	// ����� ��������
	Vect2f center_;
	// ������� ���������
	Mat2f rotation_;
	// ������������ ��������� ������ � �������
	bool canRotateByCamera_;
	bool rotateByCamera_;
	cTexture* mapTexture_;

	sColor4f viewZoneColor_;

	Vect2f worldSize_;
	
	MinimapEvents events_;

	float mapAlpha_;
	bool drawViewZone_;
	bool canDrawFogOfWar_;
	bool showFogOfWar_;
	bool drawWater_;
	bool drawEvents_;

	bool viewStartLocations_;
	UI_FontReference font_;
	sColor4f borderColor_;

	DurationTimer delayTimer_;

	UI_Align mapAlign_;
};

extern Singleton<UI_Minimap> minimap;

#endif /* __UI_MINIMAP_H__ */
