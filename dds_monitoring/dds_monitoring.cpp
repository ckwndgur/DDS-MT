
// dds_monitoring.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "dds_monitoring.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "dds_monitoringDoc.h"
#include "dds_monitoringView.h"
#include "MainSystem.h"
#include "CPlotDoc.h"
#include "CPlotView.h"
#include<thread>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//초기 실행해야 할 내용 
UINT Cdds_monitoringApp::spymain(LPVOID pParam)
{
	
	return 0;
}

// Cdds_monitoringApp

BEGIN_MESSAGE_MAP(Cdds_monitoringApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &Cdds_monitoringApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_SET_DOMAIN, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_SHOW_GRAPHVIEW, &Cdds_monitoringApp::OnShowGraph)
	ON_COMMAND(ID_SHOW_LISTVIEW, &Cdds_monitoringApp::OnShowList)
	ON_COMMAND(ID_SHOW_RELAVIEW, &Cdds_monitoringApp::OnShowRelv)
	ON_COMMAND(ID_GRAPH_DISPLAY, &Cdds_monitoringApp::OnGraphDisplay)
	ON_COMMAND(ID_SHOW_GRAPH2, &Cdds_monitoringApp::OnShowGraph2)
END_MESSAGE_MAP()



// Cdds_monitoringApp construction

Cdds_monitoringApp::Cdds_monitoringApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("dds_monitoring.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only Cdds_monitoringApp object

Cdds_monitoringApp theApp;


// Cdds_monitoringApp initialization

BOOL Cdds_monitoringApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	MainSystem::GetInstance()->getData();
	FilterInfo::GetInstance()->setDefault();
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

	//CMultiDocTemplate* pMultiDocTemplate;
	pMultiDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //수정_하연_2018-04-27
		RUNTIME_CLASS(Cdds_monitoringDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(Cdds_monitoringView));

	if (!pMultiDocTemplate)
		return FALSE;
	AddDocTemplate(pMultiDocTemplate);

	pMultiDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //수정_하연_2018-04-27
		RUNTIME_CLASS(CCPlotDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCPlotView));

	if (!pMultiDocTemplate)
		return FALSE;
	AddDocTemplate(pMultiDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Parse command line for standard shell commands, DDE, file open
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);


	//새로만들기 없애기
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();


	/*onRDMfromSPY((handle_t)NULL,(const unsigned char *)"11111111111111111111111111111104", (const unsigned char *)"22222222222222222222222222222204",
		(const unsigned char *)"33", (const unsigned char *)"44", 4, (const unsigned char *)"5", (const unsigned char *)"6", 7, 8, (const unsigned char *)"2017-11-15 04:25:25");
		*/

	AfxBeginThread(spymain,this); //초기에 실행해야 할 내용
	//thread* t = new thread(&spymain);



}
int Cdds_monitoringApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// Cdds_monitoringApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void Cdds_monitoringApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void Cdds_monitoringApp::OnSetDomain()
{
}


/**************수정 중혁 2018.03.26
* Purpose: Show GV when u click ribbon menu(Graphview1)
**************/
void Cdds_monitoringApp::OnShowGraph()
{
	
	MainSystem::GetInstance()->viwflag = 3;
	//수정_손민영_2018-02-26
	POSITION pos = GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate;

	for (int i = 0; i < 2; i++) {
		pTemplate = GetNextDocTemplate(pos);
	}
	bool flag = 1;
	CMFCTabCtrl *wndTab = UIManager::GetInstance()->wndTab;
	CDocument *pGraph1Document = UIManager::GetInstance()->pGraph1Document;
	CView *pGraph1View = UIManager::GetInstance()->pGraph1View;
	if (wndTab != NULL) {
		for (int i = 0; i < wndTab->GetTabsNum(); i++) {
			CString title;
			wndTab->GetTabLabel(i, title);
			if (title.Find(_T("GraphLast100")) >= 0) {
				wndTab->SetActiveTab(i);

				((CCPlotView*)pGraph1View)->setData((CCPlotDoc*)pGraph1Document,1);
				pGraph1View->Invalidate();
				flag = 0;
			}
		}
	}

	if (flag)//!Check_Document(pTemplate, MainSystem::GetInstance()->sel_domainid)) //수정 중혁 2018.03.26
	{
		UIManager::GetInstance()->pGraph1Document = (Cdds_monitoringDoc*)pTemplate->OpenDocumentFile(NULL);
		pGraph1Document = UIManager::GetInstance()->pGraph1Document;
		pGraph1Document->SetTitle(_T("GraphLast100") + MainSystem::GetInstance()->sel_domainid);
		POSITION pp = pGraph1Document->GetFirstViewPosition();
		UIManager::GetInstance()->pGraph1View = pGraph1Document->GetNextView(pp);

		if (wndTab == NULL) {
			CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
			const CObList &tabGroups = pFrame->GetMDITabGroups();
			UIManager::GetInstance()->wndTab = (CMFCTabCtrl*)tabGroups.GetHead();

		}
	}

	MainSystem::GetInstance()->getData();
	UIManager::GetInstance()->refreshUI();
}


/**************수정 중혁 2018.03.26
* Purpose: Show GV when u click ribbon menu(Graphview2)
**************/
void Cdds_monitoringApp::OnShowGraph2()
{
	// TODO: Add your command handler code here

	MainSystem::GetInstance()->viwflag = 4;
	//수정_손민영_2018-02-26
	POSITION pos = GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate;
	for (int i = 0; i < 2; i++) {
		pTemplate = GetNextDocTemplate(pos);
	}

	bool flag = 1;
	CMFCTabCtrl *wndTab = UIManager::GetInstance()->wndTab;
	CDocument *pGraph2Document = UIManager::GetInstance()->pGraph2Document;
	CView *pGraph2View = UIManager::GetInstance()->pGraph2View;
	if (wndTab != NULL) {
		for (int i = 0; i < wndTab->GetTabsNum(); i++) {
			CString title;
			wndTab->GetTabLabel(i, title);
			if (title.Find(_T("GraphTimeOfDay")) >= 0) {
				wndTab->SetActiveTab(i);
				((CCPlotView*)pGraph2View)->setData((CCPlotDoc*)pGraph2Document, 2);
				pGraph2View->Invalidate();
				flag = 0;
			}
		}
	}

	if (flag)//!Check_Document(pTemplate, MainSystem::GetInstance()->sel_domainid)) //수정 중혁 2018.03.26
	{
		UIManager::GetInstance()->pGraph2Document = (Cdds_monitoringDoc*)pTemplate->OpenDocumentFile(NULL);
		pGraph2Document = UIManager::GetInstance()->pGraph2Document;
		pGraph2Document->SetTitle(_T("GraphTimeOfDay")+MainSystem::GetInstance()->sel_domainid);
		POSITION pp = pGraph2Document->GetFirstViewPosition();
		UIManager::GetInstance()->pGraph2View = pGraph2Document->GetNextView(pp);
		if (wndTab == NULL) {

			CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
			const CObList &tabGroups = pFrame->GetMDITabGroups();
			UIManager::GetInstance()->wndTab = (CMFCTabCtrl*)tabGroups.GetHead();

		}
	}

	MainSystem::GetInstance()->getData();
	UIManager::GetInstance()->refreshUI();
}


/**************수정 중혁 2018.03.26
* Purpose: Show LV when u click ribbon menu(Listview)
**************/
void Cdds_monitoringApp::OnShowList()
{
	MainSystem::GetInstance()->viwflag = 1;
		POSITION pos = GetFirstDocTemplatePosition();

		CDocTemplate* pTemplate = GetNextDocTemplate(pos);

		bool flag = 1;
		CMFCTabCtrl *wndTab = UIManager::GetInstance()->wndTab;
		CDocument *pListDocument = UIManager::GetInstance()->pListDocument;
		CView *pListView = UIManager::GetInstance()->pListView;
		if (wndTab != NULL) {
			for (int i = 0; i < wndTab->GetTabsNum(); i++) {
				CString title;
				wndTab->GetTabLabel(i, title);
				if (title.Find(_T("List_")) >= 0) {
					wndTab->SetActiveTab(i);
					pListView->Invalidate();
					flag = 0;
				}
			}
		}

		if (flag)//!Check_Document(pTemplate, MainSystem::GetInstance()->sel_domainid)) //수정 중혁 2018.03.26
		{
			UIManager::GetInstance()->pListDocument = (Cdds_monitoringDoc*)pTemplate->OpenDocumentFile(NULL);
			pListDocument = UIManager::GetInstance()->pListDocument;
			pListDocument->SetTitle(_T("List_")+MainSystem::GetInstance()->sel_domainid);
			POSITION pp = pListDocument->GetFirstViewPosition();
			UIManager::GetInstance()->pListView = pListDocument->GetNextView(pp);
			if (wndTab == NULL) {

				CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
				const CObList &tabGroups = pFrame->GetMDITabGroups();
				UIManager::GetInstance()->wndTab = (CMFCTabCtrl*)tabGroups.GetHead();

			}
		}
		
		UIManager::GetInstance()->updateListenerEventUI();
		UIManager::GetInstance()->refreshUI();

}

/**************수정 중혁 2018.03.26
* Purpose: Show RV when u click ribbon menu(Relationship)
**************/
void Cdds_monitoringApp::OnShowRelv() 
{
	if (MainSystem::GetInstance()->bSelDomain)
	{
		MainSystem::GetInstance()->viwflag = 0;
		POSITION pos = GetFirstDocTemplatePosition();
		CDocTemplate* pTemplate = GetNextDocTemplate(pos);

		bool flag = 1;
		CMFCTabCtrl *wndTab = UIManager::GetInstance()->wndTab;
		CDocument *pRelvDocument = UIManager::GetInstance()->pRelvDocument;
		CView *pRelvView = UIManager::GetInstance()->pRelvView;
		if (wndTab != NULL) {
			for (int i = 0; i < wndTab->GetTabsNum(); i++) {
				CString title;
				wndTab->GetTabLabel(i, title);
				if (title.Find(_T("Relationship_")) >= 0) {
					wndTab->SetActiveTab(i);
					pRelvView->Invalidate();
					flag = 0;
				}
			}
		}

		if (flag)//!Check_Document(pTemplate, MainSystem::GetInstance()->sel_domainid))
		{
			UIManager::GetInstance()->pRelvDocument = (Cdds_monitoringDoc*)pTemplate->OpenDocumentFile(NULL);
			pRelvDocument = UIManager::GetInstance()->pRelvDocument;
			pRelvDocument->SetTitle(_T("Relationship_") + MainSystem::GetInstance()->sel_domainid);
			POSITION pp = pRelvDocument->GetFirstViewPosition();
			UIManager::GetInstance()->pRelvView = pRelvDocument->GetNextView(pp);
			if (wndTab == NULL) {

				CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
				const CObList &tabGroups = pFrame->GetMDITabGroups();
				UIManager::GetInstance()->wndTab = (CMFCTabCtrl*)tabGroups.GetHead();

			}
		}
	}
}

/**************수정 중혁 2018.03.26
* Purpose: Check whether document is or not
**************/
bool Cdds_monitoringApp::Check_Document(CDocTemplate *doctemplate, CString doctitle)
{
	POSITION posDoc = doctemplate->GetFirstDocPosition();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame();
	Cdds_monitoringDoc* pDDSDoc = (Cdds_monitoringDoc*)pFrame->GetActiveDocument();

	while (posDoc)
	{
		CDocument *pDoc = doctemplate->GetNextDoc(posDoc);
		POSITION posView = pDoc->GetFirstViewPosition();
		while (posView)
		{
			CView* pView = pDoc->GetNextView(posView);
			if (pDoc->GetTitle() == doctitle)
			{
				posSelDoc = posDoc;
				//pDDSDoc->SwitchToView(pView); -> 만드는 중
				return true;
			}
		}
	}
	
	return false;
}


/**************
* Purpose: Cdds_monitoringApp customization load/save methods
**************/
void Cdds_monitoringApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void Cdds_monitoringApp::LoadCustomState()
{
}

void Cdds_monitoringApp::SaveCustomState()
{
}

// Cdds_monitoringApp message handlers




void Cdds_monitoringApp::OnGraphDisplay()
{
}
