
#pragma once

// dateDialog dialog

class dateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(dateDialog)

public:
	CDateTimeCtrl            *pCtrl1, *pCtrl2;
	COleDateTime            m_time1, m_time2;
	dateDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~dateDialog();
	CString tmp_start=(CString)"", tmp_end= (CString)"";

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual int OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();

};
