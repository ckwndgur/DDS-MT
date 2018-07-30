
// dds_monitoringView.h : interface of the Cdds_monitoringView class
//

#pragma once
#include <list>
#include "afxcmn.h"
//#include "MessageData.h"
//#include "EntitiesData.h"
#include "DataManager.h"
#include "mainSystem.h"
#include "RelationshipView.h"
#include "XLEzAutomation.h"


//수정_하연_2018-04-04
#define HOST 0
#define PROCESS 1
#define DOMAINPARTICIPANT 2
#define PUB 3
#define SUB 4
#define DATAWRITER 5
#define DATAREADER 6

using namespace std;

class Cdds_monitoringView : public CScrollView
{
protected: // create from serialization only
	Cdds_monitoringView();
	DECLARE_DYNCREATE(Cdds_monitoringView)

// Attributes
public:
	Cdds_monitoringDoc* GetDocument() const;
	bool bTopicFlag;
	bool viewflag;
	CListCtrl m_DDSList;
	bool bUpdateEntity;
	CSize m_Pagesize; //수정_하연_2018-04-27
	float m_Zoom; //수정_하연_2018-04-27
	bool bZoomflag;//수정_하연_2018-04-27
	string preRVTab = ""; //수정_중혁_2018.05.03
	
private:
	CString preTitle;

// Operations
public:
	void Draw_RectofEntity(list<position> lstPosition, int kind, CDC* pDC); //수정_차중혁_2018.03.08
	void setFocus(int kind, entity e); //수정_하연_2018-04-04	
	void Save_ExcelFile();
	void SaveBitmapToDirectFile(CDC* pDC, CRect BitmapSize, int BitCount, CString strFilePath, bool onlyWrite); //수정_차중혁_2018.04.09

	
private:
	void SetScrollView(CSize xy); //수정_차중혁_2018.03.08
	void MakeListView(); //수정_차중혁_2018.03.08
	void Draw_Relationshipview(string domain, CDC* pDC); //수정_차중혁_2018.03.08


// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	virtual BOOL PreTranslateMessage(MSG* pMsg); //수정_이하연_2018.02.27
// Implementation
public:
	virtual ~Cdds_monitoringView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLvClick(NMHDR *pNMHDR, LRESULT *pResult); //수정_이하연_2018.02.27
	afx_msg void OnZoomInClick(); //수정_하연_2018-04-27
	afx_msg void OnZoomOutClick();//수정_하연_2018-04-27
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CListCtrl m_DDSList1;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileSave();
};

#ifndef _DEBUG  // debug version in dds_monitoringView.cpp
inline Cdds_monitoringDoc* Cdds_monitoringView::GetDocument() const
   { return reinterpret_cast<Cdds_monitoringDoc*>(m_pDocument); }
#endif

