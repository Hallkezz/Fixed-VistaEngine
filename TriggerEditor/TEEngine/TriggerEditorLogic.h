#ifndef __TRIGGER_EDITOR_LOGIC_H_INCLUDED__
#define __TRIGGER_EDITOR_LOGIC_H_INCLUDED__

class TriggerEditorView;

#include "TEEngine/WorkModes/ITEWorkMode.h"
#include "TEEngine/TEGrid.h"
#include "TEEngine/UndoManager/TEUndoManager.h"
#include "TriggerExport.h"

#include <boost/scoped_ptr.hpp>

class TEBaseWorkMode;
class TEBaseWorkModeDrawer;
class TEProfilerWorkMode;
class TEStatusColoredEleDrawer;

class TriggerEditor;
interface ITriggerProfList;

class TriggerEditorLogic
{
	class BoundingRect;
public:
	TriggerEditorLogic(TriggerEditor* triggerEditor);
	virtual ~TriggerEditorLogic(void);

	void setTriggerEditorView(TriggerEditorView* ptev);
	TriggerEditorView* getTriggerEditorView() const;

	void setTriggerProfList(ITriggerProfList* ptrTriggerProfList);

	BOOL onCommand(TriggerEditorView* pwnd, WPARAM wParam, LPARAM lParam);
	BOOL onUpdateCommandUI(TriggerEditorView* pwnd, CCmdUI* pCmdUI);

	void onRButtonDown(TriggerEditorView* source, UINT nFlags, CPoint point);
	void onRButtonUp(TriggerEditorView* source, UINT nFlags, CPoint point);

	void onLButtonDown(TriggerEditorView* source, UINT nFlags, CPoint point);
	void onLButtonUp(TriggerEditorView* source, UINT nFlags, CPoint point);
	void onLButtonDblClk(TriggerEditorView* source, UINT nFlags, CPoint point);
	void onMouseMove(TriggerEditorView* source, UINT nFlags, CPoint point);
	BOOL onSetCursor(TriggerEditorView* source, CWnd* pWnd, UINT nHitTest, UINT message);

	void OnShowWindow(TriggerEditorView* source, BOOL bShow, UINT nStatus);

	void onKeyDown(TriggerEditorView *source, UINT nChar, UINT nRepCnt, UINT nFlags);
	void onKeyUp(TriggerEditorView *source, UINT nChar, UINT nRepCnt, UINT nFlags);

	BOOL onDropData(TriggerEditorView* source, void* pData, 
					DROPEFFECT dropEffect, CPoint const& point);
	DROPEFFECT onDragOver(TriggerEditorView* source, void* pData, 
								DWORD dwKeyState, CPoint const& point);

	int findTrigger(TriggerChain const& chain, CPoint const &point) const;
	std::pair<int, int> const findLink(TriggerChain const& chain,
									const CPoint &p) const;
	int findLinkByChild(Trigger const& trigger, 
						Trigger const& child) const;
	TEGrid const& getGrid() const;
	TEGrid & getGrid();

	//! ��������������� �������������, ������������ ��� ��������
	static void remakeStrategyRect(TriggerChain const& chain);


	void updateCellIndexes(Trigger& trigger, CPoint const& leftTop) const; 
	void updateChainViewRect(TriggerChain& chain) const;

	void setTriggerChain(TriggerChain * chain);
	TriggerChain* getTriggerChain() const;
	ITEWorkMode* getCurrentWorkMode() const;

	ActionPtr createAction(int index) const;

	void setDataChanged(bool bValue = true);
	bool isDataSaved() const;

	//! \brief ����������, ���� ������������, ��������� ���������. 
	//! ����������, ���� ������������, ��������� ������
	//! ���������� ����������� ������������� ��� �������.
	void adjustTriggerChainLayout(TriggerChain& chain) const;


	void deleteTrigger(TriggerChain &chain, 
									int triggerIndex, 
									TEUndoManager::Bunch& undoBunch,
									TEUndoManager::Bunch& redoBunch);

	bool removeLink(TriggerChain& chain, 
								TriggerLink const& link,
								int linkOwnerIndex, 
								int childElementIndex,
								TEUndoManager::Action& aUndo, 
								TEUndoManager::Action& aRedo);
protected:

	//! ��������� ������� ������, ��� ������� ������� ��� �� ���������
	void adjustLinksLayout(TriggerList& triggers) const;

	//! \brief ������������� ��� ��������� ������ �����, ���� ������������, 
	//! � ���������� ��������������
	void adjustTriggersLayout(TriggerList& triggers) const;
	//! \brief ������������� ������ ����� ��� ���� ���������
	void adjustTriggersCellsForAll(TriggerList& triggers) const;
	//! \brief ������������� ������ ����� ��� ��������� c ��������������� �������� �����
	void adjustTriggersCellsForNew(TriggerList& triggers) const;

	enum TriggerCellIndexesTest{
		TCIT_NONE, ///< ��� �������� ����� �������������� ������ �����
		TCIT_ALL_TRIGGERS,///< ��� �������� ����� ���������������� ������ �����
		TCIT_SOME_TRIGGERS,///< ��������� �������� ����� ���������������� ������ �����
	};
	//! ���������, ���� �� �������� � ����������������� ��������� �����
	TriggerCellIndexesTest hasTriggersInvalidCellIndexes(
							TriggerList const& triggers) const;
	//! ���������� ������ ����� ������� � ������ ������ ������� �����
	RECT const getVisibleCells() const;

	void onViewProfiler(TriggerEditorView* source);
	void OnUpdateViewProfirler(TriggerEditorView* source, CCmdUI* pCmdUI);

	void onTEActivateProfiler(TriggerEditorView* source);
	void OnUpdateTEActivateProfiler(TriggerEditorView* source, CCmdUI* pCmdUI);

	//! ������� �����, ������ �� �������� � � ����
	void removeLinks(TriggerChain &chain, 
								 int triggerIndex, 
								 TEUndoManager::Bunch& undoBunch, 
								 TEUndoManager::Bunch& redoBunch);
	void removeIncomingLinks(TriggerChain &chain, 
										 int triggerIndex, 
										 TEUndoManager::Bunch& undoBunch,
										 TEUndoManager::Bunch& redoBunch);
	void removeOutcomingLinks(TriggerChain& chain, 
										  int triggerIndex, 
											TEUndoManager::Bunch& undoBunch,
											TEUndoManager::Bunch& redoBunch);
protected:
	void setCurrentWorkMode(ITEWorkMode* currentWorkMode);
private:
	//! ����, � ������� ����� ����������� ������� ���������
	TriggerEditorView* triggerEditorView_;
	//! ������� ������� �����
	ITEWorkMode* currentWorkMode_;
	//! �������� �����
	TEGrid grid_;
	//! ������� ������� �����
	boost::scoped_ptr<TEBaseWorkMode> baseWorkMode_;
	//! ����������� ��� �������� �������� ������
	boost::scoped_ptr<TEBaseWorkModeDrawer> baseWorkModeDrawer_;
	//! ����� ������ ���������
	boost::scoped_ptr<TEProfilerWorkMode> profilerWorkMode_;
	//! ����������� ��� ����������� ������
	boost::scoped_ptr<TEStatusColoredEleDrawer> profilerWorkModeDrawer_;

	//! ������� ���������, � ������� �������� � ������� ������
	TriggerChain* triggerChain_;

	TriggerEditor* triggerEditor_;
};

#endif
