#ifndef __MOVE_LINK_END_H_INCLUDED__
#define __MOVE_LINK_END_H_INCLUDED__

#include "TriggerExport.h"

class MoveLinkEnd
{
public:
	enum MLESelectEnd{
		MLE_CHILD_OFFSET, MLE_OWNER_OFFSET
	};
	MoveLinkEnd(int linkIndex, int linkParentIndex, 
		TriggerChain const& chain,
		CPoint const& offset, MLESelectEnd eval);
	~MoveLinkEnd(void);
	bool operator()();
	static bool run(int linkIndex, int linkParentIndex, 
		TriggerChain const& chain, 
		CPoint const& offset, MLESelectEnd eval);
private:
	//! ���������� ����� �����
	int linkIndex_;
	//! ���������� ����� ��������, �� �������� ����� �������
	int linkParentIndex_;
	//! ���������, ������� ����������� �������
	TriggerChain const& chain_;
	//! ������
	CPoint offsetSize_;
	//! ����� ����� ����� ��������
	MLESelectEnd linkEnd_;
};

#endif
