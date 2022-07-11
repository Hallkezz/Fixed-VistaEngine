#ifndef __UI_MINIMAP_H__
#define __UI_MINIMAP_H__

#include <map>
#include "Render\inc\RenderMT.h"
#include "MTSection.h"
#include "UI_Font.h"
#include "UI_MinimapSymbol.h"
#include "Util\Timers.h"

class cTexture;
class MultiRegion;
class Event;
class Anchor;
class UnitObjective;
class UnitSquad;
class UnitBase;
class UnitBuilding;

/// ������������ ���������
enum MinimapAlign
{
	UI_ALIGN_BOTTOM_RIGHT,
	UI_ALIGN_BOTTOM_LEFT,
	UI_ALIGN_TOP_RIGHT,
	UI_ALIGN_TOP_LEFT,
	UI_ALIGN_RIGHT,
	UI_ALIGN_LEFT,
	UI_ALIGN_BOTTOM,
	UI_ALIGN_TOP,
	UI_ALIGN_CENTER,
};

typedef vector<Color4c> Colors;

/// ������������ ������� �� ���������
class UI_MinimapEvent
{
public:
	UI_MinimapEvent(const UI_MinimapEventStatic& symbol, const Vect2f& object_pos, float radius, const Color4f& color, float anglez = 0.f, int id = 0);

	bool operator == (const UI_MinimapEvent& obj) const;

	void update(const UI_MinimapEvent& event);
	bool alive() const { return aliveTime_.busy(); }

	const UI_MinimapSymbol& symbol() const { return symbol_; }
	
	const Vect2f& position() const { return pos_; }
	const Vect2f& size() const { return size_; }
	float rotation() const { return rotation_; }

	Color4c color() const { return symbol_.legionColor ? Color4c(color_) : Color4c(symbol_.color); }

	int time() const { return animationTime_.time(); }
	
	float validTime() const { return symbol_.validTime(); }

private:
	/// ���������� id �������
	int uid_;
	/// ������ �������
	UI_MinimapEventStatic symbol_;
	/// ���������� ���������� � ������� �������
	Vect2f pos_;
	Vect2f size_;
	// ���������� �������
	float rotation_;
	/// ��������� ��������� ������
	Color4f color_;

	/// ����������� ����� �������
	GraphicsTimer animationTime_;
	/// ����� �����
	LogicTimer aliveTime_;
};

/// ���������
class UI_Minimap
{
	friend class EasyMap;
	friend void fClearOldEventsCommand(void *data);
public:
	enum EventType {
		BACKGROUND = 0,
		UNITS,
		EVENTS,
		EVENTS_SIZE
	};

	UI_Minimap();
	~UI_Minimap();

	bool inited() const { return worldSize_.xi() && position_.width() > FLT_EPS && position_.height() > FLT_EPS; }

	/// �������� �������� ����� ��� ��������� � ������ ������ ����
	void init(const Vect2f& world_size, const char* texture_file_name);
	void releaseMapTexture();
	void relaxLoading();

	void logicQuant();
	void logic2GraphQuant();
	void quant(float dt);
	void redraw(float alpha = 1.f);

	const Rectf& position() const { return controlPose_; }
	const Rectf& scaledPosition() const { return position_; }
	const Vect2f& worldSize() const { return worldSize_; }
	float rotationAngle() const { return rotationAngle_; }

	void setPosition(const Rectf& pos);
	void setRotate(float angle);
	void setCenter(const Vect2f& world);
	void zoomIn(const Vect2f& target);
	void zoomOut(const Vect2f& target);
	void drag(const Vect2f& delta);

	/// ���� ����� �� ����, �� ������ �������� �����
	void updateEvent(const UI_MinimapEvent& event, EventType type);
	/// ���������� ����� �� ���������
	void addUnit(const UnitObjective* unit);
	/// ���������� ������ �� ���������
	void addSquad(const UnitSquad* squad);
	/// ���������� ����� �� ���������
	void addAnchor(const Anchor* anchor);
	/// �������� �������, ������������� � ��������
	void checkEvent(const Event& event);
	/// �������� ������� �������
	void clearEvents();

	UI_Minimap& viewZone(bool val)		{ drawViewZone_ = val; return *this; }
	UI_Minimap& drawEvents(bool val)	{ drawEvents_ = val; return *this; }
	UI_Minimap& wind(bool val)			{ showWind_ = val; return *this; }
	UI_Minimap& canFog(bool val)		{ canDrawFogOfWar_ = val; return *this; }
	UI_Minimap& showFog(bool show)		{ showFogOfWar_ = show; return *this; }
	UI_Minimap& showZones(bool show)	{ showPlaceZones_ = show; return *this; }
	UI_Minimap& water(bool val)			{ drawWater_ = val; return *this; }
	UI_Minimap& rotate(bool val)		{ canRotateByCamera_ = val; return *this; }
	UI_Minimap& scale(bool val)			{ useUserScale_ = val; return *this; }
	UI_Minimap& userScale(float val)	{ userScale_ = 1.f / val; return *this; }
	UI_Minimap& drag(bool val)			{ useUserDrag_ = val; return *this; }
	UI_Minimap& toSelect(bool val)		{ centerToSelect_ = val; return *this; }
	UI_Minimap& rotScale(bool val)		{ rotationScale_ = val; return *this; }
	UI_Minimap& align(MinimapAlign val)	{ mapAlign_ = val; return *this; }
	
	UI_Minimap& startLocations(bool val){ viewStartLocations_ = val; return *this; }
	UI_Minimap& font(UI_FontReference val)		{ font_ = val; return *this; }

	UI_Minimap& waterColor(const Color4f& val)	{ waterColor_ = val; return *this; }
	UI_Minimap& zoneColor(const Color4f& val)	{ viewZoneColor_ = val; return *this; }
	UI_Minimap& borderColor(const Color4f& val)	{ borderColor_ = val; return *this; }

	UI_Minimap& mask(cTexture* val)		{ mask_ = val; return *this; }

	UI_Minimap& toggleRotateByCamera(bool rotate)	{ rotateByCamera_ = rotate; return *this; }

	bool isDrawWater() const	{ return drawWater_; }
	bool isFogOfWar() const		{ return canDrawFogOfWar_ && showFogOfWar_; }
	bool isInstallZones() const { return showPlaceZones_; }
	bool isRotateByCamera() const { return canRotateByCamera_ && rotateByCamera_; }

	/// ��������� ���������� ������� � �������
	void cameraToEvent();
	/// ��������� ����� �� �����, mouse_pos - ������������� ���������� �������
	void pressEvent(const Vect2f& mouse_pos);

	/// �������� ������������� ���������� �� ��������� {0..1, 0..1}, ���������� �������
	Vect2f minimap2world(const Vect2f& mouse) const;
	/// �������� �������, ���������� ������������� ���������� �� ��������� {0..1, 0..1}
	Vect2f world2minimap(const Vect2f& world) const;
	/// ��������� ��������� ������ � ���������, mouse - ������������� �������� ����������
	bool hitTest(Vect2f mouse) const;
	
	Vect2f screen2minimap(const Vect2f& scr) const;
	Vect2f screen2minimapTexture(const Vect2f& scr) const;

	void drawZone(const UnitBuilding* building);
	void eraseZone(const UnitBuilding* building);
private:
	/// �� �������� ������������� ��������� � ������������� �� ���������� ��������������� �� ���������
	xm_inline Vect2f xformPoint(Vect2f& pos) const{
		pos -= center_;
		pos *= scale_;
		pos *= rotation_;
		pos /= scale_;
		pos += center_;
		return pos;
	}
	/// �� �������� ������������� ��������� ��������������� �� ��������� � ������� ������������� ���������� ������
	xm_inline Vect2f invXformPoint(Vect2f& pos) const{
		pos -= center_;
		pos *= scale_;
		pos = rotation_.invXform(pos);
		pos /= scale_;
		pos += center_;
		return pos;
	}
	// ����������� ��������� � ���������� ��������� �� ������
	void reposition();
	// ��������� � ������� �������� ���������
	Rectf position_;
	// ����� ��������
	Vect2f center_;
	// ��������������� � ����� ������
	Vect2f scale_;

	// ��������� � ������� �������� �� ����
	Rectf controlPose_;
	// ���������������� ��������������
	float userScale_;
	// ���������������� �����
	Vect2f userShift_;
	// ������� ���������
	float rotationAngle_;
	Mat2f rotation_;
	// ������������ ��������� ������ � �������
	bool canRotateByCamera_;
	bool rotateByCamera_;
	// ��������� ��������������
	bool useUserScale_;
	// ��������� ��������������
	bool useUserDrag_;
	// ������������ �� ����������� �����
	bool centerToSelect_;
	// ��������� � �������
	bool rotationScale_;
	// ����� ������� ����
	bool drawViewZone_;
	Color4f viewZoneColor_;
	// ���� ����� ���������
	Color4f borderColor_;
	// ���� ���� �� ���������
	Color4f waterColor_;
	// ����� ������ �����
	bool canDrawFogOfWar_;
	bool showFogOfWar_;
	// ����������� ���� ���������
	bool showPlaceZones_;
	// ����� ��������� �������
	bool viewStartLocations_;
	UI_FontReference font_;
	// �������� �������� ����
	cTexture* mapTexture_;
	// ������ ���� ��� �������������� ��������
	Vect2f worldSize_;
	// �������� ���� �� ����
	bool drawWater_;
	// ���������� ������ � �������
	bool drawEvents_;
	// �������� ����������� �����
	bool showWind_;
	// ���� ��������� ���������
	MinimapAlign mapAlign_;

	// ��� �������������� ����
	bool dragGuard_;

	void setScale(float newScale, const Vect2f& target);

// --------------------------------------------------------------------------

	cTexture* placeZones_;
	
	struct PlaceZone {
		enum Type {
			DRAW,
			REDRAW,
			ERASE
		};
		PlaceZone(Type t, const Vect2f& p, float r, const Color4c& c) : type(t), pos(p), radius(r), color(c) {}
		Type type;
		Vect2f pos;
		float radius;
		Color4c color;
	};
	typedef vector<PlaceZone> ZoneDrawTasks;
	ZoneDrawTasks zoneDrawTasks_;

	friend void fDrawZoneCommand(void*);
	void addZone(const PlaceZone& zone){
		MTG();
		zoneDrawTasks_.push_back(zone);
	}
	
	void renderPlazeZones();

// --------------------------------------------------------------------------
	MTSection lock_;

	/// ������� �� ����, � ������������ �������� � ����
	struct WorldPoint {
		explicit WorldPoint(const Vect2f& ps) : pos(ps) {}
		/// ���������� �� ����
		Vect2f pos;
		/// ����� �����
		LogicTimer aliveTimer;
		bool alive() const { return aliveTimer.busy(); }
	};
	typedef vector<WorldPoint> WorldPoints;
	WorldPoints worldPoints_;

	typedef vector<UI_MinimapEvent> MinimapEvents;
	MinimapEvents events_[EVENTS_SIZE];
	void addEvent(const UI_MinimapEvent& event, EventType type){
		MTG();
		events_[type].push_back(event);
	}

	typedef vector<const UnitBase*> UnitList;
	UnitList unitList_;
	UnitList unitListLogic_;


// --------------------------------------------------------------------------
	void setMaskUV(const Vect2f& p, float& u, float& v) const {
		u = (p.x - controlPose_.left()) / controlPose_.width();
		v = (p.y - controlPose_.top()) / controlPose_.height();
	}
	cTexture* mask_;

	struct Line2d
	{
		Vect2f p0;
		Vect2f p1;
		Color4c c;
	};
	typedef vector<Line2d> Lines2d;
	Lines2d lines_;
	
	void drawLine(Vect2f p1, Vect2f p2, const Color4c& color, bool noCLip = false);
	void flushLines();

	struct Rectangle
	{
		Rectangle(const Vect2f& _v1, const Vect2f& _v2, const Color4c& clr, float r) 
			: v1(_v1), v2(_v2), color(clr), rot(r) {}
		Vect2f v1;
		Vect2f v2;
		Color4c color;
		float rot;
	};
	typedef vector<Rectangle> Rectangles;
	Rectangles rectangles_;
	
	void flushRectangles();

	struct Sprite
	{
		Sprite(const UI_Sprite& s, const Rectf& p, const Color4c& c, float r)
			: sprite(&s), pos(p), color(c), rot(r) {}
		const UI_Sprite* sprite;
		Rectf pos;
		Color4c color;
		float rot;
	};
	struct AnimatedSprite : public Sprite
	{
		AnimatedSprite(const Sprite& s, float ph) : Sprite(s), phase(ph) {}
		float phase;
	};
	typedef vector<Sprite> Sprites;
	typedef std::map<cTexture*, Sprites> Spritesmap;
	Spritesmap spritesmap_;

	typedef vector<AnimatedSprite> AnimatedSprites;
	AnimatedSprites animatedSprites_;

	void drawSprite(const Sprite& sprite, float time);
	void writeSprite(struct sVertexXYZWDT2* pv, const Sprite& data);
	void flushSprites();

	void drawMiniMap(float alpha);
	void drawEvents(EventType type, float alpha);
	void drawViewZone(float alpha);
	void drawStartLocations(float alpha);
};

extern Singleton<UI_Minimap> minimap;

#endif /* __UI_MINIMAP_H__ */
