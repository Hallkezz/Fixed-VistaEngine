#ifndef _VISGENERIC_H_
#define _VISGENERIC_H_

class cScene;
class cObjLibrary;
class cInterfaceRenderDevice;
class cTexture;
class cTextureScale;
class cFont;

#ifndef _FINAL_VERSION_
#define POLYGON_LIMITATION
#endif

#ifdef POLYGON_LIMITATION
extern int polygon_limitation_max_polygon;
extern int polygon_limitation_max_polygon_simply3dx;
#endif

#include "..\inc\VisGenericDefine.h"
#include "MTSection.h"

using namespace std;

enum eShadowType
{
	SHADOW_NONE=0,
	SHADOW_MAP=2,
	SHADOW_MAP_SELF=4,
};

enum eShowType
{
	SHOW_NORMAL=0,
	SHOW_SHADOW,
	SHOW_REDLECTION,
	SHOW_TILEMAP,
	SHOW_OBJECT,
	SHOW_INFO,
	SHOW_MAX
};

class cVisGeneric : public cUnknownClass
{
public:
	// ����������������� ����� 
	cVisGeneric(bool multiThread);
	virtual ~cVisGeneric();
	cInterfaceRenderDevice* GetRenderDevice();
	// ������� ��� ������ �� ������
	virtual cScene* CreateScene();

	void ReloadAllFont();

	//������ ����. height - ������ ����� � ���������� ��������.
	//� 1024x768 ���������� ������� ����� �������.
	virtual cFont* CreateFont(const char *TextureFileName,int height=20, bool silentErr=0);
	virtual cFont* CreateFontMem(void* pFontData,int size,int height=20, bool silentErr=0);
	virtual cFont* CreateDebugFont();

	// ������� ��� ������ �� ���������
	virtual cTexture* CreateTexture(const char *TextureName);
	virtual cTexture* CreateTexture(int sizex,int sizey,bool alpha);//������ 32 ������ ��������
	virtual cTexture* CreateBumpTexture(int sizex,int sizey);
	virtual cTexture* CreateRenderTexture(int width,int height,int attr=0,bool enable_assert=true);
	virtual cTextureScale* CreateTextureScale(const char *TextureName,Vect2f scale);

	virtual cTexture* CreateTextureDefaultPool(int sizex,int sizey,bool alpha);//������ 32 ������ ��������

	//���������� 32 ������ ��������, � ������� �������� ����������� ������.
	virtual cTexture* CreateTextureScreen();

	void SetEffectLibraryPath(const char* effect_path,const char* texture_path);

	//
	void SetMipMapLevel(int);
	void SetDrawNumberPolygon(bool);
	void SetShowRenderTextureDBG(bool);
	void SetFavoriteLoadDDS(bool);
	void SetAnisotropic(int);
	int GetAnisotropic();
	int GetMaxAnisotropyLevel();

	//������� ����������� ���������� �������. 
	//0 - ������ �������� � �����. 1 - � 2 ���� ������, � �.�.
	void SetTextureDetailLevel(int level);
	int GetTextureDetailLevel();

	void SetUseLod(bool enable);
	bool GetUseLod();
	void SetRestrictionLOD(int delete_lod);//0 - ������ �� �������, 1 - ������� LOD ������� ������, 2 - �������.
	int  GetRestrictionLOD();

	void SetShadowType(eShadowType shadow_type,int shadow_size);//shadow_size=0..3
	eShadowType GetShadowType();
	bool PossibilityShadowMapSelf4x4();
	void SetShadowMapSelf4x4(bool b4x4);

	//�������� ���� ��� �������� - OST_SHADOW_NONE - ��� �����, 
	//OST_SHADOW_CIRCLE - ���� ���� ���������� ��� ���� ��������, ��� ������� OST_SHADOW_CIRCLE ��� OST_SHADOW_REAL
	//OST_SHADOW_REAL - ���� ��� ��� ������� � ���������� �������.
	void SetMaximalShadowObject(c3dx::OBJECT_SHADOW_TYPE type);
	c3dx::OBJECT_SHADOW_TYPE GetMaximalShadowObject()const{return maximal_shadow_object;}


	//����������� ���� ����������� ��������� ��������� ��� �����.
	void SetTileMapTypeNormal(bool pixel_normal);

	//�������� �������������� ��������.
	void SetTilemapDetail(bool b);
	bool GetTilemapDetail();

	void SetMapLevel(float);//0..100 ����������� �����.

	//������� � ��������. ���� ����� ������� �� �����, 
	//� �� ����������� ����� ���� ��������, ������� �� ����� ������������ ������,
	//�� ������ �����.
	void EnableSilhouettes(bool enable); 
	bool IsSilhouettesEnabled() const { return silhouettes_enabled; }
	void SetSilhouetteColor(int index, sColor4c color);
	sColor4c GetSilhouetteColor(int index) const;
	int GetSilhouetteColorCount() const;

	//
	void SetEnableBump(bool enable);
	bool GetEnableBump();
	bool PossibilityBump();

	//
	bool PossibilityOcclusion();
	void EnableOcclusion(bool b);

	void SetFloatZBufferType(int type);
	int GetFloatZBufferType();

	void SetUseTextureCache(bool enable);
	bool GetUseTextureCache();
	void SetUseMeshCache(bool enable);
	bool GetUseMeshCache();

	void SetEnableDOF(bool enable);
	bool GetEnableDOF();

	bool PossibilityBumpChaos();

	static void SetAssertEnabled(bool enabled) { assertEnabled_ = enabled; }
	static bool IsAssertEnabled() { return assertEnabled_; }

	void SetUseLogicQuant(bool use){use_logic_quant=use;}
	bool GetUseLogicQuant(){return use_logic_quant;}

	//������ � ���������� ������
	void SetLogicQuant(int quant){logic_quant=quant;}
	int GetLogicQuant(){return logic_quant;};

	//������ � ����������� ������
	void SetGraphLogicQuant(int quant){graph_logic_quant=quant;}
	int GetGraphLogicQuant(){return graph_logic_quant;};

	//������ ������� �� ������. ����� ��� ��� ��������, ������� ���� ���� ���������������.
	void SetInterpolationFactor(float t){interpolation_factor=t;}
	float GetInterpolationFactor(){return interpolation_factor;}
	void SetLogicTimePeriodInv(float time){logic_time_period_inv=time;}
	float GetLogicTimePeriodInv()const{return logic_time_period_inv;}// 1/ms

	void SetGlobalParticleRate(float r);

	//��� ��������, ������� ������������� ��������� � �������.
	const char* GetEffectPath()const{return effect_path.c_str();};
	const char* GetEffectTexturePath()const{return effect_texture_path.c_str();};

	class cLib3dx* GetObjLib(){return Lib3dx;}

	//������ ����� �������� � ����������� �� ����������.
	float GetHideFactor();
	void SetHideFactor(float hide_factor);
	bool GetHideSmoothly();
	void SetHideSmoothly(bool hideSmoothly);//true - ������ �������� ��������� ���������� ����������.
	float GetHideRange();
	void SetHideRange(float hide_range);//������ ���� HideSmoothly=true. �� ����� ���������� ���������� ������ ��������.

	bool GetShadowTSM();
	void SetShadowTSM(bool enable);

	MTSection& GetLockResetDevice(){return lock_reset_device;};
	bool IsMultithread(){return is_multithread;};

	void SetShowType(eShowType type,bool show);
	void XorShowType(eShowType type);
	bool GetShowType(eShowType type);
	void DrawInfo();
	class CameraShader* GetShaders(){return shaders;}

	void SetWorkCacheDir(const char* dir);
	void SetBaseCacheDir(const char* dir);
private:
	virtual void SetData(cInterfaceRenderDevice *pData1);
	virtual void ClearData();

	int			logic_quant,graph_logic_quant;
	bool		use_logic_quant;

	class cLib3dx			*Lib3dx;
	class cLibSimply3dx		*LibSimply3dx;
	vector<class cFontInternal*>	fonts;
	friend class cD3DRender;
	class CameraShader* shaders;

	void InitShaders();
	void ReleaseShaders();

	void CalcIsShadowMap();

	string effect_path,effect_texture_path;
	float interpolation_factor;
	float logic_time_period_inv;

	bool	 silhouettes_enabled;
	sColor4c silhouette_color;
	std::vector<sColor4c> silhouette_colors;

	static bool assertEnabled_;

	MTSection lock_reset_device;
	bool is_multithread;

	c3dx::OBJECT_SHADOW_TYPE maximal_shadow_object;
};

extern class cVisGeneric		*gb_VisGeneric;

class MTTexObjAutoLock
{
	MTAuto object_lock;//���� �� ������������, 
	MTAuto texture_lock;
public:
	MTTexObjAutoLock();
};

void SetLavaTexture(const char* file_name);
#endif // _VISGENERIC_H_
