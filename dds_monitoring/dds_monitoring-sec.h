#include "tinyxml2.h"
// dds_monitoring.h : main header file for the dds_monitoring application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cdds_monitoringApp:
// See dds_monitoring.cpp for the implementation of this class
//

class Cdds_monitoringApp : public CWinAppEx
{
public:


	Cdds_monitoringApp();

//prevate
	/*
	tinyxml2::XMLDocument mXMLdoc;
	tinyxml2::XMLNode* mXMLnode;
	*/
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

//	Cdds_monitoringApp();
	CMultiDocTemplate* pDocTemplate;

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	static UINT spymain(LPVOID pParam);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};


extern Cdds_monitoringApp theApp;
