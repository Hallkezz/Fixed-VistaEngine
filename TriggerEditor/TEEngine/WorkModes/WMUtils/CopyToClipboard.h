#ifndef __COPY_TO_CLIPBOARD_H_INCLUDED__
#define __COPY_TO_CLIPBOARD_H_INCLUDED__

#include "TriggerExport.h"

class SelectionManager;
class TriggerClipBuffer;

class CopyToClipBoard  
{
public:
	CopyToClipBoard();
	~CopyToClipBoard();
	bool copy(TriggerChain const& chain,
		TriggerClipBuffer& clipBuffer, 
		SelectionManager const& selMngr);
protected:
	//! ��������� ������ �� �����, ������� ��������� �������� � ������ ���������
	void filterLinks(TriggerList& triggers);
	void makeTriggerCellIndexesRelative(
				TriggerList& triggers) const;

};

#endif
