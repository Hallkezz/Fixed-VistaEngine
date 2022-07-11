#ifndef __T_E_BASE_WORK_MODE_DRAWER_BASE_H_INCLUDED__
#define __T_E_BASE_WORK_MODE_DRAWER_BASE_H_INCLUDED__
/********************************************************************
	created:	2003/05/24
	created:	24:5:2003   16:49
	filename: 	d:\Projects\Quest\QuestEditor\TEBaseWorkModeDrawerBase.h
	file path:	d:\Projects\Quest\QuestEditor
	file base:	TEBaseWorkModeDrawerBase
	file ext:	h
	Powerd by:	����� ��� ������� ����������� � ��������� ��������� of Scmorr
	
	purpose:	������� ����� ��� ���������� �������� ������ ������
*********************************************************************/
//#include "TEEngine/Drawers/TEConditionColoredEleDrawer.h"
#include "TEEngine/Drawers/TESelfColoredEleDrawer.h"

class TriggerEditorView;
struct TriggerLink;

class TEBaseWorkModeDrawerBase :
//	public TEConditionColoredEleDrawer
	public TESelfColoredEleDrawer
{
public:
	TEBaseWorkModeDrawerBase(void);
	~TEBaseWorkModeDrawerBase(void);

	//������ ���� � �������, ������������ �������� ����� ��������� �����
	virtual void drawAnchoredLink(TriggerEditorView* view, HDC dc, 
		Trigger const& link_owner, TriggerLink const& link) const = 0;
	//������ ����, ����� �� ��� ����� �� ������ �������� � �������
	virtual void drawDragedLink(TriggerEditorView* view, HDC dc, const CPoint& pt_from, 
		const CPoint& pt_to, int type) const = 0;
	//������ ������������� �������
	virtual void drawDragedTitle(TriggerEditorView* view, 
		std::string const& strTitle, CPoint pos) const = 0;
	//���������� ����� ��������������� ����� ������� ���
	virtual HBRUSH getBrushByLinkType(int type) const = 0;
	//������ ����� ��� �����
	virtual void drawLinkAnchors(TriggerEditorView* view, HDC dc, 
		Trigger const& link_owner, TriggerLink const& link) const = 0;
};

#endif
