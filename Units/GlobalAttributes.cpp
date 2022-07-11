#include "StdAfx.h"
#include "GlobalAttributes.h"
#include "Terra\vmap.h"
#include "CameraManager.h"
#include "Serialization\SerializationFactory.h"
#include "Serialization\EnumDescriptor.h"
#include "Serialization\MillisecondsWrapper.h"
#include "Serialization\RangedWrapper.h"
#include "Serialization\RadianWrapper.h"
#include "GameOptions.h"
#include "UnicodeConverter.h"


WRAP_LIBRARY(GlobalAttributes, "GlobalAttributes", "���������� ���������", "Scripts\\Content\\GlobalAttributes", 0, 0);

BEGIN_ENUM_DESCRIPTOR(ObjectLodPredefinedType, "ObjectLodPredefinedType")
REGISTER_ENUM(OBJECT_LOD_DEFAULT, "LOD: �� ���������")
REGISTER_ENUM(OBJECT_LOD_VERY_SMALL, "LOD: ����� ��������� ������")
REGISTER_ENUM(OBJECT_LOD_SMALL, "LOD: ��������� ������")
REGISTER_ENUM(OBJECT_LOD_NORMAL, "LOD: ������� ������")
REGISTER_ENUM(OBJECT_LOD_BIG, "LOD: ������� ������")
REGISTER_ENUM(OBJECT_LOD_VERY_BIG, "LOD: ����� ������� ������")
END_ENUM_DESCRIPTOR(ObjectLodPredefinedType)	


CameraBorder::CameraBorder()
{
	CAMERA_WORLD_BORDER_TOP = 0.f;
	CAMERA_WORLD_BORDER_LEFT = 0.f;
	CAMERA_WORLD_BORDER_RIGHT = 0.f;
	CAMERA_WORLD_BORDER_BOTTOM = 0.f;
}

void CameraBorder::serialize(Archive& ar)
{
	ar.serialize(CAMERA_WORLD_BORDER_TOP,    "CAMERA_WORLD_BORDER_TOP",    "������");
	ar.serialize(CAMERA_WORLD_BORDER_BOTTOM, "CAMERA_WORLD_BORDER_BOTTOM", "�����");
	ar.serialize(CAMERA_WORLD_BORDER_LEFT,   "CAMERA_WORLD_BORDER_LEFT",   "�����");
	ar.serialize(CAMERA_WORLD_BORDER_RIGHT,  "CAMERA_WORLD_BORDER_RIGHT",  "������");
	clampRect();
}


CameraRestriction::CameraRestriction()
{
	aboveWater = true;

	//�������������� ����������� ������
	CAMERA_SCROLL_SPEED_DELTA = 10.0f;
	CAMERA_BORDER_SCROLL_SPEED_DELTA = 10.0f;
	CAMERA_SCROLL_SPEED_DAMP = 0.7f;

	CAMERA_BORDER_SCROLL_AREA_UP = 0.008f;
	CAMERA_BORDER_SCROLL_AREA_DN = 0.014f;
	CAMERA_BORDER_SCROLL_AREA_HORZ = 0.008f;

	//�������� � ������
	CAMERA_KBD_ANGLE_SPEED_DELTA = M_PI/30.f;
	CAMERA_MOUSE_ANGLE_SPEED_DELTA = M_PI;
	CAMERA_ANGLE_SPEED_DAMP = 0.7f;

	//zoom
	zoomKeyAcceleration = 30.f;
	zoomWheelImpulse = 1.5f;
	zoomDamping = 8.f;

	CAMERA_FOLLOW_AVERAGE_TAU = 0.1f;

	//�����������
	CAMERA_MOVE_ZOOM_SCALE = 500.0f;

	zoomMaxTheta = zoomMax = 1000.0f;
	zoomMin = 300.0f;

	heightMin = 0.0f;
	heightMax = 1000.0f;

	zoomDefault = 300.0f;
	thetaMinLow = 0.f;
	thetaMinHigh = 0.f;
    thetaMaxLow = M_PI/3.f;
	thetaMaxHigh = M_PI/10.f;
	thetaDefault = M_PI/6.f;

	CAMERA_WORLD_SCROLL_BORDER = 100.0f;

	unitFollowDistance = 200.0f;
	unitFollowTheta = 1.3f;
	unitHumanFollowDistance = 140.0f;
	unitHumanFollowTheta = 1.42f;

	directControlThetaFactor = 0.5f;
	directControlPsiMax = M_PI / 4.0f;
	directControlRelaxation = 0.4f;
}

void CameraBorder::clampRect()
{
	CAMERA_WORLD_BORDER_TOP = clamp(CAMERA_WORLD_BORDER_TOP, -1000.f, 8192.f);
	CAMERA_WORLD_BORDER_BOTTOM = clamp(CAMERA_WORLD_BORDER_BOTTOM, -1000.f, 8192.f);
	CAMERA_WORLD_BORDER_LEFT = clamp(CAMERA_WORLD_BORDER_LEFT, -1000.f, 8192.f);
	CAMERA_WORLD_BORDER_RIGHT = clamp(CAMERA_WORLD_BORDER_RIGHT, -1000.f, 8192.f);
}

void CameraRestriction::serialize(Archive &ar)
{
	ar.serialize(CAMERA_SCROLL_SPEED_DELTA, "CAMERA_SCROLL_SPEED_DELTA", "�������� ������ ������");
	CAMERA_SCROLL_SPEED_DELTA = clamp(CAMERA_SCROLL_SPEED_DELTA, 0.f, 100.f);
	
	ar.serialize(CAMERA_BORDER_SCROLL_SPEED_DELTA, "CAMERA_BORDER_SCROLL_SPEED_DELTA", "�������� ������ ������ ��� �������� ��������� �� ����");
	CAMERA_BORDER_SCROLL_SPEED_DELTA = clamp(CAMERA_BORDER_SCROLL_SPEED_DELTA, 0.f, 100.f);
	
	float tmp = 1 / CAMERA_SCROLL_SPEED_DAMP;
	ar.serialize(tmp, "CAMERA_SCROLL_SPEED_DAMP", "������� ������ ������");
	if(tmp >= 1.0f) CAMERA_SCROLL_SPEED_DAMP = 1 / tmp;
	CAMERA_SCROLL_SPEED_DAMP = clamp(CAMERA_SCROLL_SPEED_DAMP, 0.1f, 10.f);

	tmp = CAMERA_BORDER_SCROLL_AREA_UP * 100.f;
	ar.serialize(tmp, "CAMERA_BORDER_SCROLL_AREA_UP", "������ ������ ��� ������ ����� (%)");
	CAMERA_BORDER_SCROLL_AREA_UP = clamp(tmp, 0.f, 20.f) / 100.f;

	tmp = CAMERA_BORDER_SCROLL_AREA_DN * 100.f;
	ar.serialize(tmp, "CAMERA_BORDER_SCROLL_AREA_DN", "������ ����� ��� ������ ����� (%)");
	CAMERA_BORDER_SCROLL_AREA_DN = clamp(tmp, 0.f, 20.f) / 100.f;

	tmp = CAMERA_BORDER_SCROLL_AREA_HORZ * 100.f;
	ar.serialize(tmp, "CAMERA_BORDER_SCROLL_AREA_HORZ", "������ �� ����� ��� ������ ����� (%)");
	CAMERA_BORDER_SCROLL_AREA_HORZ = clamp(tmp, 0.f, 20.f) / 100.f;

	tmp = CAMERA_KBD_ANGLE_SPEED_DELTA / M_PI * 180.f;
	ar.serialize(tmp, "CAMERA_KBD_ANGLE_SPEED_DELTA", "�������� �������� ������ �����������");
	CAMERA_KBD_ANGLE_SPEED_DELTA = clamp(tmp, 0.f, 280.f) / 180.f * M_PI;

	tmp = CAMERA_MOUSE_ANGLE_SPEED_DELTA / M_PI * 180.f;
	ar.serialize(tmp, "CAMERA_MOUSE_ANGLE_SPEED_DELTA", "�������� �������� ������ �����");
	CAMERA_MOUSE_ANGLE_SPEED_DELTA = clamp(tmp, 0.f, 280.f) / 180.f * M_PI;

	tmp = 1 / CAMERA_ANGLE_SPEED_DAMP;
	ar.serialize(tmp, "CAMERA_ANGLE_SPEED_DAMP", "������� ������ ��� ��������");
	if(tmp >= 1.0f) CAMERA_ANGLE_SPEED_DAMP = 1 / tmp;
	CAMERA_ANGLE_SPEED_DAMP = clamp(CAMERA_ANGLE_SPEED_DAMP, 0.1f, 10.f);

	ar.serialize(zoomKeyAcceleration, "zoomKeyAcceleration", "�������� ���� ���������");
	ar.serialize(zoomWheelImpulse, "zoomWheelImpulse", "�������� ���� �������");
	ar.serialize(RangedWrapperf(zoomDamping, 0, 10.f) , "zoomDamping", "������� ����");
	
	ar.serialize(RangedWrapperf(CAMERA_FOLLOW_AVERAGE_TAU, 0.0001f, 0.01f), "CAMERA_FOLLOW_AVERAGE_TAU", "��������� �������� � �����");

	ar.serialize(RangedWrapperf(CAMERA_MOVE_ZOOM_SCALE, 0.f, 5000.f), "CAMERA_MOVE_ZOOM_SCALE", "��������������� ��������");

	ar.serialize(RangedWrapperf(heightMax, 0.f, 5000.f), "heightMax", "������������ ������ ��� �����");
	ar.serialize(RangedWrapperf(heightMin, 0.f, 5000.f), "heightMin", "����������� ������ ��� �����");

	ar.serialize(RangedWrapperf(zoomMax, 20.f, 5000.f), "zoomMax", "������������ �������� �� ����� ����������");
	ar.serialize(RangedWrapperf(zoomMin, 20.f, 0.95f*zoomMax), "zoomMin", "����������� �������� �� ����� ����������");
	ar.serialize(RangedWrapperf(zoomDefault, zoomMin, zoomMax), "zoomDefault", "�������� �� ����� ���������� �� ���������");
	ar.serialize(RangedWrapperf(zoomMaxTheta, 0.f, 5000.f), "zoomMaxTheta", "������������ �������� �� ����� ���������� ��� �������");

	float angleMax = G2R(80);
	ar.serialize(RadianWrapper(thetaMinLow, 0, angleMax), "thetaMinLow", "����������� ���� ������� ������ �� ����������� ������");
	ar.serialize(RadianWrapper(thetaMinHigh, 0, angleMax), "thetaMinHigh", "����������� ���� ������� ������ �� ������������ ������");

	ar.serialize(RadianWrapper(thetaMaxLow, 0, angleMax), "thetaMaxLow", "������������ ���� ������� ������ �� ����������� ������");
	ar.serialize(RadianWrapper(thetaMaxHigh, 0, angleMax), "thetaMaxHigh", "������������ ���� ������� ������ �� ������������ ������");

	ar.serialize(RadianWrapper(thetaDefault, 0, angleMax), "thetaDefault", "���� ������� ������ �� ���������");

	ar.serialize(RangedWrapperf(CAMERA_WORLD_SCROLL_BORDER, -50.f, 1000.f), "CAMERA_WORLD_SCROLL_BORDER", "����������� ������ ����� ���������� �� ���� ��� ������������ ��������");
	
	ar.serialize(unitFollowDistance, "unitFollowDistance", 0);
	ar.serialize(unitFollowTheta, "unitFollowTheta", 0);
	ar.serialize(unitHumanFollowDistance, "unitHumanFollowDistance", 0);
	ar.serialize(unitHumanFollowTheta, "unitHumanFollowTheta", 0);

	ar.serialize(directControlThetaFactor, "directControlThetaFactor", "����������� ��� �������� �������� ������ � ������ ����������");
	tmp = directControlPsiMax / M_PI * 180.f;
	ar.serialize(tmp, "directControlPsiMax", "����������� ������ � ������ ����������");
	directControlPsiMax = clamp(tmp, 5.0f, 120.0f) / 180.f * M_PI;
	ar.serialize(directControlRelaxation, "directControlRelaxation", "��������� ������ � ������ ����������");

	ar.serialize(aboveWater, "aboveWater", "��� �����");
}

void CameraBorder::setRect(const Recti& rect)
{
	CAMERA_WORLD_BORDER_LEFT	= rect.left();
	CAMERA_WORLD_BORDER_TOP		= rect.top();
	CAMERA_WORLD_BORDER_RIGHT	= static_cast<int>(vMap.H_SIZE) - rect.right();
	CAMERA_WORLD_BORDER_BOTTOM	= static_cast<int>(vMap.V_SIZE) - rect.bottom();

	clampRect();
}

Recti CameraBorder::rect() const
{
	Recti result(CAMERA_WORLD_BORDER_LEFT, CAMERA_WORLD_BORDER_TOP,
				 vMap.H_SIZE - CAMERA_WORLD_BORDER_RIGHT - CAMERA_WORLD_BORDER_LEFT,
				 vMap.V_SIZE - CAMERA_WORLD_BORDER_BOTTOM - CAMERA_WORLD_BORDER_TOP);
	return result;
}

//---------------------------------------------------------------------------------------

bool ShowHeadName::serialize(Archive& ar, const char* name, const char* nameAlt) 
{
	static ModelSelector::Options headOptions("*.3dx", "Resource\\UI\\Models", "������");
	return ar.serialize(ModelSelector(fileName_, headOptions), name, nameAlt);
}

//---------------------------------------------------------------------------------------

void MapSizeName::serialize(Archive& ar)
{
	ar.serialize(size, "size", "������������ ������� ����� � (���.��)^2");
	ar.serialize(name, "name", "name");
}

//---------------------------------------------------------------------------------------

GlobalAttributes::GlobalAttributes() 
{
	windowTitle = "Perimeter 2";
	savePath = "Perimeter2";
	enableSilhouettes = true;
	terrainSelfShadowing = true;
	enableFieldDispatcher = true;
	enablePathTrackingRadiusCheck = false;
	moveToImpassabilities = false;
	enableFogOfWar = true;
	checkImpassabilityInDC = false;
	pathTrackingStopRadius = 100.0f;
	minRadiusCheck = 10.0f;
	enemyRadiusCheck = false;
	enableEnemyMakeWay = false;
	enableMakeWay = true;
	enableAutoImpassability = true;
	debrisLyingTime = 10;
	debrisProjectileLyingTime = 10;
	treeLyingTime = 10000;
	for(int i = 0; i < 5; i++)
		playerColors.push_back(Color4f(1,1,1,1));
	for(int i = 0; i < 5; i++)
		underwaterColors.push_back(UnitColor(Color4c(0, 0, 255, 255), false));
	for(int i = 0; i < 5; i++)
		silhouetteColors.push_back(Color4f(1,1,1,1));
	analyzeAreaRadius = 15.f;
	uniformCursor = true;
	useStackSelect = false;
	drawUnitSideSpritesAfterInterface = false;
	serverCanChangeClientOptions = false;
	version = 0;
	enableAnimationInterpolation = true;
	animationInterpolationTime = 300;
	directControlMode = DIRECT_CONTROL_DISABLED;
	circleManagerDrawOrder=CIRCLE_MANAGER_DRAW_BEFORE_GRASS_NOZ;
	float radius = 12.5f;
	for(int i=0;i<OBJECT_LOD_SIZE;i++)
	{
		lod_border[i].radius = radius;
		radius*=2;
		lod_border[i].lod12=200;
		lod_border[i].lod23=600;
		lod_border[i].hideDistance = 500;
	}

	cameraDefaultDistance_ = 175.0f;
	cameraDefaultTheta_ = 0.8f;

	cheatTypeSpeed = 60;

	opacityMin = 0.3f;
	opacitySpeed = 0.2f;
	opacityRestoreSpeed = 0.1f;
	opacityBoundFactorXY = 0.7f;
	opacityBoundFactorZ = 0.7f;

	enableUnitVisibilityTrace = false;
	applyArithmeticsOnCreation = false;

	frameTimeAvrTau = 0.025f;
	
	unitSpriteMaxDistance = 1000.f;

	assemblyCommandShowTime = 5.f;

	installerOffset = 30;
}

void GlobalAttributes::serializeHeadLibrary (Archive& ar) 
{
	ar.serialize(showHeadNames, "showHeadNames", "������"); 
}

void GlobalAttributes::Sign::serialize(Archive& ar)
{
	static ResourceSelector::Options textureOptions("*.tga", "Resource\\Models\\Textures");
	ar.serialize(ResourceSelector(unitTexture, textureOptions), "unitTexture", "�������� �� ������");
	ar.serialize(sprite, "sprite", "������ ��� ����������");
}

void GlobalAttributes::LodBorder::serialize(Archive& ar)
{
	ar.serialize(radius,"radius","������ �������");
	ar.serialize(lod12,"lod12","������� �� lod1 � lod2");
	ar.serialize(lod23,"lod23","������� �� lod2 � lod3");
	ar.serialize(hideDistance,"hideDistance","���������� ������������");
}

///  CONVERSION 2008-1-17
struct LanguageConversion
{
	string language;

	LanguageConversion() { } 
	void serialize(Archive& ar);
};
void LanguageConversion::serialize(Archive& ar)
{
	ar.serialize(language, "language", "����");
}
typedef vector<LanguageConversion> LanguageConversionList;
/// ^^^

LanguageCombo::LanguageCombo(const char* lang)
: ComboListString(GameOptions::instance().getLaguagesList())
{
	string current(lang);
	static_cast<ComboListString&>(*this) = _strlwr(const_cast<char*>(current.c_str()));
}

bool LanguageCombo::serialize(Archive& ar, const char* name, const char* nameAlt)
{
	return ar.serialize((ComboListString&)(*this), name, nameAlt);
}

bool LanguageCombo::valid() const
{
	if(value().empty() || !comboList() || !*comboList())
		return false;
	return indexInComboListString(comboList(), *this) >= 0;
}

void GlobalAttributes::serializeGameScenario(Archive& ar) 
{
	ar.serialize(windowTitle, "windowTitle", "�������� ���� ����");
	ar.serialize(savePath, "savePath", "����� ��� ����������");
	static ResourceSelector::Options iconOptions("*.ico", "Resource\\Cursors", "������");
	ar.serialize(ResourceSelector(icon, iconOptions), "icon", "������ ����");

	ar.serialize(directControlMode, "directControlMode", "����� ������� ���������� �� ���������");

	static ResourceSelector::Options uiTextureOptions("*.tga; *.dds; *.avi", "Resource\\UI\\Textures");
	ar.serialize(ResourceSelector(startScreenPicture_, uiTextureOptions), "startScreenPicture", "��������� �����");
	ar.serialize(enableSilhouettes, "enableSilhouettes", "�������� ��������� ��������");
	ar.serialize(terrainSelfShadowing, "terrainSelfShadowing", "�������� ���� �� �����������");
	ar.serialize(enableFogOfWar, "enableFogOfWar", "�������� ����� �����");
	ar.serialize(enableFieldDispatcher, "enableFieldDispatcher", "�������� ������� ����");

	if(ar.isInput())
	{//���������
		float lod12=200; /// ������� �� lod1 � lod2
		float lod23=600; /// ������� �� lod2 � lod3
		if(ar.serialize(lod12, "lod12", "������� �� lod1 � lod2") &&
		 ar.serialize(lod23, "lod23", "������� �� lod2 � lod3"))
		{
			for(int i=0;i<OBJECT_LOD_SIZE;i++)
			{
				lod_border[i].lod12=lod12;
				lod_border[i].lod23=lod23;
			}
		}
	}

	for(int i = 0; i < OBJECT_LOD_SIZE; ++i)
	{
		ar.serialize(lod_border[i], getEnumName(ObjectLodPredefinedType(i)), getEnumNameAlt(ObjectLodPredefinedType(i)));
		if(i>0&&lod_border[i].radius < lod_border[i-1].radius)
		{
			lod_border[i].radius = lod_border[i-1].radius;
			XBuffer buf;
			buf < "������ \"" <  getEnumNameAlt(ObjectLodPredefinedType(i)) < "\" ������ ���� ������ ������� ����������� ���� ";
			xxassert(false,buf);
		}
	}

	if(ar.openBlock("PathTracking", "PathTracking")){
		ar.serialize(moveToImpassabilities, "moveToImpassabilities", "��������� ���� ����� �� ��������������");
		ar.serialize(enablePathTrackingRadiusCheck, "enablePathTrackingRadiusCheck", "PathTracking:������� ����� ���������� �������");
		ar.serialize(enemyRadiusCheck, "enemyRadiusCheck", "PathTracking:����� ���������� �������");
		ar.serialize(minRadiusCheck, "minRadiusCheck", "PathTracking:����������� ������������ ������");
		ar.serialize(enableMakeWay, "enableMakeWay", "PathTracking:�������� ������");
		ar.serialize(enableEnemyMakeWay, "enableEnemyMakeWay", "PathTracking:�������� ������ �����");
		ar.serialize(pathTrackingStopRadius, "pathTrackingStopRadius", "PathTracking:������ ���������");
		ar.serialize(analyzeAreaRadius, "analyzeAreaRadius", "������ ��� ������� ������� �����������");
		ar.serialize(checkImpassabilityInDC, "checkImpassabilityInDC", "��������� �������������� � ������ ����������");
		ar.serialize(enableAutoImpassability, "enableAutoImpassability", "�������� �������������� ��������� ��� ��������������");
		ar.closeBlock();
	}

	ar.serialize(cameraRestriction, "cameraRestriction", "����������� ������");
	ar.serialize(mapSizeNames, "mapSizeNames", "�������� �������� ����");

	ar.serialize(cameraDefaultTheta_, "cameraDefaultTheta", 0);
	ar.serialize(cameraDefaultDistance_, "cameraDefaultDistance", 0);
	ar.serialize(unitSpriteMaxDistance, "unitSpriteMaxDistance", "��������� ���������� �������� ������ ��� �������������� ������");

	ar.serialize(playerColors, "playerColors", "����� �������"); 
	ar.serialize(playerSigns, "playerSignes", "������� �������");
	ar.serialize(silhouetteColors, "silhouetteColors", "����� ��������"); 
	ar.serialize(serverCanChangeClientOptions, "serverCanChangeClientOptions", "������ ������� ���� ����� ������ ��������� ��������");

	ar.serialize(underwaterColors, "underwaterUnitColors", "����� ������� ��� �����");

	ar.serialize(uniformCursor,	"uniformCursor",	"������� �������� ���������� ���� ��� � ������� �����");
	ar.serialize(useStackSelect, "useStackSelect", "������������ �������� ������");

	ar.serialize(drawUnitSideSpritesAfterInterface, "drawUnitSideSpritesAfterInterface", "�������� ���� ����� �� ������� ������ ����������");
	
	ar.serialize(languagesList, "avaiableLanguages", "��������� �����");

	ar.serialize(resourseStatisticsFactors, "resourseStatisticsFactors", "������������ ��� ���������� �������� (���������� �� ��������������� ��������� � ������������)");
	ar.serialize(worldTriggers, "worldTriggers", "�������� ��� ����");
	ar.serialize(assistantDifficulty, "assistantDifficulty", "��������� �� ����������");
	ar.serialize(applyArithmeticsOnCreation, "applyArithmeticsOnCreation", "��������� ���������� ������ ��� �������� �����");

	ar.serialize(debrisLyingTime, "debrisLyingTime", "�����, ������� ������� ����� ����� ������");
	ar.serialize(debrisProjectileLyingTime, "debrisProjectileLyingTime", "�����, ������� ������� �������� ����� ����� ������");
	ar.serialize(MillisecondsWrapper(treeLyingTime), "treeLyingTimeMiliseconds", "�����, ������� ������� ����� ����� �������");
	
	ar.serialize(enableAnimationInterpolation, "enableAnimationInterpolation", "��������� ������������ ��������");
	if(enableAnimationInterpolation)
		ar.serialize(animationInterpolationTime, "animationInterpolationTime", "����� ������������ ��������");

	ar.serialize(enableUnitVisibilityTrace, "enableUnitVisibilityTrace", "��������� ����������� ��������� ������");

	ar.serialize(circleManagerDrawOrder, "circleManagerDrawOrder", "������ ��������� �������� ������");
	ar.serialize(cheatTypeSpeed, "cheatTypeSpeed", "����������� �������� ����� �����, ������ � ������");
	ar.serialize(profileParameters, "profileParameters", "���� ����������, ������� ����� ����������� � ������� (������������ ������ ���)");

	if(ar.openBlock("Opacity", "��������� ������������ ��������")){
		ar.serialize(opacityMin, "opacityMin", "opacityMin");
		ar.serialize(opacitySpeed, "opacitySpeed", "opacitySpeed");
		ar.serialize(opacityRestoreSpeed, "opacityRestoreSpeed", "opacityRestoreSpeed");
		ar.serialize(opacityBoundFactorXY, "opacityBoundFactorXY", "opacityBoundFactorXY");
		ar.serialize(opacityBoundFactorZ, "opacityBoundFactorZ", "opacityBoundFactorZ");
		ar.closeBlock();
	}
	ar.serialize(frameTimeAvrTau, "frameTimeAvrTau", "frameTimeAvrTau");
	ar.serialize(RangedWrapperi(installerOffset, 0, 50), "installerOffset", "������ ����������");

	ar.serialize(RangedWrapperf(assemblyCommandShowTime, 1.f, 180.f), "assemblyCommandShowTime", "����� ������ ������� �� ����� ���������");
    
	if(ar.openBlock("Show up attributes", "��������� ����������� ������")){
		ShowUpAttribute::serializeStatic(ar);
		ar.closeBlock();	
	}

	if(ar.isInput()){
		vector<Color4f>::iterator i;
		FOR_EACH(playerColors, i)
			i->a = 1;

		if(playerColors.empty())
			playerColors.push_back(Color4f(1,1,1,1));
		if(underwaterColors.empty())
			underwaterColors.push_back(UnitColor(Color4c(0, 0, 255, 255), false));
		if(silhouetteColors.empty())
			silhouetteColors.push_back(Color4f(1,1,1,1));
	}
}

void GlobalAttributes::serialize(Archive& ar) 
{
	serializeHeadLibrary (ar);
	serializeGameScenario(ar);

	ar.serialize(version, "version", 0);
}

void GlobalAttributes::setCameraCoordinate()
{
	if(cameraManager) {
		cameraDefaultDistance_ = cameraManager->coordinate().distance();
		cameraDefaultTheta_ = cameraManager->coordinate().theta();
	}
}


