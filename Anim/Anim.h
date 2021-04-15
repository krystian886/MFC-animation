
// Anim.h : main header file for the Anim application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAnimApp:
// See Anim.cpp for the implementation of this class
//

class CAnimApp : public CWinApp
{
public:
	CAnimApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAnimApp theApp;
