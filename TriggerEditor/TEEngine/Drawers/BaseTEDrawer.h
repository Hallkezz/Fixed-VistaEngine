/********************************************************************
	created:	2003/05/23
	created:	23:5:2003   15:58
	filename: 	d:\Projects\Quest\QuestEditor\BaseTEDrawer.h
	file path:	d:\Projects\Quest\QuestEditor
	file base:	BaseTEDrawer
	file ext:	h
	Powerd by:	����� ��� ������� ����������� � ��������� ��������� of Scmorr
	
	purpose:	���������� ������� ������������. ��������� ���������, ������, �����.
*********************************************************************/
#ifndef __BASE_T_E_DRAWER_H_INCLUDED__
#define __BASE_T_E_DRAWER_H_INCLUDED__

#include "TEEngine/Drawers/TEDrawerBase.h"
#include "TEEngine/Drawers/IDrawData.h"

class TriggerEditorView;

class BaseTEDrawer : public TEDrawerBase
{
public:
	BaseTEDrawer(void);
	virtual ~BaseTEDrawer(void);
	void draw(CDC* pdc, TriggerEditorView* source, 
				IDrawData* pdd, CRect const& rcPaint) const;
	void drawDraggedRect(HDC dc, CRect const&r, HPEN hpen) const;
	void drawNet(CDC* pdc, TriggerEditorView* source, CRect const& rcPaint) const;
protected:
	virtual HBRUSH	SelectTriggerBackBrush(Trigger const& trigger) const;
	virtual HBRUSH	SelectLinkBrushColor(TriggerLink const& link) const;
	virtual HPEN	SelectLinkPenColor(TriggerLink const& link) const;

protected://������� � �������
//	void setStrategyOffset(CSize const& sz) const;
//	CSize const& getStrategyOffset() const;

	BOOL setNetPen(int nPenStyle, int nWidth, COLORREF crColor);
	BOOL setNetPen(HPEN hPen);
	HPEN getNetPen() const;

	BOOL setAutoRestartedLinkPen(int nPenStyle, int nWidth, COLORREF crColor);
	BOOL setAutoRestartedLinkPen(HPEN hPen);
	HPEN getAutoRestartedLinkPen() const;

	void setDrawedChain(TriggerChain const* chain) const;
	TriggerChain const* getDrawedChain() const;

	BOOL setDefaultTriggerBrush(HBRUSH hBrush);
	BOOL setDefaultTriggerBrush(COLORREF crColor);
	HBRUSH getDefaultTriggerBrush() const;

	BOOL setSelectedTriggerBrush(HBRUSH hBrush);
	BOOL setSelectedTriggerBrush(COLORREF crColor);
	HBRUSH getSelectedTriggerBrush() const;

	void setDrawedLinkOwner(Trigger const* trigger) const;
	Trigger const* getDrawedLinkOwner() const;

	void setPaintRect(CRect const& rc) const;
	CRect const& getPaintRect() const;
protected:
	//! ������������ �������
	void DrawTrigger(TriggerEditorView* source, 
					CDC* pdc, 
					Trigger const& trigger) const;
	//! ������������ �����
	void DrawLink(TriggerEditorView* source, HDC dc, CPoint pt_from, CPoint pt_to, 
					TriggerLink const& link) const;
	//! ������������ ��� ����� � �������
	void DrawLinks(TriggerEditorView* source, CDC* pdc, 
					TriggerChain const& chain) const;
	//! ������������ ��� ����� ��������
	void DrawTriggerLinks(TriggerEditorView* source, CDC* pdc, 
		const RECT& rcEle, Trigger const& trigger) const;
	//! ������������ ��� �������� � �������
	void DrawTriggers(TriggerEditorView* source, CDC* pdc, 
		const CRect& rcPaint, TriggerChain const& chain) const;
	void DrawChain(TriggerEditorView* source, CDC* pdc, const CRect& rcPaint, 
		TriggerChain const& chain) const;
private:
	static void DrawLink(HDC dc, const CPoint& pt_from, const CPoint& pt_to, 
		CPoint* ptrg, HPEN pen, HGDIOBJ brush);
private:
	//��������, ������� �������� �����
	CPen netPen_;
	CPen autoRestartedLinkPen_;
	//! ������� �������������� � ������� ������
	mutable TriggerChain const* drawedStrategy_;
	//! ����� ��� ��������� ��������. ��� �����������.
	mutable CString drawedTriggerTitle_;

	//�������� ������� ������� �� � ���������� ���������.
	//������������ ��� ��������� ���� ������� �����.
//	mutable CSize strategyOffset_;

	//�����, ������� �������� ����� �������� ��������
	CBrush defaultTriggerBrush_;
	//�����, ������� �������� ����� ���������� ��������
	CBrush selectedTriggerBrush_;

	//! �������� �������� �����
	mutable Trigger const* drawedLinkOwner_;
	//! ������������� ������� ����������
	//! ��������������� � ������ ���������� � �������������� ����� ��
	mutable CRect paintRect_;
};

#endif
