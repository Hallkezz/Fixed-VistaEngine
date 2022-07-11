/********************************************************************
	created:	2002/09/17
	created:	17:9:2002   12:41
	filename: 	D:\Projects\Quest\QuestEditor\te_helpers.h
	file path:	D:\Projects\Quest\QuestEditor
	file base:	te_helpers
	file ext:	h
	Powerd by:	�����
	
	purpose:	��������������� ������� ��� ��������� ���������
*********************************************************************/
#ifndef _TE_HELPERS_HEADER_
#define _TE_HELPERS_HEADER_

#include "TriggerExport.h"

namespace te_h{

//!\brief ���������� ����� ������� ��������� ��� � ��������������
//! ��� ������� �� ����� ������!!! ��������������
BYTE check_inters(const CPoint& a, const CPoint& b, const CPoint* pts,
								float* pkx, float* pky);


inline CPoint const get_center(RECT const& r)
{
	return CPoint((r.right + r.left)>>1, (r.bottom + r.top)>>1);
}

inline CPoint const get_center(CPoint const& a, CPoint const& b)
{
	return CPoint((a.x + b.x)>>1, (a.y + b.y)>>1);
}

inline CPoint const get_lt(RECT const& r)
{
	return CPoint(r.left, r.top);
}
inline bool IsKeyPressed(int keyID)
{
	return GetAsyncKeyState(keyID)>>15 != 0;
}

typedef CPoint TRIANGLE[3];
void getLinkTriangle(CPoint const& pt_from, CPoint const& pt_to, TRIANGLE &trg);
bool pointInTrg( CPoint const& test, TRIANGLE const& trg);


//! ��������� �����, ��� ����� ����������
CPoint const getLinkBegin(Trigger const& ownedTrigger, 
						  TriggerLink const& link);
//! ��������� �����, ��� ����� ���������
CPoint const getLinkEnd(TriggerLink const& link);
//! ��������������� ����� ������
//void recalcCellNumber(Trigger& trigger, CPoint const& p, int cell_w, int cell_h);

//! ��������� ������ �� ������ bound �� ����� ������������� �����
CSize getLinkOffset(const CPoint& ptFrom, const CPoint& ptTo, const RECT& bound);
}//namespace te_h


#endif//_TE_HELPERS_HEADER_
