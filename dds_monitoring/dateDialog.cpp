// dateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "dds_monitoring.h"
#include "dateDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// dateDialog dialog

IMPLEMENT_DYNAMIC(dateDialog, CDialogEx)

dateDialog::dateDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

dateDialog::~dateDialog()
{
}

int dateDialog::OnInitDialog() {

	pCtrl1 = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1);
	pCtrl2 = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER2);
	string a = FilterInfo::GetInstance()->getSdate();
	string a1 = FilterInfo::GetInstance()->getEdate();
	CString b(a.c_str());
	CString b1(a1.c_str());

	CTime tm1(_wtoi(b.Left(4)), _wtoi(b.Mid(5, 2)), _wtoi(b.Mid(8, 2)), 0, 0, 0);
	CTime tm2(_wtoi(b1.Left(4)), _wtoi(b1.Mid(5, 2)), _wtoi(b1.Mid(8, 2)), 0, 0, 0);

	m_time1 = tm1.GetTime();
	m_time2 = tm2.GetTime();

	VERIFY(pCtrl1->SetTime(m_time1));
	VERIFY(pCtrl2->SetTime(m_time2));
	return 0;
}
void dateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, *pCtrl1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, *pCtrl2);
}


BEGIN_MESSAGE_MAP(dateDialog, CDialogEx)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &dateDialog::OnDtnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &dateDialog::OnDtnDatetimechangeDatetimepicker2)
	ON_BN_CLICKED(IDC_BUTTON1, &dateDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// dateDialog message handlers


void dateDialog::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	tmp_start.Format(_T("YYYY-MM-DD %04d-%02d-%02d"), pDTChange->st.wYear, pDTChange->st.wMonth, pDTChange->st.wDay);

}


void dateDialog::OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	tmp_end.Format(_T("YYYY-MM-DD %04d-%02d-%02d"), pDTChange->st.wYear, pDTChange->st.wMonth, pDTChange->st.wDay);
}


void dateDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	VERIFY(pCtrl1->GetTime(m_time1));
	VERIFY(pCtrl2->GetTime(m_time2));
	tmp_start=m_time1.Format(_T("%Y-%m-%d"));
	tmp_end = m_time2.Format(_T("%Y-%m-%d"));

	FilterInfo::GetInstance()->changeDate(std::string(CT2CA(tmp_start.operator LPCWSTR())), std::string(CT2CA(tmp_end.operator LPCWSTR())));

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->changeButtonText();

	MainSystem::GetInstance()->getData();
	UIManager::GetInstance()->refreshUI();
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}