#ifndef __T_E_BASE_WORK_MODE_DRAWER_H_INCLUDED__
#define __T_E_BASE_WORK_MODE_DRAWER_H_INCLUDED__
/********************************************************************
	created:	2003/05/23
	created:	23:5:2003   18:11
	filename: 	d:\Projects\Quest\QuestEditor\TEBaseWorkModeDrawer.h
	file path:	d:\Projects\Quest\QuestEditor
	file base:	TEBaseWorkModeDrawer
	file ext:	h
	Powerd by:	����� ��� ������� ����������� � ��������� ��������� of Scmorr
	
	purpose:	���������� �������� ������
*********************************************************************/
#include "tebaseworkmodedrawerbase.h"

class TEBaseWorkMode;
class TriggerEditorView;

class TEBaseWorkModeDrawer :
	public TEBaseWorkModeDrawerBase
{
public:
	TEBaseWorkModeDrawer(TEBaseWorkMode* ptrWorkMode);
	~TEBaseWorkModeDrawer(void);

	virtual void draw(CDC* pdc, TriggerEditorView* view, 
						IDrawData* pdd, CRect const& rcPaint) const;

	virtual void drawAnchoredLink(TriggerEditorView* view, HDC dc, 
		Trigger const& link_owner, TriggerLink const& link) const;
	virtual void drawDragedLink(TriggerEditorView* view, HDC dc, 
		const CPoint& pt_from, const CPoint& pt_to, int type) const;
	virtual void drawDragedTitle(TriggerEditorView* view, 
				std::string const& strTitle, CPoint pos) const;
	void drawLinkAnchors(TriggerEditorView* view, HDC dc, 
		Trigger const& link_owner, TriggerLink const& link) const;
	virtual HBRUSH getBrushByLinkType(int type) const{
		return getLinkTypeBrush(type);
	}
protected:
	BOOL setPenCant(HPEN hpen);
	HPEN getPenCant() const;

	virtual HBRUSH	SelectLinkBrushColor(TriggerLink const& link) const;
	virtual HPEN	SelectLinkPenColor(TriggerLink const& link) const;

	BOOL setLinkOutPen(HPEN hpen);
	BOOL setLinkInPen(HPEN hpen);
	BOOL setOtherLinksPen(HPEN hpen);

	BOOL setLinkOutBrush(HPEN hbrush);
	BOOL setLinkInBrush(HPEN hbrush);
	BOOL setOtherLinksBrush(HPEN hbrush);

	HPEN getLinkOutPen() const;
	HPEN getLinkInPen() const;
	HPEN getOtherLinksPen() const;

private:
	//! ���� ������ ���������� �����, �� ���� ���������� ������ �����
	CPen cantPen_;
	//! ��������� ��� ������, ����� ���������� ����� ������ �/�� ��������
	//! �������� ��� ��������� ������
	CPen linkOutPen_;
	//! �������� ��� �������� ������
	CPen linkInPen_;
	//�������� ��� ��������� ������
	CPen otherLinksPen_;

	//! ����� ��� ������, ����� ���������� ����� ������ �/�� ��������
	//����� ��� ��������� ������
	CBrush linkOutBrush_;
	//����� ��� �������� ������
	CBrush linkInBrush_;
	//����� ��� ��������� ������
	CBrush otherLinksBrush_;

	//������� �����, �� ������� �� ��������
	TEBaseWorkMode *workMode_;
};

#endif
