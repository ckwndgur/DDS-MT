// CPlotDoc.h : interface of the CCPlotDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPLOTDOC_H__5ED1A3D2_A706_419E_8975_17C8C42CDA11__INCLUDED_)
#define AFX_CPLOTDOC_H__5ED1A3D2_A706_419E_8975_17C8C42CDA11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Chart.h"

class CCPlotDoc : public CDocument
{
protected: // create from serialization only
	CCPlotDoc();
	DECLARE_DYNCREATE(CCPlotDoc)

// Attributes
public:

// Operations
public:
	CImageChart		imageChart;
	CXYChart		xyChart;
	unsigned int	m_WhichChart;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPlotDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCPlotDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCPlotDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLOTDOC_H__5ED1A3D2_A706_419E_8975_17C8C42CDA11__INCLUDED_)
