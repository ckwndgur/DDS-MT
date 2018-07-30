// CPlotView.cpp : implementation of the CCPlotView class
//

#include "stdafx.h"
#include "dds_monitoring.h"

#include <math.h>

#include "Chart.h"
#include "CPlotDoc.h"
#include "PlotSettingsDialog.h"
#include "AxisSettings.h"
#include "ColorControl.h"
#include "DataSettingsDialog.h"
#include "MainFrm.h"
#include "CPlotUtility.h"
#include "CPlotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		kNoUpdatePoint		-1000000

/////////////////////////////////////////////////////////////////////////////
// CCPlotView

IMPLEMENT_DYNCREATE(CCPlotView, CView)

BEGIN_MESSAGE_MAP(CCPlotView, CView)
	//{{AFX_MSG_MAP(CCPlotView)
	ON_COMMAND(ID_PLOTSETTINGS_PLOTSETUP, OnPlotsettingsPlotSetup)
	ON_COMMAND(ID_PLOTSETTINGS_AXISSETUP, OnPlotsettingsAxisSetup)
	ON_COMMAND(ID_PLOTSETTINGS_DATASETUP, OnPlotsettingsDataSetup)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND_RANGE(IDS_COLORTABLE_START, IDS_COLORTABLE_END, OnSetColorTable)
	ON_COMMAND_RANGE(ID_CHART_XYCHART, ID_CHART_XYCHART, OnSelectChart)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CHART_XYCHART, ID_CHART_XYCHART, OnUpdateSelectChart)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPlotView construction/destruction

CCPlotView::CCPlotView()
{
	// TODO: add construction code here
	m_LastPoint.x = kNoUpdatePoint;
	m_CurDataSet = 1;
	graphtype = MainSystem::GetInstance()->viwflag - 2;
}

CCPlotView::~CCPlotView()
{
}

BOOL CCPlotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCPlotView drawing

void CCPlotView::OnDraw(CDC* pDC)
{
	CCPlotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect		clientRect, updateRect;
	CSize		ext, org;
	m_LastPoint.x = kNoUpdatePoint;

	GetClientRect((LPRECT)clientRect);
	pDC->DPtoLP((LPPOINT)&clientRect, 2);

	pDC->GetClipBox(updateRect);
	updateRect.NormalizeRect();
	switch (GetDocument()->m_WhichChart)
	{
		case ID_CHART_XYCHART: GetDocument()->xyChart.OnDraw(pDC, clientRect, updateRect); break;
		default: break;
	}
	if(	isDataEmpty ){
		RECT rc2;
		GetClientRect(&rc2);
		CClientDC dc(this);
		dc.Rectangle(rc2.left,rc2.top,rc2.right,rc2.bottom);
		dc.SetTextColor(RGB(255, 0, 0));
		dc.DrawText(CString("데이터가 없습니다."), &rc2, DT_VCENTER| DT_CENTER);
	}

}



/**************
* Purpose: 그래프를 위한 데이터 설정
**************/
void CCPlotView::setData(CCPlotDoc* pDoc, int type) {

#define		kDataSize		100

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	string sday = FilterInfo::GetInstance()->getSdate();// "2018-02-26";//날짜변경
	string eday = FilterInfo::GetInstance()->getEdate();//"2018-02-26";//날짜변경

	CAxis		*axis;
	double			data[2 * kDataSize];
	double *realdata;

	if (type == 1) {//최근100건전송간격
		realdata = MainSystem::GetInstance()->LastTransmissionInterval();
	}
	else {//24시간 메시지 건수
		  //realdata = MainSystem::GetInstance()->LastTransmissionInterval(eno);
		realdata = MainSystem::GetInstance()->FrequencyByHour();
	}

	int maxsize = 100;
	if (type != 1)maxsize = 24;
	int			dims[] = { 2, kDataSize };
	int			i;
	int			dataID;
	double sum = 0;
	for (i = 0; i < kDataSize; i++)
	{
		data[i] = i;
		if (i < maxsize) {
			data[kDataSize + i] = realdata[i];
			sum += realdata[i];
		}
		else
			data[kDataSize + i] = 0;
	}
	if (pDoc->xyChart.GetDataSetCount()>0 && sum>0 ) pDoc->xyChart.DeleteAllData();
	dataID = pDoc->xyChart.AddData(data, 2, dims);
	if (sum>0) {
		isDataEmpty = false;
		if (type == 1) {//최근100건전송간격
			pDoc->xyChart.SetTitle((CString)"최근 100건 송수신 시간 간격 ");
			axis = pDoc->xyChart.AddAxis(kLocationBottom);
			axis->SetTitle((CString)"발생시간");

			axis = pDoc->xyChart.AddAxis(kLocationLeft);
			axis->SetTitle((CString)"송수신간격(단위s)");
		}
		else {//24시간 메시지 건수

			pDoc->xyChart.SetTitle(((CString)sday.c_str() + "~" + (CString)eday.c_str() + (CString)" 시간대별 누적 수신 횟수 "));
			axis = pDoc->xyChart.AddAxis(kLocationBottom);
			axis->SetTitle((CString)"시간대(24시간표기)");

			axis = pDoc->xyChart.AddAxis(kLocationLeft);
			axis->SetTitle((CString)"횟수");
		}
	}
	else {
		isDataEmpty = true;

	}
		pDoc->xyChart.SetMarkerType(dataID, kXYMarkerNone);
		pDoc->xyChart.SetChartType(dataID, kXYChartConnect);


		pDoc->xyChart.SetMarkerType(dataID, kXYMarkerSquare);
		pDoc->xyChart.SetChartType(dataID, kXYChartConnect);
		pDoc->xyChart.SetDataColor(dataID, RGB(255, 0, 0));
		pDoc->xyChart.SetDataTitle(dataID, (CString)"Graph Data");


		pDoc->xyChart.SetMarkerType(dataID, kXYMarkerTriangle);
		pDoc->xyChart.SetChartType(dataID, kXYChartHistogram);
		pDoc->xyChart.SetDataColor(dataID, RGB(0, 0, 255));

		pDoc->xyChart.SetXRange(0, maxsize);
		pDoc->xyChart.m_UseMajorHorizontalGrids = TRUE;
		pDoc->xyChart.m_UseMajorVerticalGrids = TRUE;

		//	chart.m_UseMajorHorizontalGrids = TRUE;
		//	chart.m_UseMajorVerticalGrids = TRUE;
		//	chart.m_UseMinorHorizontalGrids = TRUE;
		//	chart.m_UseMinorVerticalGrids = TRUE;

		//axis = pDoc->xyChart.AddAxis( kLocationRight );
		//axis->SetTitle( "송수신간격" );
		//axis->SetRange( 0, 1 );

		//axis = pDoc->xyChart.AddAxis( kLocationTop );
		//axis->SetTitle( "Top axis" );

		//pDoc->xyChart.SetYRange( 0, 1000000000 );

		// Some other settings to play with

		//	pDoc->xyChart.m_UseMinorHorizontalGrids = TRUE;
		//	pDoc->xyChart.m_UseMinorVerticalGrids = TRUE;

		/*	axis = chart.AddAxis( kLocationTop );
		axis->SetRange( 5, -5 );
		axis->SetTitle( "Top axis" );

		axis = chart.AddAxis( kLocationRight );
		axis->SetRange( 0, 10 );
		axis->SetTitle( "Right hand axis" );
		*/

}
void CCPlotView::OnDrawPrint(CDC *pDC, CRect boundingRect)
{
	// We'll inset the rect by a couple centimeters each side
	boundingRect.InflateRect(-100, 100);

	switch (GetDocument()->m_WhichChart)
	{
	case ID_CHART_XYCHART: GetDocument()->xyChart.OnDraw(pDC, boundingRect); break;
	default: break;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CCPlotView printing

BOOL CCPlotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CCPlotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCPlotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCPlotView diagnostics

#ifdef _DEBUG
void CCPlotView::AssertValid() const
{
	CView::AssertValid();
}

void CCPlotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCPlotDoc* CCPlotView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCPlotDoc)));
	return (CCPlotDoc*)m_pDocument;
}
#endif //_DEBUG

void CCPlotView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CRect		rect;
	CRect		thisRect;
	float		ratio;

	GetClientRect(thisRect);

	// Get the aspect ratio
	ratio = (float)thisRect.Width() / (float)thisRect.Height();

	rect = pInfo->m_rectDraw;

	if ((float)abs(rect.Width()) / (float)abs(rect.Height()) > ratio)
	{
		// chop off some width
		rect.right = rect.left + (int)(abs(rect.Height()) * ratio);
	}
	else
	{
		// chop some off height
		rect.bottom = rect.top - (int)(abs(rect.Width()) / ratio);
	}

	OnDrawPrint(pDC, rect);
}

void CCPlotView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CView::OnPrepareDC(pDC, pInfo);

	pDC->SetMapMode(MM_LOMETRIC);
}

/////////////////////////////////////////////////////////////////////////////
// CCPlotView message handlers

void CCPlotView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	setData(GetDocument(), graphtype);
}

void	CCPlotView::OnSetColorTable(UINT id)
{
	GetDocument()->imageChart.SetPaletteMenu(id);

	// Force an update
	Invalidate();
}

void	CCPlotView::OnSelectChart(UINT id)
{
	GetDocument()->m_WhichChart = id;

	Invalidate();
}

void	CCPlotView::OnUpdateSelectChart(CCmdUI *cmd)
{
	cmd->SetCheck((cmd->m_nID == GetDocument()->m_WhichChart) ? 1 : 0);
}


void CCPlotView::OnPlotsettingsPlotSetup()
{
	if (GetDocument()->m_WhichChart == ID_CHART_XYCHART)
	{
		if (GetDocument()->xyChart.PlotSettings())
			Invalidate(TRUE);
	}
	else
	{
		if (GetDocument()->imageChart.PlotSettings())
			Invalidate(TRUE);
	}
}

void CCPlotView::OnPlotsettingsAxisSetup()
{
	if (GetDocument()->m_WhichChart == ID_CHART_XYCHART)
	{
		if (GetDocument()->xyChart.AxisSettings())
			Invalidate(TRUE);
	}
	else
	{
		if (GetDocument()->imageChart.AxisSettings())
			Invalidate(TRUE);
	}
}

void CCPlotView::OnPlotsettingsDataSetup()
{
	if (GetDocument()->m_WhichChart != ID_CHART_XYCHART) return;

	if (GetDocument()->xyChart.DataSettings())
		Invalidate(TRUE);
}

void CCPlotView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint		temp(kNoUpdatePoint, 0);
	OnMouseMove(0, temp);

	ClientToScreen(&point);

	if (GetDocument()->m_WhichChart == ID_CHART_XYCHART)
	{
		if (GetDocument()->xyChart.ShowPopupMenu(this, nFlags, point)) Invalidate();
	}
	else
	{
		if (GetDocument()->imageChart.ShowPopupMenu(this, nFlags, point)) Invalidate();
	}
}

void CCPlotView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int		dataSetCount;

	if (GetDocument()->m_WhichChart == ID_CHART_XYCHART)
		dataSetCount = GetDocument()->xyChart.GetDataSetCount();
	else
		dataSetCount = GetDocument()->imageChart.GetDataSetCount();

	m_CurDataSet = (m_CurDataSet + 1) % dataSetCount + 1;

	// call it manually
	OnMouseMove(0, point);

	// Print the current coordinates
	// Uncomment if you want a dialog box to go up
	// showing results of coordinate conversions

}

void CCPlotView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame			*frame = (CMainFrame*)AfxGetMainWnd();

	if (frame)
	{
		CDC			*pDC = this->GetDC();
		int			oldMode, oldROP, i;
		double		nearestData[2];
		CString		string;
		CPen		pen, *pOldPen;
		CPoint		nearestPoint, otherPoint;
		CRect		arcRect;

		oldMode = pDC->SetMapMode(MM_LOMETRIC);

		if (point.x != kNoUpdatePoint)
		{
			pDC->DPtoLP(&point);

			if (GetDocument()->m_WhichChart == ID_CHART_XYCHART)
			{
				GetDocument()->xyChart.LPToNearest(pDC, &point, nearestData, m_CurDataSet);
				GetDocument()->xyChart.DataToLP(pDC, nearestData, &nearestPoint);
			}
			else
			{
				GetDocument()->imageChart.LPToNearest(pDC, &point, nearestData);
				GetDocument()->imageChart.DataToLP(pDC, nearestData, &nearestPoint);
			}


		}

		pen.CreatePen(PS_SOLID, 5, RGB(0, 255, 255));
		pOldPen = pDC->SelectObject(&pen);

		oldROP = pDC->SetROP2(R2_XORPEN);

		otherPoint = m_LastPoint;
		for (i = 0; i < 2; i++)
		{
			if (otherPoint.x != kNoUpdatePoint)
			{
				arcRect.SetRect(otherPoint.x - 25, otherPoint.y + 25, otherPoint.x + 25, otherPoint.y - 25);
				pDC->Arc(arcRect, otherPoint, otherPoint);
			}
			otherPoint = nearestPoint;
		}

		pDC->SelectObject(pOldPen);
		pDC->SetROP2(oldROP);
		m_LastPoint = nearestPoint;

		pDC->SetMapMode(oldMode);

	}

	CView::OnMouseMove(nFlags, point);
}
