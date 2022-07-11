#ifndef __TOOL_TIP_H_INCLUDED__
#define __TOOL_TIP_H_INCLUDED__

class CWnd;

class ToolTip  
{
public:
	ToolTip();
	virtual ~ToolTip();
	CString const& getToolTipText() const;
	//! ��������� ����� �������
	void setToolTipText(CString const& str) const;
	//! ���������� ������
	void trackToolTip(CPoint const& pos) const;
	//! �������� �������
	HWND create(HWND owner);
	void setOwnerWindow(HWND owner);
	HWND getOnwenerWindow() const;

	//! �������� �� OnTTNGetDispInfo ����
	BOOL OnTTNGetDispInfo(UINT id, NMHDR * pTTTStruct, LRESULT * pResult ) const;
private:
	//! ����� ��������
	mutable CString toolTipText_;
	//! ���� �������
	HWND toolTipWindow_;
	//! ���� ���������
	HWND ownerWindow_;
};

#endif
