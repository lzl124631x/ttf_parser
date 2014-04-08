
// ttf_parser_app.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cttf_parser_appApp:
// See ttf_parser_app.cpp for the implementation of this class
//

class Cttf_parser_appApp : public CWinApp
{
public:
	Cttf_parser_appApp();

// Overrides
public:
	virtual BOOL InitInstance();
	// GDI+
	ULONG_PTR m_gdiplusToken;
	GdiplusStartupInput m_gdiplusStartupInput;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cttf_parser_appApp theApp;