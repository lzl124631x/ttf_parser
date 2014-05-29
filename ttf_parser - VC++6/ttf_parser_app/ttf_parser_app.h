// ttf_parser_app.h : main header file for the TTF_PARSER_APP application
//

#if !defined(AFX_TTF_PARSER_APP_H__8CDD5E2D_5603_4E09_98F5_F4ADC03579C5__INCLUDED_)
#define AFX_TTF_PARSER_APP_H__8CDD5E2D_5603_4E09_98F5_F4ADC03579C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// Cttf_parser_appApp:
// See ttf_parser_app.cpp for the implementation of this class
//

class Cttf_parser_appApp : public CWinApp
{
public:
	Cttf_parser_appApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cttf_parser_appApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
  
  // GDI+
  ULONG_PTR m_gdiplusToken;
  GdiplusStartupInput m_gdiplusStartupInput;
// Implementation

	//{{AFX_MSG(Cttf_parser_appApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTF_PARSER_APP_H__8CDD5E2D_5603_4E09_98F5_F4ADC03579C5__INCLUDED_)
