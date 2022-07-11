#include "StdAfx.h"

// #include "Terra\geo2.h"
// #include "Terra\break.h"

#include "SourceTerTool.h"
#include "UnitAttribute.h"
#include "Serialization\MillisecondsWrapper.h"

void SourceTerTool::serialize(Archive& ar)
{
	ar.serialize(terToolReference, "terToolReference", "������");
	SourceEffect::serialize(ar);
	//ar.serialize(period, "period", "������");
	ar.serialize(MillisecondsWrapper(sleepTime_), "sleepTime_", "�������� �������");
	ar.serialize(flag_autoKill, "flag_autoKill", "������������(�� �� �������)");
	//ar.serialize(angle_disp_, "angle_disp", "������� ��������");
	ar.serialize(flag_dispersion, "flag_dispersion", "��������� �������");
	//if(!ar.isEdit())
	//	ar.serialize(pose_.trans(), "pos", "�������");
	serializationApply(ar);
}

void SourceTerTool::start()
{
	__super::start();
	release();
	sleepTimer_.start(sleepTime_);
}

void SourceTerTool::stop()
{
	__super::stop();
	//if(pCurTerTool)
	//	pCurTerTool->startNextAnimationChain();
	release();
}

void SourceTerTool::quant()
{
	__super::quant();
    if(sleepTimer_.busy() || isUnderEditor()) {
        return/* false*/;
    }
	if(!terToolReference) {//
		kill(); //���� ��������� � start!
		return;
	}
    if(!pCurTerTool){
        if(!active_)
            return;
//			effectStart();//��� ������ �������
        int xg=position().xi()>>kmGrid;
        int yg=position().yi()>>kmGrid;
        pose_.trans().z = vMap.getApproxAlt(position().xi(), position().yi());
		if(terToolReference->terTool){
			pCurTerTool=terToolReference->terTool->clone();
			//pCurTerTool->setPosition(pose(),true);
			generateToolserPosition();
			//pCurTerTool->setScale(currentScaleFactor);
			pCurTerTool->stopOnInds(true);
			pCurTerTool->setSurKind4Working((int)surfaceKind_);
			pCurTerTool->setRadiusTerrainType(radius_);
		}
		else {
			flag_end=true; //for requestKill
			kill();//setActivity(false);
		}
        return/* true*/;
    }
    else {
        bool result=pCurTerTool->pQuant();
        if(!result){
			//if(period){
			//	setActivity(true);
			//	sleepTimer.start(period);
			//}
			//else {
				flag_end=true; //for requestKill
				kill();//setActivity(false);
			//}
        }
        return/* result*/;
    }
}
