#ifndef __EDITOR_ACTION_H_INCLUDED__
#define __EDITOR_ACTION_H_INCLUDED__

#include "Handle.h"

class ControlsTree;

/// ����������� ���� ��� ���� ��������� �������� ��������������.
class EditorAction : public ShareHandleBase
{
public:
    EditorAction(){};
	virtual ~EditorAction(){};
    /// ����� ����������� ��������.
    virtual void act() = 0;
    /// ����� ����������� ����� ��������.
    virtual void undo() = 0;
    /// ������� �������� �������� (�������������� � ���� Edit->Undo).
	virtual std::string description() const = 0;
protected:
	ControlsTree* controlsTree();
};

#endif
