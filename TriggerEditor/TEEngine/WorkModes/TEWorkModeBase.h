/********************************************************************
	created:	2003/05/21
	created:	21:5:2003   15:40
	filename: 	TEWorkModeBase.h
	file base:	TEWorkModeBase
	file ext:	h
	Powerd by:	����� ��� ������� ����������� � ��������� ��������� of Scmorr
	
	purpose:	������� ����������. ����������� ������ �� ������, ����� �����
				����� ��� ���� �������
*********************************************************************/
#ifndef __T_E_WORK_MODE_BASE_H_INCLUDED__
#define __T_E_WORK_MODE_BASE_H_INCLUDED__

#include "ITEWorkMode.h"

class TEWorkModeBase : public ITEWorkMode
{
protected:

public:
	TEWorkModeBase(void);
	virtual ~TEWorkModeBase(void) = 0;

	void OnLButtonDown(TriggerEditorView* source, UINT nFlags, CPoint point);
	void OnLButtonUp(TriggerEditorView* source, UINT nFlags, CPoint point);
	void OnLButtonDblClk(TriggerEditorView* source, UINT nFlags, CPoint point);

	void OnRButtonDown(TriggerEditorView* source, UINT nFlags, CPoint point);
	void OnRButtonUp(TriggerEditorView* source, UINT nFlags, CPoint point);

	void OnMouseMove(TriggerEditorView* source, UINT nFlags, CPoint point);
	BOOL OnSetCursor(TriggerEditorView* source, CWnd* pWnd, UINT nHitTest, UINT message);

	void OnKeyDown(TriggerEditorView* source, UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnKeyUp(TriggerEditorView* source, UINT nChar, UINT nRepCnt, UINT nFlags);

	void OnPaint(TriggerEditorView* source, CDC* pdc, CRect const& rcPaint);
	BOOL OnCommand(TriggerEditorView* source, WPARAM wParam, LPARAM lParam);

	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis);
	void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpmis);

	void OnShowWindow(TriggerEditorView* source, BOOL bShow, UINT nStatus);

	bool OnStart();
	bool OnEnd();

	BOOL OnDropData(TriggerEditorView* source, void* pData, const CPoint& point);
	bool CanDrop(TriggerEditorView* source, void* pData, const CPoint& point);

	virtual bool OnCommandUpdateUI(TriggerEditorView *source, CCmdUI* pCmdUI);

	void onFindReplace(TriggerEditorView* source) {};
protected:
	BOOL trackPopupMenu(UINT id, TriggerEditorView* source);
	virtual void ModifyMenu(CMenu* pmenu, UINT menuResId, TriggerEditorView* source);
	virtual void trackToolTip(TriggerEditorView* source, CPoint point);
protected:
	void setBasePoint(CPoint const& point);
	CPoint const& getBasePoint() const{
		return m_base_point;
	}
	void setCanDragContent(bool bVal);
	bool getCanDragContent() const{
		return m_bCanDragContent;
	}
	void setCursor(HCURSOR hCursor);
	HCURSOR getCursor() const{
		return m_hCursor;
	}

private:
	//����� ����, � ������� ��������� ���������� �������
	CPoint m_base_point;
	//����� �� ������������� ���������� ����
	bool m_bCanDragContent;
	//������ ��� ������
	HCURSOR m_hCursor;
};

#endif
