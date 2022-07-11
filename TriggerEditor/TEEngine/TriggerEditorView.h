#ifndef __TRIGGER_EDITOR_VIEW_H_INCLUDED__
#define __TRIGGER_EDITOR_VIEW_H_INCLUDED__

#include "Scale/ScaleInterfaces.h"
#include "gdi_resource.h"
#include <boost/scoped_ptr.hpp>
#include "ITriggerEditorView.h"

class TriggerEditorLogic;
class ToolTip;
class TEGrid;
class TEMiniMapManager;
interface ITEWorkMode;

#define PARENT CView
class TriggerEditorView : public PARENT
						, public IScalable
						, public ITriggerEditorView
{
	typedef gdi_resource_guard<HFONT> Font;


protected:
	TriggerEditorView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(TriggerEditorView)

// Attributes
public:
	using PARENT::GetCurrentMessage;
	enum SCALE_ACTION{SA_MINUS, SA_PLUS, SA_RESET};
	//! Scroll Area Place
	enum {
		SAP_LEFT	= 1,
		SAP_RIGHT	= 2,
		SAP_TOP		= 4,
		SAP_BOTTOM	= 16,
	};

	virtual void SetScaleMgr(IScaleMgr* psmgr);
	virtual void IncreaseScale();
	virtual void DecreaseScale();
	virtual	float CurrentScale();
	virtual void SetScale(float fscale);

	virtual void redraw();

	TriggerEditorLogic* getTriggerEditorLogic() const;
	void setTriggerEditorLogic(TriggerEditorLogic* ptel);
	void setMiniMapManager(TEMiniMapManager* miniMap);

	void updateScrollers();
	void onFindReplace(LPFINDREPLACE findReplace);
	void scrollView(int dx, int dy);
	bool extendWorkArea();
	//! ��������� ���������� � ������� �������
	void updateViewWorkArea();
	//! ����� ������� ���� r ��������� � ����� ������� ����� ����
	void ensureVisibleRect(CRect const& r);

	ToolTip const* getToolTip() const;
	
	TEGrid const& getGrid() const;
	TEGrid& getGrid();

	HDC getDC() const;
	CRect const& getWorkArea() const;

	//! ���������� ��������� �� ����� � ����� ����
	DWORD pointInScrollArea(CPoint const& p);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TriggerEditorView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


// Implementation
protected:
	virtual ~TriggerEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(TriggerEditorView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpmis);
	BOOL OnTTNGetDispInfo(UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	afx_msg int OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();
protected:
	void offsetViewportOrg(int dx, int dy);

	bool testForBetweenPlots(CPoint p);

	void setScale(float fOldScale, float fNewScale);
	void setScale(SCALE_ACTION action);

	void setHorzScroller();
	void setVertScroller();

	//! ��������� ������� ���������������
	void setScaleRanges();
	//! ��������� ������� ��������������
	void setScrollRanges();
	//! ��������� ���������� � �������� � scale-���������
	void updateScaleInfo();
	//! ������ ���� ������� �������
	void createFont();

	void initViewportOrg();
	void initScale();

	void paintRect(CRect const& r);

	void extendWAToLeft(int ex);
	void extendWAToRight(int x);
	void extendWAToTop(int ex);
	void extendWAToBottom(int ex);

	//! ��������� ����� ���������� ������� ������� �� �����������
	int getWorkAreaHorzAdd() const;
	//! ��������� ����� ���������� ������� ������� �� ���������
	int getWorkAreaVertAdd() const;


	ITEWorkMode* getCurrentWorkMode() const;
public:
	void setViewportOrg(int x, int y);
	void setViewportOrg(CPoint const& point);
	CPoint const& getViewportOrg() const;

	void log2Scr(CSize* psz){
		POINT p0 = {0, 0};
		POINT p1 = {psz->cx, psz->cy};
		log2Scr(&p0, 1);
		log2Scr(&p1, 1);
		psz->cx = p1.x - p0.x;
		psz->cy = p1.y - p0.y;
	}
	//������� ���������
	void log2Scr(POINT* p, int cnt){
		for(int i = cnt; --i>=0;)
		{
			scalePoint(p);
			p->x += viewportOrg_.x;
			p->y += viewportOrg_.y;
			++p;
		}
	}
	
	void scr2Log(POINT* p, int cnt){
		for(int i = cnt; --i>=0;)
		{
			p->x -= viewportOrg_.x;
			p->y -= viewportOrg_.y;
			descalePoint(p);
			++p;
		}
	}
	
	void log2Scr(RECT* pr){
		log2Scr(reinterpret_cast<POINT*>(pr), 2);
	}
	
	void scr2Log(RECT* pr){
		scr2Log(reinterpret_cast<POINT*>(pr), 2);
	}
	int scaleX(const int x){
		return round(x*fScale_);
	}
	int scaleY(const int y){
		return round(y*fScale_);
	}

	int descaleX(const int x){
		return round(x/fScale_);
	}
	int descaleY(const int y){
		return round(y/fScale_);
	}

	void scalePoint(POINT*p){
		p->x = scaleX(p->x);
		p->y = scaleY(p->y);
	}
	void scalePoint(POINT*p, int cnt){
		for(int i = cnt; --i>=0;)
			scalePoint(p++);
	}
	
	void descalePoint(POINT*p){
		p->x = descaleX(p->x);
		p->y = descaleY(p->y);
	}
	void descalePoint(POINT*p, int cnt){
		for(int i = cnt; --i>=0;)
			descalePoint(p++);
	}
protected:	

	HFONT getFont() const{
		return font_.get();
	}

private:
	//! ������, ����������� ���������
	IScaleMgr* scaleManager_;
	TEMiniMapManager* miniMapManager_;
	//! 
	CPoint viewportOrg_;
	//! ������ �������
	float fScale_;
	//! ����������� �������
	float fMinScale_;
	//! ������������ �������
	float fMaxScale_;
	//! ����������� ������� ��������� �� �����������
	int iMinScrollLimX_;
	//! ������������ ������� ��������� �� �����������
	int iMaxScrollLimX_;
	//! ������������ ������� ��������� �� ���������
	int iMaxScrollLimY_;
	//! ����������� ������� ��������� �� ���������
	int iMinScrollLimY_;
	//! ����, ������� ��������� �����
	Font font_;
	//! DC ����
	CDC deviceContext_;
	//! ������� ������� ����
	CRect workAreaRect_;

	//! ������ ������
	TriggerEditorLogic *triggerEditorLogic_;
	//! ������ ����������� drag&drop
	COleDropTarget dropTarget_;
	//! ������
	boost::scoped_ptr<ToolTip> toolTip_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
