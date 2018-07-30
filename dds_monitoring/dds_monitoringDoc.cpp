
// dds_monitoringDoc.cpp : implementation of the Cdds_monitoringDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "dds_monitoring.h"
#endif

#include "dds_monitoringDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "dds_monitoringView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cdds_monitoringDoc

IMPLEMENT_DYNCREATE(Cdds_monitoringDoc, CDocument)

BEGIN_MESSAGE_MAP(Cdds_monitoringDoc, CDocument)
END_MESSAGE_MAP()


// Cdds_monitoringDoc construction/destruction

Cdds_monitoringDoc::Cdds_monitoringDoc()
{
	// TODO: add one-time construction code here

}

Cdds_monitoringDoc::~Cdds_monitoringDoc()
{
}

BOOL Cdds_monitoringDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/**************
* Purpose: Switch one tab to another tab (not complete
**************/
CView* Cdds_monitoringDoc::SwitchToView(CView* pNewView)
{
	CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();

	// Get the active MDI child window.
	CMDIChildWnd* pChild = (CMDIChildWnd*)pMainWnd->MDIGetActive();

	// Get the active view attached to the active MDI child window.
	CView* pOldActiveView = pChild->GetActiveView();

	// Set flag so that document will not be deleted when view is dettached.
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;

	// Dettach existing view
	RemoveView(pOldActiveView);

	// restore flag
	m_bAutoDelete = bAutoDelete;

	// Show the newly active view and hide the inactive view.
	pNewView->ShowWindow(SW_SHOW);
	pOldActiveView->ShowWindow(SW_HIDE);

	// Attach new view
	AddView(pNewView);

	pChild->RecalcLayout();
	pNewView->UpdateWindow();
	pChild->SetActiveView(pNewView);
	return pOldActiveView;
}

// Cdds_monitoringDoc serialization

void Cdds_monitoringDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void Cdds_monitoringDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void Cdds_monitoringDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Cdds_monitoringDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Cdds_monitoringDoc diagnostics

#ifdef _DEBUG
void Cdds_monitoringDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cdds_monitoringDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cdds_monitoringDoc commands
