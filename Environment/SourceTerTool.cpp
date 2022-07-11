#include "StdAfx.h"

// #include "..\Terra\geo2.h"
// #include "..\Terra\break.h"

#include "SourceTerTool.h"
#include "UnitAttribute.h"

void SourceTerTool::serialize(Archive& ar)
{
	ar.serialize(terToolReference, "terToolReference", "������");
	SourceEffect::serialize(ar);
	ar.serialize(period, "period", "������");
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
    if( sleepTimer() || isUnderEditor()) {
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
        pose_.trans().z = vMap.GetApproxAlt(position().xi(), position().yi());
        char surkind=vMap.getSurKindG(xg, yg);
        xassert(surkind>=0 && surkind<TERRAIN_TYPES_NUMBER);
        if(!vMap.isIndestructabilityG( xg, yg )){
            if(terToolReference->terToolNrml && terToolReference->interplayArr[surkind]==true ){
                pCurTerTool=terToolReference->terToolNrml->clone();
                //pCurTerTool->setPosition(pose(),true);
                generateToolserPosition();
                //pCurTerTool->setScale(currentScaleFactor);
                pCurTerTool->stopOnInds(true);
            }
			else {
				flag_end=true; //for requestKill
                kill();//setActivity(false);
			}
        }
        else {
            if(terToolReference->terToolInds && terToolReference->interplayArr[surkind]==true ){
                pCurTerTool=terToolReference->terToolInds->clone();
                //pCurTerTool->setPosition(pose(),true);
                generateToolserPosition();
                //pCurTerTool->setScale(currentScaleFactor);
            }
			else {
				flag_end=true; //for requestKill
                kill();//setActivity(false);
			}
        }
        return/* true*/;
    }
    else {
        bool result=pCurTerTool->pQuant();
        if(!result){
            if(period){
                setActivity(true);
                sleepTimer.start(period);
            }
			else {
				flag_end=true; //for requestKill
                kill();//setActivity(false);
			}
        }
        return/* result*/;
    }
}
