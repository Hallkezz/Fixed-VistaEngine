/********************************************************************
	created:	2003/05/23
	created:	23:5:2003   12:23
	filename: 	TESingleChainWorkMode.h
	file base:	TESingleChainWorkMode
	file ext:	h
	Powerd by:	�����  ��� ������� ����������� � ��������� ��������� of Scmorr
	
	purpose:	������� ������ ��� �������, ������� �������� � ����� ��������
*********************************************************************/
#ifndef __T_E_SINGLE_CHAIN_WORK_MODE_H_INCLUDED__
#define __T_E_SINGLE_CHAIN_WORK_MODE_H_INCLUDED__
#include "teworkmodebase.h"

#include "TriggerExport.h"
#include "TEEngine/Drawers/ITEDrawer.h"

#include "TEEngine/SelectionManager.h"

class TriggerEditorView;
class TriggerEditorLogic;

class TESingleChainWorkMode :
	public TEWorkModeBase
{
protected:
	class DataKeeper : public IDrawData
	{
	public:
		DataKeeper(TESingleChainWorkMode const* ptrWorkMode) : ptrWorkMode_(ptrWorkMode)
		{}
		DrawingData getDrawedChains() const{
			assert(ptrWorkMode_);
			return DrawingData(&ptrWorkMode_->triggerChain_, 1);
		}
		void setWorkMode(TESingleChainWorkMode const* ptrWorkMode){
			ptrWorkMode_ = ptrWorkMode;
		}
	private:
		TESingleChainWorkMode const* ptrWorkMode_;
	};
	friend DataKeeper;
protected:
	TESingleChainWorkMode(TriggerEditorLogic* logic);
	~TESingleChainWorkMode(void);

public:
	virtual bool OnStart();

	BOOL OnCommand(TriggerEditorView* source, WPARAM wParam, LPARAM lParam);
	void OnShowWindow(TriggerEditorView* source, BOOL bShow, UINT nStatus);

	virtual void setTriggerChain(TriggerChain* chain);
	virtual TriggerChain* getTriggerChain() const;
protected:
	void OnTeProperties(TriggerEditorView* source);
	void OnTeShowCondition(TriggerEditorView* source);
	virtual void trackToolTip(TriggerEditorView* source, CPoint point);

	TriggerEditorLogic* getTriggerEditorLogic() const;
	void setTriggerEditorLogic(TriggerEditorLogic* logic);

	Trigger& getStrategyTrigger(int idx);
	Trigger const& getStrategyTrigger(int idx) const;
	int getStrategyTriggerIndex(Trigger const& trigger);
	int getStrategyTriggerIndex(TriggerChain const& chain, 
								Trigger const& trigger);

	TriggerLink& getStrategyLink(Trigger& trigger, 
										int linkIndex);
	TriggerLink const& getStrategyLink(Trigger const& trigger, 
										int linkIndex) const;
	TriggerLink& getStrategyLink(int triggerIndex, int linkIndex);
	TriggerLink const& getStrategyLink(int triggerIndex, int linkIndex) const;

	SelectionManager& getSelectionManager();
	void setBaseElement(int baseElement);
	int  getBaseElement() const;

	class BaseElementGuard
	{
		TESingleChainWorkMode* mode_;
	public:
		BaseElementGuard(TESingleChainWorkMode* mode):mode_(mode){}
		~BaseElementGuard(){
			mode_->setBaseElement(-1);
		}
	};
	friend class BaseElementGuard;
private:
	//! �������, � ������� ����������� ������
	TriggerChain* triggerChain_;
	//! ������, ��������� ������� ������
	TriggerEditorLogic* triggerEditorLogic_;

	//! �������, �� ������� �������� ������
	int baseElement_;
protected:
	SelectionManager selectionManager_;

};

#endif
