#ifndef __I_TREE_NODE_H_INCLUDED__
#define __I_TREE_NODE_H_INCLUDED__

class TETreeLogic;
class xTreeListCtrl;
class CTreeListItem;

interface IUITreeNode
{
	//! �������� ���� � ������
	virtual bool load(xTreeListCtrl& tree, CTreeListItem* pParent) = 0;
	//! ��������� ������ �� �������  ����
	virtual bool onCommand(TETreeLogic& logic, WPARAM wParam, LPARAM lParam) = 0;
	//! ��������� ����� ��������������
	virtual void onBeginDrag(TETreeLogic& logic) = 0;
	//! ���������� ��������� �� �������� ����
	virtual void onDeleteItem(TETreeLogic& logic) = 0;
};

#endif
