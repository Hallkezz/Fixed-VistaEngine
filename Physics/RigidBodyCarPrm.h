#ifndef __RIGID_BODY_CAR_PRM_H__
#define __RIGID_BODY_CAR_PRM_H__

#include "Units\Object3dxInterface.h"
#include "Serialization\StringTableReferencePolymorphic.h"
#include "Render\3dx\Node3DX.h"

class cObject3dx;
struct RigidBodyPrm;
typedef StringTableReferencePolymorphic<RigidBodyPrm, true> RigidBodyPrmReference;

///////////////////////////////////////////////////////////////
//
//    struct WheelDescriptor
//
///////////////////////////////////////////////////////////////

struct WheelDescriptor 
{
	Logic3dxNode nodeLogic;
	Object3dxNode nodeGraphics;
	bool frontWheel;

	WheelDescriptor() : frontWheel(false) {}
	void serialize(Archive& ar) {
		ar.serialize(nodeLogic, "nodeLogic", "��� ���������� ����");
		ar.serialize(nodeGraphics, "nodeGraphics", "��� ����������� ����");
		ar.serialize(frontWheel, "frontWheel", "�������� ������");
	}
};

///////////////////////////////////////////////////////////////

typedef vector<WheelDescriptor> WheelDescriptorList;

///////////////////////////////////////////////////////////////
//
//    struct RigidBodyWeelPrm
//
///////////////////////////////////////////////////////////////

struct RigidBodyWheelPrm
{
public:
	///////////////////////////////////////////////////////////
	//
	//    struct RigidBodyWeelPrm
	//
	///////////////////////////////////////////////////////////
	
	enum WheelType{
		FREE_WHEEL = 0,
		STEERING_WHEEL = 1,
		DRIVING_WHEEL = 2,
		LEFT_WHEEL = 2 << 1,
		RIGHT_WHEEL = 2 << 2,
		FRONT_WHEEL = 2 << 3,
		REAR_WHEEL = 2 << 4
	};

	///////////////////////////////////////////////////////////

	Object3dxNode nodeGraphicsWheel;
	Object3dxNode nodeGraphicsSuspension;
	Logic3dxNodeBound nodeLogic;
	float radius;
	Vect3f displacement;
	float suspentionMovement;
	int wheelType;

	RigidBodyWheelPrm() : 
		radius(0.0f),
		displacement(Vect3f::ZERO),
		suspentionMovement(1.0f),
		wheelType(DRIVING_WHEEL)
	{
	}
	void update(cObject3dx* modelLogic, const Vect3f& center)
	{
		sBox6f boundBox(modelLogic->GetLogicBoundScaledUntransformed(nodeLogic));
		radius = (boundBox.max.z - boundBox.min.z) * 0.5f;
		displacement = modelLogic->GetNodePosition(nodeLogic).trans();
		displacement.x += (boundBox.max.x - boundBox.min.x) * 0.5f;
		displacement -= center;
		if(displacement.x < -FLT_EPS)
			wheelType |= LEFT_WHEEL;
		else
			wheelType &= ~LEFT_WHEEL;
		if(displacement.x > FLT_EPS)
			wheelType |= RIGHT_WHEEL;
		else
			wheelType &= ~RIGHT_WHEEL;
		if(displacement.y > FLT_EPS)
			wheelType |= FRONT_WHEEL;
		else
			wheelType &= ~FRONT_WHEEL;
		if(displacement.y < -FLT_EPS)
			wheelType |= REAR_WHEEL;
		else
			wheelType &= ~REAR_WHEEL;
	}
	void serialize(Archive& ar) 
	{
		ar.serialize(nodeGraphicsSuspension, "nodeGraphicsSuspension", "��� ����������� ���� ��� ��������");
		ar.serialize(nodeGraphicsWheel, "nodeGraphicsWheel", "��� ����������� ���� ��� ������");
		ar.serialize(nodeLogic, "nodeLogic", "��� ���������� ����");
		ar.serialize(suspentionMovement, "suspentionMovement", "��� �������� (������������ ������� ������)");
		ar.serialize(radius, "radius", 0);
		ar.serialize(displacement, "displacement", 0);
		ar.serialize(wheelType, "wheelType", 0);
		if(ar.isEdit()){
			bool steeringWheel = (wheelType & STEERING_WHEEL) != 0;
			bool drivingWheel = (wheelType & DRIVING_WHEEL) != 0;
			ar.serialize(steeringWheel, "steeringWheel", "����������� ������");
			ar.serialize(drivingWheel, "drivingWheel", "������� ������");
			if(steeringWheel)
				wheelType |= STEERING_WHEEL;
			else
				wheelType &= ~STEERING_WHEEL;
			if(drivingWheel)
				wheelType |= DRIVING_WHEEL;
			else
				wheelType &= ~DRIVING_WHEEL;
		}
	}
};

///////////////////////////////////////////////////////////////
//
//    class RigidBodyCarPrm
//
///////////////////////////////////////////////////////////////

class RigidBodyCarPrm : public vector<RigidBodyWheelPrm> 
{
public:
	RigidBodyPrmReference rigidBodyPrm;
	bool turnInPosition;
	
	RigidBodyCarPrm();
	void update(cObject3dx* modelLogic, const Vect3f& center)
	{
		turnInPosition = true;
		iterator i;
		FOR_EACH(*this, i){
			i->update(modelLogic, center);
			if(i->wheelType & RigidBodyWheelPrm::STEERING_WHEEL)
				turnInPosition = false;
		}
	}
	void serialize(Archive& ar);	
};

#endif // __RIGID_BODY_CAR_PRM_H__