#include "StdAfx.h"
#include "GlobalAttributes.h"
#include "..\Terra\vmap.h"
#include "CameraManager.h"

WRAP_LIBRARY(GlobalAttributes, "GlobalAttributes", "���������� ���������", "Scripts\\Content\\GlobalAttributes", 0, false);

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


CameraRestriction::CameraRestriction(){
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
	CAMERA_ZOOM_SPEED_DELTA = 20.0f;
	CAMERA_ZOOM_MOUSE_MULT = 600.0f;
	CAMERA_ZOOM_SPEED_DAMP = 0.7f;

	CAMERA_FOLLOW_AVERAGE_TAU = 0.1f;

	//�����������
	CAMERA_MOVE_ZOOM_SCALE = 500.0f;

	CAMERA_ZOOM_MAX = 1000.0f;
	CAMERA_ZOOM_MIN = 300.0f;

	CAMERA_MIN_HEIGHT = 0.0f;
	CAMERA_MAX_HEIGHT = 1000.0f;

	CAMERA_ZOOM_DEFAULT = 300.0f;
    CAMERA_THETA_MAX = M_PI/3.f;
	CAMERA_THETA_MIN = M_PI/10.f;
	CAMERA_THETA_DEFAULT = M_PI/6.f;

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

	ar.serialize(CAMERA_ZOOM_SPEED_DELTA, "CAMERA_ZOOM_SPEED_DELTA", "�������� ����������� ������ (����)");
	CAMERA_ZOOM_SPEED_DELTA = clamp(CAMERA_ZOOM_SPEED_DELTA, 0.f, 100.f);

	ar.serialize(CAMERA_ZOOM_MOUSE_MULT, "CAMERA_ZOOM_MOUSE_MULT", "���������������� ���� �����");
	CAMERA_ZOOM_MOUSE_MULT = clamp(CAMERA_ZOOM_MOUSE_MULT, 0.f, 5000.f);

	tmp = 1 / CAMERA_ZOOM_SPEED_DAMP;
	ar.serialize(tmp, "CAMERA_ZOOM_SPEED_DAMP", "������� ����");
	if(tmp >= 1.0f) CAMERA_ZOOM_SPEED_DAMP = 1 / tmp;
	CAMERA_ZOOM_SPEED_DAMP = clamp(CAMERA_ZOOM_SPEED_DAMP, 0.1f, 10.f);

	ar.serialize(CAMERA_FOLLOW_AVERAGE_TAU, "CAMERA_FOLLOW_AVERAGE_TAU", "��������� �������� � �����");
	CAMERA_FOLLOW_AVERAGE_TAU = clamp(CAMERA_FOLLOW_AVERAGE_TAU, 0.0001f, 0.01f);

	ar.serialize(CAMERA_MOVE_ZOOM_SCALE, "CAMERA_MOVE_ZOOM_SCALE", "��������������� ��������");
	CAMERA_MOVE_ZOOM_SCALE = clamp(CAMERA_MOVE_ZOOM_SCALE, 0.f, 5000.f);

	ar.serialize(CAMERA_ZOOM_MAX, "CAMERA_ZOOM_MAX", "������������ �������� �� ����� ����������");
	CAMERA_ZOOM_MAX = clamp(CAMERA_ZOOM_MAX, 0.f, 5000.f);

	ar.serialize(CAMERA_ZOOM_MIN, "CAMERA_ZOOM_MIN", "����������� �������� �� ����� ����������");
	CAMERA_ZOOM_MIN = clamp(CAMERA_ZOOM_MIN, 0.f, 0.95f * CAMERA_ZOOM_MAX);

	ar.serialize(CAMERA_MAX_HEIGHT, "CAMERA_MAX_HEIGHT", "������������ ������ ��� �����");
	CAMERA_MAX_HEIGHT = clamp(CAMERA_MAX_HEIGHT, 0.f, 5000.f);

	ar.serialize(CAMERA_MIN_HEIGHT, "CAMERA_MIN_HEIGHT", "����������� ������ ��� �����");
	CAMERA_MIN_HEIGHT = clamp(CAMERA_MIN_HEIGHT, 0.f, 5000.f);

	ar.serialize(CAMERA_ZOOM_DEFAULT, "CAMERA_ZOOM_DEFAULT", "�������� �� ����� ���������� �� ���������");
	CAMERA_ZOOM_DEFAULT = clamp(CAMERA_ZOOM_DEFAULT, CAMERA_ZOOM_MIN, CAMERA_ZOOM_MAX);

	tmp = CAMERA_THETA_MAX / M_PI * 180.f;
	ar.serialize(tmp, "CAMERA_THETA_MAX", "������������ ���� ������� ������");
	CAMERA_THETA_MAX = clamp(tmp, 0.f, 85.f) / 180.f * M_PI;

	tmp = CAMERA_THETA_MIN / M_PI * 180.f;
	ar.serialize(tmp, "CAMERA_THETA_MIN", "����������� ���� ������� ������");
	CAMERA_THETA_MIN = clamp(tmp / 180.f * M_PI, M_PI/36.f, 0.95f * CAMERA_THETA_MAX) ;

	tmp = CAMERA_THETA_DEFAULT / M_PI * 180.f;
	ar.serialize(tmp, "CAMERA_THETA_DEFAULT", "���� ������� ������ �� ���������");
	CAMERA_THETA_DEFAULT = clamp(tmp / 180.f * M_PI, CAMERA_THETA_MIN, CAMERA_THETA_MAX) ;

	ar.serialize(CAMERA_WORLD_SCROLL_BORDER, "CAMERA_WORLD_SCROLL_BORDER", "����������� ������ ����� ���������� �� ���� ��� ������������ ��������");
	CAMERA_WORLD_SCROLL_BORDER = clamp(CAMERA_WORLD_SCROLL_BORDER, -50.f, 1000.f);
	
	ar.serialize(unitFollowDistance, "unitFollowDistance", 0);
	ar.serialize(unitFollowTheta, "unitFollowTheta", 0);
	ar.serialize(unitHumanFollowDistance, "unitHumanFollowDistance", 0);
	ar.serialize(unitHumanFollowTheta, "unitHumanFollowTheta", 0);

	ar.serialize(directControlThetaFactor, "directControlThetaFactor", "����������� ��� �������� �������� ������ � ������ ����������");
	tmp = directControlPsiMax / M_PI * 180.f;
	ar.serialize(tmp, "directControlPsiMax", "����������� ������ � ������ ����������");
	directControlPsiMax = clamp(tmp, 5.0f, 120.0f) / 180.f * M_PI;
	ar.serialize(directControlRelaxation, "directControlRelaxation", "��������� ������ � ������ ����������");
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

Language::Language()
{
	codePage = 1250;
}

void Language::serialize(Archive& ar) {
	ar.serialize(language, "language", "����");
	ar.serialize(codePage, "codePage", "������� ��������");
}

//---------------------------------------------------------------------------------------

bool ShowHeadName::serialize(Archive& ar, const char* name, const char* nameAlt) 
{
	return ar.serialize(ModelSelector (fileName_, ModelSelector::HEAD_OPTIONS), name, nameAlt);
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
	enableSilhouettes = true;
	enablePathTrackingRadiusCheck = false;
	checkImpassabilityInDC = false;
	pathTrackingStopRadius = 100.0f;
	minRadiusCheck = 10.0f;
	enemyRadiusCheck = false;
	enableEnemyMakeWay = false;
	enableMakeWay = true;
	enableAutoImpassability = true;
	debrisLyingTime = 10;
	debrisProjectileLyingTime = 10;
	treeLyingTime = 10;
	for(int i = 0; i < 5; i++)
		playerColors.push_back(sColor4f(1,1,1,1));
	for(int i = 0; i < 5; i++)
		underwaterColors.push_back(UnitColor(sColor4c(0, 0, 255, 255), false));
	for(int i = 0; i < 5; i++)
		silhouetteColors.push_back(sColor4f(1,1,1,1));
	analyzeAreaRadius = 15.f;
	uniformCursor = true;
	useStackSelect = false;
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
}

void GlobalAttributes::serializeHeadLibrary (Archive& ar) 
{
	ar.serialize(showHeadNames, "showHeadNames", "������"); 
}

void GlobalAttributes::Sign::serialize(Archive& ar)
{
	ar.serialize(unitTexture, "unitTexture", "�������� �� ������");
	ar.serialize(sprite, "sprite", "������ ��� ����������");
}

void GlobalAttributes::LodBorder::serialize(Archive& ar)
{
	ar.serialize(radius,"radius","������ �������");
	ar.serialize(lod12,"lod12","������� �� lod1 � lod2");
	ar.serialize(lod23,"lod23","������� �� lod2 � lod3");
	ar.serialize(hideDistance,"hideDistance","���������� ������������");
}

void GlobalAttributes::serializeGameScenario(Archive& ar) 
{
	if(!ar.serialize(directControlMode, "directControlMode", "����� ������� ���������� �� ���������")){ // conversion 7.11
		bool enableDirectControl = false;
		ar.serialize(enableDirectControl, "enableDirectControlAtStart", 0);
		if(enableDirectControl)
			directControlMode = DIRECT_CONTROL_ENABLED;
	}

	ar.serialize(ResourceSelector(startScreenPicture_, ResourceSelector::UI_TEXTURE_OPTIONS), "startScreenPicture", "��������� �����");
	ar.serialize(enableSilhouettes, "enableSilhouettes", "�������� ��������� ��������");

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

	ar.openBlock("PathTracking", "PathTracking");
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

	ar.serialize(cameraRestriction, "cameraRestriction", "����������� ������");
	if(ar.isInput()) // CONVERSION 26.09.2006
		ar.serialize(cameraBorder, "cameraRestriction", 0);

	ar.serialize(mapSizeNames, "mapSizeNames", "�������� ������� ����");

	ar.serialize(cameraDefaultTheta_, "cameraDefaultTheta", 0);
	ar.serialize(cameraDefaultDistance_, "cameraDefaultDistance", 0);
	ar.serialize(environmentAttributes_, "environmentColors", "����� ��������� �����");

	ar.serialize(playerColors,     "playerColors",     "����� �������"); 
	ar.serialize(playerSigns, "playerSignes", "������� �������");
	ar.serialize(silhouetteColors, "silhouetteColors", "����� ��������"); 
	ar.serialize(serverCanChangeClientOptions, "serverCanChangeClientOptions", "������ ������� ���� ����� ������ ��������� ��������");

	ar.serialize(underwaterColors, "underwaterUnitColors", "����� ������� ��� �����");

	ar.serialize(uniformCursor,	"uniformCursor",	"������� �������� ���������� ���� ��� � ������� �����");
	ar.serialize(useStackSelect, "useStackSelect", "������������ �������� ������");
	ar.serialize(languagesList, "languagesList", "��������� �����");

	ar.serialize(resourseStatisticsFactors, "resourseStatisticsFactors", "������������ ��� ���������� �������� (���������� �� ��������������� ��������� � ������������)");
	ar.serialize(worldTriggers, "worldTriggers", "�������� ��� ����");
	ar.serialize(assistantDifficulty, "assistantDifficulty", "��������� �� ����������");

	ar.serialize(debrisLyingTime, "debrisLyingTime", "�����, ������� ������� ����� ����� ������");
	ar.serialize(debrisProjectileLyingTime, "debrisProjectileLyingTime", "�����, ������� ������� �������� ����� ����� ������");
	ar.serialize(treeLyingTime, "treeLyingTime", "�����, ������� ������� ����� ����� �������");

	ar.serialize(enableAnimationInterpolation, "enableAnimationInterpolation", "��������� ������������ ��������");
	if(enableAnimationInterpolation)
		ar.serialize(animationInterpolationTime, "animationInterpolationTime", "����� ������������ ��������");

	if(ar.isInput()){
		vector<sColor4f>::iterator i;
		FOR_EACH(playerColors, i)
			i->a = 1;
	}

	ar.serialize(circleManagerDrawOrder, "circleManagerDrawOrder", "������ ��������� �������� ������");
	ar.serialize(cheatTypeSpeed, "cheatTypeSpeed", "����������� �������� ����� �����, ������ � ������");
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


