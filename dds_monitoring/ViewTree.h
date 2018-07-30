
#pragma once
#include "EditTreeCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CViewTree window

class CViewTree : public CEditTreeCtrlEx
{
// Construction
public:
	CViewTree();

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()
};
