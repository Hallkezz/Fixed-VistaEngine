#ifndef __NODE_3DX_H_INCLUDED__
#define __NODE_3DX_H_INCLUDED__
#include "Static3dx.h"
#include "Render\Src\observer.h"
#include "Render\Src\nparticle.h"
#include "XMath\Mats.h"
#include "Render\Src\Leaves.h"
/*
	����������� cObject3dx. 
	65536 ��������� �������� ����� ���� � ������. 
	�������� 256 ���. (��� �� ��� ��������� �������� � GetNodePosition)
	�������� 32 ������ ��������� � ����� set. (SetVisibilityGroup)
*/

struct cNode3dx
{
	Mats pos;
	float phase;
	BYTE chainIndex;
	BYTE index_scale;
	BYTE index_position;
	BYTE index_rotation;
	BYTE additional_transform;//255 - ��� �������
	bool IsAdditionalTransform() const { return additional_transform != 255; }

	inline void calculatePos(StaticNodeAnimations& animations, Mats& pos);
	cNode3dx();
};

struct c3dxAdditionalTransformation
{
	bool used;
	Mats mat;
};

class RENDER_API cObject3dxAnimation
{
public:
	cObject3dxAnimation(cStatic3dx* pStatic);
	cObject3dxAnimation(cObject3dxAnimation* obj);
	cStatic3dx* GetStatic(){return pStatic;}//������ ��� ������

	void SetPhase(float phase);
	void SetAnimationGroupPhase(int ianimationgroup,float phase);

	float GetAnimationGroupPhase(int ianimationgroup);
	void SetAnimationGroupChain(int ianimationgroup,const char* chain_name);
	void SetAnimationGroupChain(int ianimationgroup,int chain_index);
	int GetAnimationGroupChain(int ianimationgroup);

	int GetChainNumber();
	StaticAnimationChain* GetChain(int i);
	bool SetChain(const char* chain);
	void SetChain(int chain_index);
	int GetChainIndex(const char* chain_name);//���������� ������, ������� ������������ ��� chain_index

	//nodeindex - ������ �� ������� cStatic3dx::nodes
	int FindNode(const char* node_name) const;//������ � cStatic3dx::nodes (-1=�� �����)
	const Se3f& GetNodePosition(int nodeindex) const;//�������� ������ ������ ����� ������� Update, ���������� ��������� ������������ ���������� ������� ���������
	const Mats& GetNodePositionMats(int nodeindex) const;
	const MatXf& GetNodePositionMat(int nodeindex) const;
	int GetNodeNumber() const;
	int GetParentNumber(int node_index) const;
	bool CheckDependenceOfNodes(int dependent_node_index, int node_index) const;
	const char* GetNodeName(int node_index) const;

	void UpdateMatrix(Mats& position,vector<c3dxAdditionalTransformation>& additional_transformations);

	void GetNodeOffset(int nodeindex,Mats& offset,int& parent_node);

protected:
	int FindChain(const char* chain_name);
	void CalcOpacity(int imaterial);

	cStatic3dx* pStatic;
	bool updated_;
	bool treeUpdated_;
	//������� �������� cNode3dx �����, ��� 
	//child ������ ��������� ����� parent
	vector<cNode3dx> nodes_;

	struct  MaterialAnim
	{
		float phase;
		int chain;
		float opacity;
	};
	vector<MaterialAnim> materials;

	friend class cObject3dxAnimationSecond;
	friend class cObject3dx;
};

class RENDER_API cObject3dxAnimationSecond : public cObject3dxAnimation
{
public:
	cObject3dxAnimationSecond(cStatic3dx* pStatic);
	cObject3dxAnimationSecond(cObject3dxAnimationSecond* pObj);
	float GetAnimationGroupInterpolation(int ianimationgroup);
	void SetAnimationGroupInterpolation(int ianimationgroup,float k);

	bool IsInterpolation();
protected:
	friend class cObject3dx;
	vector<float> interpolation;
};

class RENDER_API cObject3dx : public c3dx, public cObject3dxAnimation
{//root ������ ����� ������� ���������� �� ��������������
public:
	cObject3dx(cStatic3dx* pStatic,bool interpolate);
	cObject3dx(cObject3dx* pObj);
	~cObject3dx();
	void SetPosition(const Se3f& pos);
	void SetPosition(const MatXf& Matrix);
	const MatXf& GetPosition() const;
	const Se3f& GetPositionSe() const;
	const Mats& GetPositionMats() const;

	void SetScale(float scale);
	float GetScale()const{return position.scale();}

	bool unvisible() const { return unvisible_; }

	float GetBoundRadius() const;
	void GetBoundBox(sBox6f& box) const;
	void GetBoundBoxUnscaled(sBox6f& box);

	void PreDraw(Camera* camera);
	void Draw(Camera* camera);
	void Animate(float dt);

	void DrawLine(Camera* camera);

	int GetAnimationGroupNumber();
	int GetAnimationGroup(const char* name);//���������� ������, ������� ������������ ��� ianimationgroup
	const char* GetAnimationGroupName(int ianimationgroup);

	int GetVisibilityGroupNumber(VisibilitySetIndex iset = VisibilitySetIndex::ZERO){return GetVisibilitySet(iset).GetVisibilityGroupNumber();};
	const char* GetVisibilityGroupName(VisibilityGroupIndex group,VisibilitySetIndex iset = VisibilitySetIndex::ZERO){return GetVisibilitySet(iset).GetVisibilityGroupName(group);};
	VisibilityGroupIndex GetVisibilityGroupIndex(const char* group_name,VisibilitySetIndex iset = VisibilitySetIndex::ZERO){return GetVisibilitySet(iset).GetVisibilityGroupIndex(group_name);};
	void SetVisibilityGroup(VisibilityGroupIndex group,VisibilitySetIndex iset=VisibilitySetIndex::ZERO);
	bool SetVisibilityGroup(const char* name, bool silently = false,VisibilitySetIndex iset = VisibilitySetIndex::ZERO);

	StaticVisibilityGroup* GetVisibilityGroup(VisibilitySetIndex iset=VisibilitySetIndex::ZERO);
	VisibilityGroupIndex GetVisibilityGroupIndex(VisibilitySetIndex iset);
	
	int GetVisibilitySetNumber(){return pStatic->visibilitySets_.size();};
	const char* GetVisibilitySetName(VisibilitySetIndex iset);
	VisibilitySetIndex GetVisibilitySetIndex(const char* set_name);

	bool IntersectSphere(const Vect3f& p0, const Vect3f& p1) const;
	bool IntersectBound(const Vect3f& p0, const Vect3f& p1) const;
	const VoxelBox& voxelBox() const { return pStatic->voxelBox; }
	const char* GetFileName() const;

	//SetNodePosition ����� ��������� ������ ��� ����� ATTR3DX_NOUPDATEMATRIX
	//��� ������� ��������� � ��� ������, ��� ����� �������, ��������� ��� ����� ������ UpdateMatrix ������� ��������� �������
	void SetNodePosition(int nodeindex,const Se3f& pos);

	void SetNodePositionMats(int nodeindex,const Mats& pos);

	void SetUserTransform(int nodeindex,const Se3f& pos);//�� ���������� ���������, ���� ������������� ����� ��� ������ node

	//���������� �������� ���������, �������� ���������� � ����� ��������� ������� � 3DS Max
	//�� ������ �� �� ���, ����� ��������, ������������� ����������� � �������.
	bool GetVisibilityTrack(int nodeindex) const;

	//���������� true ���� �� ���� ��������� ���� ������� �� ���������� � ��������.
	bool GetVisibilityTrackInterval(int nodeindex,float begin_phase,float end_phase) const;

	//RestoreNodeMatrix �������� �������� SetUserTransform
	void RestoreUserTransform(int nodeindex);
	bool HasUserTransform(int node_index) const;

	void CopyUserTransformToSecondAnimation(int nodeindex);
	void RestoreSecondAnimationUserTransform();

	void SetSilhouetteIndex(int index);
	int GetSilhouetteIndex() const { return silhouette_index; }

	void SetSkinColor(Color4c skin_color, const char* emblem_name_ = "");//������� �������, ������ �������� (���� ������ �� ������� PreloadElement c ������� �����������)
	Color4c GetSkinColor() const {return skin_color;};

	//ambient.a,specular.a  ������������ ������������ � ������� �� ��������� ��������� � ���������.
	//!diffuse.a - ������������ �������
	void SetColorOld(const Color4f *ambient,const Color4f *diffuse,const Color4f *specular=0);
	void GetColorOld(Color4f *ambient,Color4f *diffuse,Color4f *specular=0) const;

	//ambient.a,specular.a,diffuse.a  ������������ ������������ � ������� �� ��������� ��������� � ���������.
	void SetColorMaterial(const Color4f *ambient,const Color4f *diffuse,const Color4f *specular=0);
	void GetColorMaterial(Color4f *ambient,Color4f *diffuse,Color4f *specular=0) const;

	//������������ �������.
	virtual void SetOpacity(float opacity);
	virtual float GetOpacity()const;

	//���� �������� � ����������� �� ��������� lerp_color.� ��������. 0 - ���� ��������, 1 - ���� lerp_color.
	void SetTextureLerpColor(const Color4f& lerp_color);
	Color4f GetTextureLerpColor() const;

	void DrawLogic(Camera* camera,int selected=-1);
	void DrawLogicBound();
	void DrawBound() const;
	void drawBoundSpheres() const;

	//������������� �������������� �������.
	//����� ��������, ���� ������� ������ ��������� ������ �� �����
	void Update();

	void AddLink(ObserverLink* link){observer.AddLink(link);}
	void BreakLink(ObserverLink* link){observer.BreakLink(link);}

	//������������ ���� ������ � �������.
	//���� object=0 - �����������.
	void LinkTo(cObject3dx* object,int inode,bool set_scale=true);

	//������ ��� ��������������� ������ ��������� ������������ � �����.
	void GetEmitterMaterial(struct cObjMaterial& material);

	//������ ��� �����, ���������� ����� ������ �� ��� ����� ������ �� triangles ���� �� visible_points
	void GetTriangleInfo(TriangleInfo& all,DWORD tif_flags,int selected_node=-1);
	void GetVisibilityVertex(vector<Vect3f> &pos, vector<Vect3f> &norm);

	//debug statistics
	int GetMaterialNum();
	int GetNodeNum();
	/////////////

	void SetLodDistance(float lod12,float lod23);
	void SetHideDistance(float distance);
	static void SetUseLod(bool enable);
	static bool GetUseLod();

	//SetCircleShadow,EnableSelfShadow() - ��������� ����� ��� ���� �������� � ����� ��������� (GetFileName())
	//height -������, ����� �������� ����
	void SetCircleShadowParam(float radius,float height=-1);
	void SetShadowType(ObjectShadowType type);
	ObjectShadowType getShadowType();
	void getCircleShadowParam(float& radius, float& height);

	cObject3dxAnimationSecond* GetInterpolation(){return pAnimSecond;};

	void SetLod(int ilod);
	void AutomaticSetLod(){ clearAttribute(ATTRUNKOBJ_NO_USELOD); }

	void EnableSelfIllumination(bool enable);
	void setAttribute(int attribute);
	void clearAttribute(int attribute);

	const MatXf& GetNodePositionMat(int nodeindex) const{return cObject3dxAnimation::GetNodePositionMat(nodeindex);};
	const Mats& GetNodePositionMats(int nodeindex) const{return cObject3dxAnimation::GetNodePositionMats(nodeindex);};

	int GetNodeNumber() const{return cObject3dxAnimation::GetNodeNumber();}
	const char* GetNodeName(int node_index) const{return cObject3dxAnimation::GetNodeName(node_index);}

	void DrawAll(Camera *camera);
	void AddLight(cUnkLight* light);//��� SceneLightProc

	bool QueryVisibleIsVisible();
	void QueryVisible(Camera* camera);
	void DisableDetailLevel();//��� ������ ���������, ������ ��� ������������ ����� �����.

	void SetSilouetteCenter(Vect3f center){silouette_center=center;}
	const Vect3f& GetSilouetteCenter()const{return silouette_center;}
	void GetTextureNames(vector<string>& texture_names);

	sBox6f CalcDynamicBoundBox(const MatXf& world_view);//������ ��� ��������! � ���������� �����������.
	StaticVisibilitySet& GetVisibilitySet(VisibilitySetIndex iset);

	bool IsLogicBound(int nodeindex);
	sBox6f GetLogicBoundScaledUntransformed(int nodeindex);
	int GetNumPolygons();
	int GetNumOutputPolygons()const{return num_out_polygons;}
	cTexture* GetDiffuseTexture(int num_mat)const;

	//����� �� ��������, ��� SetSkinColor ���������� ���� ��������
	//������ ������ AddRef
	void SetDiffuseTexture(int num_mat,cTexture* texture);
	float GetFov(){return pStatic->cameraParams.fov;}
	void SetFurScalePhase(float phase);
	void SetFurAlphaPhase(float phase);
	float GetFurScalePhase(){return furScalePhase_;}
	float GetFurAlphaPhase(){return furAlphaPhase_;}

protected:
	struct EffectData
	{
		cEffect* pEffect;
		BYTE index_visibility;
		float prev_phase;
		EffectData():pEffect(0),index_visibility(255){}//255 - ������������������.
	};
	float distance_alpha;
	int iLOD;
	bool isSkinColorSet_;
	bool isHaveSkinColorMaterial_;
	bool isTree_;
	bool unvisible_;
	
	struct VisibilityGroup
	{
		VisibilityGroupIndex visibilityGroupIndex;
		StaticVisibilityGroup* visibilityGroup;
		float alpha;
		VisibilityGroup() : visibilityGroupIndex(0), visibilityGroup(0), alpha(1.f) {}
	};
	vector<VisibilityGroup> visibilityGroups_;
	Mats position;

	struct MaterialTexture
	{
		cTexture* diffuse_texture;
		float texture_phase;
		bool is_opacity_vg;
		MaterialTexture() : diffuse_texture(0), texture_phase(0), is_opacity_vg(false) {}
	};
	vector<MaterialTexture> material_textures;

	Color4c skin_color;
	Color4f ambient,diffuse,specular;
	Color4f lerp_color;
	float object_opacity;

	vector<c3dxAdditionalTransformation> additional_transformations;
	vector<EffectData> effects;
	Observer observer;

	class EffectObserverLink3dx:protected ObserverLink
	{
		cObject3dx* parent_object;
		int parent_node;
		cObject3dx* this_object;
		bool set_scale;
	public:
		EffectObserverLink3dx():parent_object(0),parent_node(-1),this_object(0),set_scale(true){}
		void SetParent(cObject3dx* this_object_){this_object=this_object_;}

		void Link(class cObject3dx* object,int inode,bool set_scale);
		virtual void Update();
		bool IsInitialized(){return observer!=0;}
	} link3dx;

	typedef vector<cUnkLight*> Lights;
	Lights point_light;//���������, ������� ������ �� ������
	Lights lights;//��������� ������ �������.
	Leaves* leaves_;

	cObject3dxAnimationSecond* pAnimSecond;//��� ������������ ������������ �������.
	unsigned char silhouette_index;

	void CalcBoundingBox();
	void CalcBoundSphere();

	void LoadTexture(bool only_skinned,bool only_self_illumination=false, const char* texture_name = "");
	void ProcessEffect(Camera* camera);

	void GetWorldPoses(const StaticBunch& s,MatXf* world,int& world_num) const;
	void GetWorldPos(const StaticBunch& s,MatXf& world, int idx) const;
	void AddCircleShadow();
	void UpdateVisibilityGroups();
	inline void UpdateVisibilityGroup(VisibilitySetIndex iset);

	void DrawMaterialGroup(StaticBunch& s);
	void DrawMaterialGroupSelectively(StaticBunch& s,const Color4f& color,bool draw_opacity,
							class VSSkin* vs,class PSSkin* ps);

	bool isVisible(const cTempVisibleGroup& vg) const;
	bool isVisibleMaterialGroup(StaticBunch& bunch) const;

	void CalcIsOpacity(	bool& is_opacity,bool& is_noopacity);

	float border_lod12_2,border_lod23_2;
	float hideDistance;

	class cOcclusionSilouette* pOcclusionQuery;

	c3dxAdditionalTransformation& GetUserTransformIndex(cNode3dx& s);

	int num_out_polygons;

	Vect3f silouette_center;
	void SetUVTrans(class VSSkinBase* vs,StaticMaterial& mat,MaterialAnim& mat_anim);
	void SetSecondUVTrans(bool use,class VSSkinBase* vs,StaticMaterial& mat,MaterialAnim& mat_anim);
	void UpdateAndLerp();
	
	bool IsHaveSkinColorMaterial();

	void DrawShadowAndZbuffer(Camera* camera,bool ZBuffer=false);

	void DrawFur(Camera* camera);
	float furScalePhase_;
	float furAlphaPhase_;
};

#endif
