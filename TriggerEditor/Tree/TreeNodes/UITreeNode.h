#ifndef __U_I_TREE_NODE_H_INCLUDED__
#define __U_I_TREE_NODE_H_INCLUDED__


#include "TREE\TREENODES\ITreeNode.h"
#include <string>

class UITreeNodeFabric;
class xTreeListCtrl;
class CTreeListItem;

class UITreeNode : public IUITreeNode  
{
	friend class UITreeNodeFabric;
protected:
	UITreeNode(std::string const& action, int ordinalNumber);
	virtual ~UITreeNode();
public:

	//! �������� ���� � ������
	virtual bool load(xTreeListCtrl& tree, CTreeListItem* pParent);
	//! ��������� ������ �� �������  ����
	virtual bool onCommand(TETreeLogic& logic, WPARAM wParam, LPARAM lParam);
	//! ��������� ����� ��������������
	virtual void onBeginDrag(TETreeLogic& logic);
	//! ���������� ��������� �� �������� ����
	virtual void onDeleteItem(TETreeLogic& logic);
protected:
	//! ���������� ��� ��������
	std::string const& getAction() const;
	//! ��������� ��� ��������
	void setAction(std::string const& action);
	//! ���������� ���� �������� � ������
	CTreeListItem* getTreeListItem() const;
	//! ��������� ���� �������� � ������
	void setTreeListItem(CTreeListItem* pItem);
	//! ���������� ���������� ����� ��������
	int getOrdinalNumber() const;
	//! ��������� ���������� �����
	void setOrdinalNumber(int ordNum);
private:
	//! ��� ��������
	std::string action_;

	//! ���������� ����� ��������
	int ordinalNumber_;
	//! ��������� �� ���� �������� � ������
	CTreeListItem* treeListItem_;
};

#endif
