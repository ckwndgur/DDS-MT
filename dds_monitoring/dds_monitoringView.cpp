
// dds_monitoringView.cpp : implementation of the Cdds_monitoringView class
//

#include "stdafx.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "dds_monitoring.h"
#endif

#include "dds_monitoringDoc.h"
#include "dds_monitoringView.h"
#include "stdlib.h"
#include "string.h"
#include <algorithm> //수정_하연_2018-05-01

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cdds_monitoringView

IMPLEMENT_DYNCREATE(Cdds_monitoringView, CScrollView)

BEGIN_MESSAGE_MAP(Cdds_monitoringView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cdds_monitoringView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	//수정_이하연_2018.02.27
	ON_NOTIFY(NM_CLICK, 700, &Cdds_monitoringView::OnLvClick)
	ON_COMMAND(ID_FILE_SAVE, &Cdds_monitoringView::OnFileSave)
	ON_COMMAND(IDC_ZOOMIN_BTN, &Cdds_monitoringView::OnZoomInClick)
	ON_COMMAND(IDC_ZOOMOUT_BTN, &Cdds_monitoringView::OnZoomOutClick)
END_MESSAGE_MAP()

// Cdds_monitoringView construction/destruction


void Cdds_monitoringView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	viewflag = MainSystem::GetInstance()->viwflag;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

Cdds_monitoringView::Cdds_monitoringView()
{
	// TODO: add construction code here
	
	CSize sizeTotal;
	int width, height;

	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_Zoom = 1.f;  //수정_하연_2018-04-27

	bUpdateEntity = false;
	viewflag = 0;
}

//수정_이하연_2018.02.27
BOOL Cdds_monitoringView::PreTranslateMessage(MSG * pMsg)
{
	
	return CScrollView::PreTranslateMessage(pMsg);
}


void createSimpleDoc(const char* filename) {
}

Cdds_monitoringView::~Cdds_monitoringView()
{
}

BOOL Cdds_monitoringView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	bTopicFlag = false;
	return CScrollView::PreCreateWindow(cs);
}

/**************수정 중혁 2018.03.26
* Purpose: Cdds_monitoringView drawing
**************/
void Cdds_monitoringView::OnDraw(CDC* pDC)
{
	Cdds_monitoringDoc* pDoc = GetDocument();
	
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	if (viewflag) {
		
		if (bUpdateEntity && MainSystem::GetInstance()->bSelDomain) //수정 차중혁 2018.03.29
		{
			//수정_손민영_02_28
			MakeListView();
			bUpdateEntity = false;
		}

	}
	else {
		//if (IsWindow(m_DDSList))
		//	m_DDSList.DeleteAllItems();

		//수정 차중혁 2018.03.26
		if (MainSystem::GetInstance()->sel_domainid.GetLength() > 0)
		{
			CString str;
			if (pDoc->GetTitle() == "DomainList")
			{
				str = preTitle;
			}
			else
			{
				str = pDoc->GetTitle();
				AfxExtractSubString(str, str, 1, '_');
				preTitle = str;
			}
			
			Draw_Relationshipview(string(CT2CA(str.operator LPCWSTR())), pDC);
		}
	}

	SetScrollView(m_Pagesize); //수정_하연_2018-04-27
	// TODO: add draw code for native data here
}


/**************수정 중혁 2018.05.02
* Purpose: Draw RV in one domain
**************/
void Cdds_monitoringView::Draw_Relationshipview(string domain, CDC* pDC)
{
	//CButton *datebutton = new CButton();
	//datebutton->Create(_T("ddd"), WS_CHILD | WS_VISIBLE |
	//	BS_PUSHBUTTON, CRect(0, 0, 10, 10), 	//수정_하연_2018-04-04
	//	this, 100);

	MainSystem * m = MainSystem::GetInstance();
	RelationshipView *R = RelationshipView::GetInstance();
	CSize sizeMax = { 0,0 }; //수정_하연_2018-05-04

	if (preRVTab.compare(domain)) 
	{
		R->Cal_AllPosition(domain);
		preRVTab = domain;
	}
		
	if (R->lstHostPosition.size() > 0)
	{
		list<position>::iterator iter = --R->lstHostPosition.end();

		for (size_t i = 0; i < TOPIC + 1; i++)
		{
			switch (i)
			{
			case HOST:
				Draw_RectofEntity(R->lstHostPosition, HOST, pDC);
				//SetScrollView(iter->y);
				break;
			case PROCESS:
				Draw_RectofEntity(R->lstProcessPosition, PROCESS, pDC);
				break;
			case DOMAINPARTICIPANT:
				Draw_RectofEntity(R->lstDPPosition, DOMAINPARTICIPANT, pDC);
				break;
			case PUB:
				Draw_RectofEntity(R->lstPubPosition, PUB, pDC);
				break;
			case SUB:
				Draw_RectofEntity(R->lstSubPosition, SUB, pDC);
				break;
			case DATAWRITER:
				Draw_RectofEntity(R->lstDWPosition, DATAWRITER, pDC);
				break;
			case DATAREADER:
				Draw_RectofEntity(R->lstDRPosition, DATAREADER, pDC);
				break;
			case TOPIC:
				Draw_RectofEntity(R->lstTopicPosition, TOPIC, pDC);
				break;
			default:
				break;
			}
		}
	}

	if (R->lstHostPosition.size() > 0) { //수정_하연_2018-05-04
		for (list<position>::iterator i = R->lstHostPosition.begin(); i != R->lstHostPosition.end(); i++) {

			if (sizeMax.cx < i->y.cx)
				sizeMax.cx = i->y.cx;

			if (sizeMax.cy < i->y.cy)
				sizeMax.cy = i->y.cy;
		}
	}

	m_Pagesize = CSize(sizeMax.cx * m_Zoom, sizeMax.cy * m_Zoom); //수정_하연_2018-05-04
}



/**************수정 중혁 2018.05.02
* Purpose: Draw rectangle & text in one Entity
**************/
void Cdds_monitoringView::Draw_RectofEntity(list<position> lstPosition, int kind, CDC* pDC)
{
	RelationshipView *R = RelationshipView::GetInstance();

	for (list<position>::iterator iter = lstPosition.begin(); iter != lstPosition.end(); iter++)
	{
		//draw rect
		CPen pen;
		CPen* oldPen;
		CString textout;

		switch (kind)
		{
		case HOST:
			pen.CreatePen(PS_ALL, 4, RGB(114, 53, 162));
			oldPen = pDC->SelectObject(&pen);
			textout = "H: ";
			break;
		case PROCESS:
			pen.CreatePen(PS_ALL, 3.5, RGB(6, 177, 83));
			oldPen = pDC->SelectObject(&pen);
			textout = "P: ";
			break;
		case DOMAINPARTICIPANT:
			pen.CreatePen(PS_ALL, 3, RGB(42, 65, 107));
			oldPen = pDC->SelectObject(&pen);
			textout = "DP: ";
			break;
		case PUB:
			pen.CreatePen(PS_ALL, 2, RGB(12, 133, 176));
			oldPen = pDC->SelectObject(&pen);
			textout = "PUB: ";
			break;
		case SUB:
			pen.CreatePen(PS_ALL, 2, RGB(255, 73, 165));
			oldPen = pDC->SelectObject(&pen);
			textout = "SUB: ";
			break;
		case DATAWRITER:
			pen.CreatePen(PS_ALL, 1, RGB(10, 178, 242));
			oldPen = pDC->SelectObject(&pen);
			textout = "DW: ";
			break;
		case DATAREADER:
			pen.CreatePen(PS_ALL, 1, RGB(255, 109, 128));
			oldPen = pDC->SelectObject(&pen);
			textout = "DR: ";
			break;
		case TOPIC:
			pen.CreatePen(PS_ALL, 1, RGB(255, 109, 11));
			oldPen = pDC->SelectObject(&pen);
			textout = "T: ";
			break;
		default:
			break;
		}

		if (iter->bclick)
		{
			CBrush myBrush;
			myBrush.CreateSolidBrush(RGB(255, 128, 0));
			CBrush* oldBrush = pDC->SelectObject(&myBrush);
			pDC->Rectangle(iter->x.cx * m_Zoom, iter->x.cy * m_Zoom, iter->y.cx * m_Zoom, iter->y.cy * m_Zoom); //수정_하연_2018-05-01
			pDC->SelectObject(oldBrush);
			oldBrush->DeleteObject();
			
		}
		else
		{
			pDC->Rectangle(iter->x.cx * m_Zoom, iter->x.cy * m_Zoom, iter->y.cx * m_Zoom, iter->y.cy * m_Zoom); //수정_하연_2018-05-01
		}


		//Naming
		CFont font, *pOldFont;
		font.CreatePointFont(80, _T("Lucida Grande Bold"));
		pOldFont = (CFont*)pDC->SelectObject(&font);

		int xtext;
		int ytext;
		textout += iter->name.c_str();
		if (kind == DATAWRITER || kind == DATAREADER)
		{
			xtext = ceil(iter->x.cx * m_Zoom + 5);//수정_하연_2018-05-01
			ytext = ceil(iter->x.cy * m_Zoom + 5);//수정_하연_2018-05-01
			pDC->TextOutW(xtext, ytext, textout);
		}
		else
		{
			if (textout.GetLength() > 8 && kind == TOPIC)
			{
				textout = textout.Left(8);
				textout.Append(_T(".."));
				xtext = ceil((iter->y.cx * m_Zoom + iter->x.cx * m_Zoom) / 2) - textout.GetLength() * 3;//수정_하연_2018-05-01
				ytext = ceil(iter->x.cy * m_Zoom + 5);//수정_하연_2018-05-01
			}
			else
			{
				xtext = ceil((iter->y.cx * m_Zoom + iter->x.cx * m_Zoom) / 2) - textout.GetLength() * 3;//수정_하연_2018-05-01
				ytext = ceil(iter->x.cy * m_Zoom + 5);//수정_하연_2018-05-01
			}

			pDC->TextOutW(xtext, ytext, textout);
		}
	}

}

void Cdds_monitoringView::OnZoomInClick() //수정_하연_2018-04-27
{
	RelationshipView *R = RelationshipView::GetInstance();

	CSize sizeZoom = (0, 0);

	if (R->lstHostPosition.size() > 0) {
		for (list<position>::iterator iter = R->lstHostPosition.begin(); iter != R->lstHostPosition.end(); iter++) {

			if (sizeZoom.cx < iter->y.cx)
				sizeZoom.cx = iter->y.cx;

			if (sizeZoom.cy < iter->y.cy)
				sizeZoom.cy = iter->y.cx;
		}
	}

	m_Zoom += 0.1f;

	//CClientDC dc(this);
	m_Pagesize = CSize(sizeZoom.cx * m_Zoom, sizeZoom.cy * m_Zoom);

	Invalidate();
}

void Cdds_monitoringView::OnZoomOutClick()//수정_하연_2018-04-27
{
	RelationshipView *R = RelationshipView::GetInstance();

	CSize sizeZoom = (0, 0);

	if (R->lstHostPosition.size() > 0) {
		for (list<position>::iterator iter = R->lstHostPosition.begin(); iter != R->lstHostPosition.end(); iter++) {

			if (sizeZoom.cx < iter->y.cx)
				sizeZoom.cx = iter->y.cx;

			if (sizeZoom.cy < iter->y.cy)
				sizeZoom.cy = iter->y.cx;
		}
	}

	m_Zoom -= 0.1f;

	//CClientDC dc(this);
	m_Pagesize = CSize(sizeZoom.cx * m_Zoom, sizeZoom.cy * m_Zoom);

	Invalidate();

}

/**************
* Purpose: Save RV, GV as bmp extension
**************/
void Cdds_monitoringView::SaveBitmapToDirectFile(CDC* pDC, CRect BitmapSize, int BitCount, CString strFilePath, bool onlyWrite)
{
	CBitmap bmp, *pOldBmp;
	CDC dcMem;
	BITMAP                  bm;
	BITMAPINFOHEADER        bi;
	LPBITMAPINFOHEADER      lpbi;
	DWORD                   dwLen;
	HANDLE                  handle;
	HANDLE                  hDIB;
	HPALETTE                hPal = NULL;
	/*----- CDC의 내용을 Bitmap으로 전송 ----*/
	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, BitmapSize.Width(), BitmapSize.Height());
	pOldBmp = (CBitmap*)dcMem.SelectObject(&bmp);
	dcMem.BitBlt(0, 0, BitmapSize.Width(), BitmapSize.Height(), pDC, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	if (strFilePath == "")          return;
	/*------------------------- 비트멥 헤더를 기록함 -------------------------*/
	if (hPal == NULL)
		hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	GetObject(HBITMAP(bmp), sizeof(BITMAP), &bm);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = bm.bmWidth * bm.bmHeight * 3;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	int nColors = (1 << bi.biBitCount);
	if (nColors > 256)
		nColors = 0;
	dwLen = bi.biSize + nColors * sizeof(RGBQUAD);
	hPal = SelectPalette(pDC->GetSafeHdc(), hPal, FALSE);
	RealizePalette(pDC->GetSafeHdc());
	hDIB = GlobalAlloc(GMEM_FIXED, dwLen);
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	*lpbi = bi;
	GetDIBits(pDC->GetSafeHdc(),
		HBITMAP(bmp),
		0,
		(DWORD)bi.biHeight,
		(LPBYTE)NULL,
		(LPBITMAPINFO)lpbi,
		(DWORD)DIB_RGB_COLORS);
	bi = *lpbi;
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
			* bi.biHeight;
	}
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	GetDIBits(pDC->GetSafeHdc(),
		HBITMAP(bmp),
		0,
		(DWORD)bi.biHeight,
		(LPBYTE)lpbi
		+ (bi.biSize + nColors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpbi,
		(DWORD)DIB_RGB_COLORS);
	BITMAPFILEHEADER      hdr;
	hdr.bfType = ((WORD)('M' << 8) | 'B');
	hdr.bfSize = GlobalSize(hDIB) + sizeof(hdr);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD)(sizeof(hdr) + lpbi->biSize + nColors * sizeof(RGBQUAD));
	char* pBmpBuf;
	DWORD FileSize;
	FileSize = sizeof(hdr) + GlobalSize(hDIB);
	pBmpBuf = new char[FileSize];
	memcpy(pBmpBuf, &hdr, sizeof(hdr));
	memcpy(pBmpBuf + sizeof(hdr), lpbi, GlobalSize(hDIB));
	/*--------------------- 실제 파일에 기록함 --------------------------*/
	if (onlyWrite)
	{
		CFile file;
		file.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		file.Write(pBmpBuf, FileSize);
		file.Close();
	}
	else
	{
		CFile file;
		file.Open(strFilePath, CFile::modeWrite);
		file.Write(pBmpBuf, FileSize);
		file.Close();
	}

	/*------------------------ 임시로 할당한 메모리를 해제.. -------------*/
	delete[] pBmpBuf;
	if (hDIB)
	{
		GlobalFree(hDIB);
	}
	SelectPalette(pDC->GetSafeHdc(), hPal, FALSE);
}

/**************
* Purpose: Set focus selected entity of treectrl
**************/

//수정_하연_2018-04-04
void Cdds_monitoringView::setFocus(int kind, entity e)
{
	string entity_str, buf_entity_str;
	CString entity_cstr;

	m_DDSList.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_DDSList.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_DDSList.SetFocus();

	switch (kind) {
	case HOST:
		entity_str = "Host:" + e.host;
		break;
	case PROCESS:
		entity_str = "Host:" + e.host + ">" + "Process:" + e.process;
		break;
	case DOMAINPARTICIPANT:
		entity_str = "Host:" + e.host + ">" + "Process:" + e.process + ">" + "DP:" + e.domainParticipant;
		break;
	case SUB:
		entity_str = "Host:" + e.host + ">" + "Process:" + e.process + ">" + "DP:" + e.domainParticipant + ">" + "Sub:" + e.subscriber;
		break;
	case DATAREADER:
		entity_str = "Host:" + e.host + ">" + "Process:" + e.process + ">" + "DP:" + e.domainParticipant + ">" + "Sub:" + e.subscriber + ">" + "DR:" + e.dataReader;
		break;
	}

	entity_cstr = entity_str.c_str();
	int cnt = m_DDSList.GetItemCount();

	if (entity_str != buf_entity_str) {
		for (int i = 0; i < cnt; i++) {
			if (m_DDSList.GetItemText(i, 0).Find(entity_cstr) != -1) {
				m_DDSList.SetSelectionMark(i);
				m_DDSList.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				m_DDSList.SetFocus();
			}
		}
	}


	buf_entity_str = entity_str;
	entity_str = "";
}

/**************수정 차중혁 2018.03.14
* Purpose: Set scroll size
**************/
void Cdds_monitoringView::SetScrollView(CSize xy)
{
	if ((xy.cx == 0) && (xy.cy == 0))
	{
		CRect rc;
		GetClientRect(&rc);
		CSize sizeViewPage;
		sizeViewPage.cx = rc.right - rc.left;
		sizeViewPage.cy = rc.bottom - rc.top;
		SetScrollSizes(MM_TEXT, sizeViewPage);
	}
	else
	{
		CRect rc;
		GetClientRect(&rc);
		CSize sizeViewPage;
		int check_x = rc.right - rc.left;
		
		if (xy.cx < check_x)
		{
			sizeViewPage.cx = check_x;
		}
		else
		{
			sizeViewPage.cx = xy.cx;
		}

		int check_y = rc.bottom - rc.top;
		if (xy.cy < check_y)
		{
			sizeViewPage.cy = check_y;
		}
		else
		{
			sizeViewPage.cy = xy.cy;
		}

		SetScrollSizes(MM_TEXT, sizeViewPage);
	}

}

/**************
* Purpose: Cdds_monitoringView printing
**************/
void Cdds_monitoringView::MakeListView()
{//수정0208
 //CSize sizeTotal;
 //sizeTotal.cx = sizeTotal.cy = 100;

 //수정_손민영_02_28
	if (IsWindow(m_DDSList)) {
		//m_DDSList.DeleteAllItems();
	}
	else {
		CRect crect;
		GetWindowRect(crect);
		CSize siz;

		siz.cx = crect.Width();
		siz.cy = crect.Height();
		//m_DDSList.RemoveAllGroups();
		m_DDSList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, CRect(0, 0, siz.cx, siz.cy - 10), this, 700);
		m_DDSList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		m_DDSList.ShowScrollBar(SB_BOTH, TRUE);

		if (m_DDSList.GetColumnWidth(0) == 0) //수정 차중혁 2018.03.29
		{
			m_DDSList.InsertColumn(0, _T("Entity"), LVCFMT_LEFT, 450);
			m_DDSList.InsertColumn(1, _T("State"), LVCFMT_LEFT, 50);
			m_DDSList.InsertColumn(2, _T("Status"), LVCFMT_LEFT, 50);
			m_DDSList.InsertColumn(3, _T("DR's TimeStamp"), LVCFMT_LEFT, 150);
			m_DDSList.InsertColumn(4, _T("DW's TimeStamp"), LVCFMT_LEFT, 150);
			m_DDSList.InsertColumn(5, _T("Value"), LVCFMT_LEFT, 400);
		}

		//lstMessagedata.sort();

		//수정 차중혁 2018.03.29
		int datasize = MainSystem::GetInstance()->currentDataSet.size();
		for (size_t i = 0; i < datasize; i++)
		{
			listener_event mMessageData = MainSystem::GetInstance()->currentDataSet.find(datasize - i)->second;
			int lineno = 0;
			if (FilterInfo::GetInstance()->checkViewOption(mMessageData)) {
				entity mEntitiesdata = MainSystem::GetInstance()->currentEntitySet.find(mMessageData.id)->second;

				if (mEntitiesdata.domainId.compare(string(CT2CA(MainSystem::GetInstance()->sel_domainid.operator LPCWSTR()))) == 0)
				{
					CString listitem_entity;
					string listitem_entity_buf;

					listitem_entity_buf = "Host:";

					if (mEntitiesdata.publisher.length() > 0)
						listitem_entity_buf = "Host:" + mEntitiesdata.host + ">" + "Process:" + mEntitiesdata.process + ">" + "DP:" + mEntitiesdata.domainParticipant + ">" + "Pub:" + mEntitiesdata.publisher + ">" + "DW:" + mEntitiesdata.dataWriter;
					else
						listitem_entity_buf = "Host:" + mEntitiesdata.host + ">" + "Process:" + mEntitiesdata.process + ">" + "DP:" + mEntitiesdata.domainParticipant + ">" + "Sub:" + mEntitiesdata.subscriber + ">" + "DR:" + mEntitiesdata.dataReader;

					m_DDSList.InsertItem(lineno, _T(""));

					listitem_entity = listitem_entity_buf.c_str();
					m_DDSList.SetItemText(lineno, 0, listitem_entity);
					listitem_entity = mMessageData.state.c_str();
					m_DDSList.SetItemText(lineno, 1, listitem_entity);
					listitem_entity = mMessageData.status.c_str();
					m_DDSList.SetItemText(lineno, 2, listitem_entity);
					listitem_entity = mMessageData.datetime.c_str();
					m_DDSList.SetItemText(lineno, 3, listitem_entity);
					listitem_entity = mMessageData.datetime2.c_str();
					m_DDSList.SetItemText(lineno, 4, listitem_entity);
					listitem_entity = mMessageData.value.c_str();
					m_DDSList.SetItemText(lineno, 5, listitem_entity);

					lineno++;
				}
			}
		}
	}
}


void Cdds_monitoringView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cdds_monitoringView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cdds_monitoringView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cdds_monitoringView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cdds_monitoringView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cdds_monitoringView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cdds_monitoringView diagnostics

#ifdef _DEBUG
void Cdds_monitoringView::AssertValid() const
{
	CView::AssertValid();
}

void Cdds_monitoringView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cdds_monitoringDoc* Cdds_monitoringView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cdds_monitoringDoc)));
	return (Cdds_monitoringDoc*)m_pDocument;
}
#endif //_DEBUG


// Cdds_monitoringView message handlers


void Cdds_monitoringView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (IsWindow(m_DDSList.m_hWnd)) {
		m_DDSList.MoveWindow(0, 0, cx, cy);
	}
}



/**************수정_이하연_2018.02.27
* Purpose: When u click LV item, display entity info on PV
**************/
void Cdds_monitoringView::OnLvClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CPropertiesWnd *pProWnd = (CPropertiesWnd*)pFrame->GetPropertyViewPT();
	CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame();

	CString val[5];

	POSITION pos = m_DDSList.GetFirstSelectedItemPosition();
	int iIndex = m_DDSList.GetNextItem(-1, LVNI_SELECTED);

	for (int i = 0; i < 5; i++) {
		val[i] = m_DDSList.GetItemText(iIndex, i+1);
	}


	pProWnd->entity_state = val[0];
	pProWnd->entity_status = val[1];
	pProWnd->entity_time1 = val[2];
	pProWnd->entity_time2 = val[3];
	pProWnd->ValueFromView(val[4]);

	pProWnd->bSetValueProp = true;
	pProWnd->Invalidate();

	*pResult = 0;
}

void Cdds_monitoringView::Save_ExcelFile()
{
	int m_iMax;
	int nColumncount = m_DDSList.GetItemCount();
	m_iMax = nColumncount;
	int i, j;

	int columnNum = 0;
	CString m_strFileName;

	CXLEzAutomation XL(FALSE);

	m_strFileName = "리스트"; //파일 저장할 때 초기에 나타나는 파일명. 변경가능

	XL.SetCellValue(++columnNum, 1, _T("Entity")); //엑셀에 뿌려질 각 칼럼명들
	XL.SetCellValue(++columnNum, 1, _T("State"));
	XL.SetCellValue(++columnNum, 1, _T("Status"));
	XL.SetCellValue(++columnNum, 1, _T("DR's TimeStamp"));
	XL.SetCellValue(++columnNum, 1, _T("DW's TimeStamp"));
	XL.SetCellValue(++columnNum, 1, _T("Value"));

	for (i = 1; i <= m_iMax; i++)
	{
		XL.SetCellValue(1, i + 1, m_DDSList.GetItemText(i - 1, 0)); //먼저 이름 칼럼을 뿌려줌

		for (j = 1; j <= m_iMax; j++)
			XL.SetCellValue(j + 1, i + 1, m_DDSList.GetItemText(i - 1, j)); //그 다음에 성별과 나이 뿌림
	}

	CFileDialog dlg(FALSE, _T("xlsx"), m_strFileName + _T(".xlsx"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_NOCHANGEDIR, _T("xlsx 파일 (*.xlsx)|*.xlsx|"), NULL);

	if (dlg.DoModal() == IDOK)
		XL.SaveFileAs(dlg.GetPathName());

	// XL.ReleaseExcel();

}

/**************
* Purpose: Cdds_monitoringView save as file(xlsx, bmp)
**************/
void Cdds_monitoringView::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	Cdds_monitoringDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	AfxExtractSubString(str, str, 0, '_');

	if (str == "List")
	{
		Save_ExcelFile();
	}
	else
	{
		//CString filter = _T("PDF files (*.pdf) | *.pdf");
		//CFileDialog dlgFile(FALSE, _T("pdf"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, filter);
		CString filter = _T("BMP files (*.bmp) | *.bmp");
		CFileDialog dlgFile(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, filter);
		CString fileName;

		if (dlgFile.DoModal() == IDOK)
		{
			fileName = dlgFile.GetFolderPath() + _T("\\") + dlgFile.GetFileName();

			CRect rc;
			CRect real_rc;
			CDC* dc = GetDC();
			GetClientRect(&rc);

			RelationshipView *R = RelationshipView::GetInstance();
			list<position>::iterator iter = --R->lstHostPosition.end();
			real_rc.SetRect(iter->x.cx, iter->x.cy, iter->y.cx, iter->y.cy);

			bool check_R = rc.right > real_rc.right;
			bool check_B = rc.bottom > real_rc.bottom;

			SaveBitmapToDirectFile(dc, rc, 256, fileName, 1);

			//if (check_R && check_B)
			//{
			//	SaveBitmapToDirectFile(dc, rc, 256, fileName, 1);
			//}
			//else
			//{
			//	int a = (int)(rc.Width() / real_rc.Width());
			//	int b = (int)(rc.Height() / real_rc.Height());
			//	int pos_H = GetScrollPos(SB_HORZ);
			//	int pos_V = GetScrollPos(SB_VERT);
			//	CRect buf_rc;

			//	for (int i = 1; i < a+2; i++)
			//	{
			//		SetScrollPos(SB_HORZ, pos_H+rc.right, 1);
			//		//rc.SetRect()
			//	}
			//}
		}
	}
}