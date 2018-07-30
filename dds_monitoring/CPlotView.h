// CPlotView.h : interface of the CCPlotView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPLOTVIEW_H__E362E6DC_B955_4971_9741_2AEF4226DE3B__INCLUDED_)
#define AFX_CPLOTVIEW_H__E362E6DC_B955_4971_9741_2AEF4226DE3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Chart.h"

class CCPlotView : public CView
{
protected: // create from serialization only
	CCPlotView();
	DECLARE_DYNCREATE(CCPlotView)

// Attributes
public:
	CPoint			m_LastPoint;
	int				m_CurDataSet;
	CXYChart		m_Chart;
	int graphtype;
	CCPlotDoc* GetDocument();
	bool isDataEmpty = true;
// Operations
public:

	void setData(CCPlotDoc* pDoc, int type = 1); //¼öÁ¤_¼Õ¹Î¿µ_02_27
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPlotView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCPlotView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
void	OnSetColorTable( UINT id );
void	OnSelectChart( UINT id );
void	OnUpdateSelectChart( CCmdUI *cmd );
void	OnDrawPrint( CDC *pDC, CRect boundingRect );

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCPlotView)
	afx_msg void OnPlotsettingsPlotSetup();
	afx_msg void OnPlotsettingsAxisSetup();
	afx_msg void OnPlotsettingsDataSetup();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in CPlotView.cpp
inline CCPlotDoc* CCPlotView::GetDocument()
   { return (CCPlotDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLOTVIEW_H__E362E6DC_B955_4971_9741_2AEF4226DE3B__INCLUDED_)
