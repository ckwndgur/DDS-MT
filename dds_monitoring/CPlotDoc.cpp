// CPlotDoc.cpp : implementation of the CCPlotDoc class
//

#include "stdafx.h"
#include "dds_monitoring.h"
#include <math.h>
#include "CPlotDoc.h"
#include "MainSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	M_PI		3.141592653589793

/////////////////////////////////////////////////////////////////////////////
// CCPlotDoc

IMPLEMENT_DYNCREATE(CCPlotDoc, CDocument)

BEGIN_MESSAGE_MAP(CCPlotDoc, CDocument)
	//{{AFX_MSG_MAP(CCPlotDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPlotDoc construction/destruction

CCPlotDoc::CCPlotDoc()
{
	m_WhichChart = ID_CHART_XYCHART;
}

CCPlotDoc::~CCPlotDoc()
{
	ASSERT( 1 );
}

BOOL CCPlotDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCPlotDoc serialization

void CCPlotDoc::Serialize(CArchive& ar)
{
	imageChart.Serialize( ar );
	xyChart.Serialize( ar );

	if (ar.IsStoring())
	{
		ar << m_WhichChart;
	}
	else
	{
		ar >> m_WhichChart;
		UpdateAllViews(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCPlotDoc diagnostics

#ifdef _DEBUG
void CCPlotDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCPlotDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCPlotDoc commands
