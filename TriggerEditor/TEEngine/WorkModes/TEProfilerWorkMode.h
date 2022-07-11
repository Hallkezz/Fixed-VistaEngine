#ifndef __T_E_PROFILER_WORK_MODE_H_INCLUDED__
#define __T_E_PROFILER_WORK_MODE_H_INCLUDED__

#include "TEEngine/WorkModes/TESingleChainWorkMode.h"
#include <memory>

interface ITEDrawer;
interface ITriggerProfList;

class TEProfilerWorkMode :
	public TESingleChainWorkMode
{
public:
	TEProfilerWorkMode(TriggerEditorLogic* logic);
	~TEProfilerWorkMode(void);
	void setDrawer(ITEDrawer* ptrDrawer);
	virtual void OnLButtonDown(TriggerEditorView* source, UINT nFlags, CPoint point);
	virtual void OnRButtonUp(TriggerEditorView* source, UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(TriggerEditorView* source, UINT nFlags, CPoint point);
	virtual void OnKeyUp(TriggerEditorView *source, UINT nChar, 
						UINT nRepCnt, UINT nFlags);
	virtual void OnPaint(TriggerEditorView* source, CDC* pdc, CRect const& rcPaint);
	virtual bool OnStart();
	virtual bool OnEnd();
	virtual void ModifyMenu(CMenu* pmenu, UINT menuResId, TriggerEditorView* source);

	void setTriggerProfList(ITriggerProfList* ptrTriggerProfList);
	ITriggerProfList* getTriggerProfList() const;

protected:
	void trackToolTip(TriggerEditorView* source, CPoint point);
private:
	//! ������� �����������, ��� ������ ������
	ITEDrawer *drawer_;
	//! ������ ��� ���������
	std::auto_ptr<DataKeeper> drawDataKeeper_;
	ITriggerProfList* ptrTriggerProfList_;
};

#endif
