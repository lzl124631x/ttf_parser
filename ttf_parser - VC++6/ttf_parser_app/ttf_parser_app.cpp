// ttf_parser_app.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ttf_parser_app.h"
#include "ttf_parser_appDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cttf_parser_appApp

BEGIN_MESSAGE_MAP(Cttf_parser_appApp, CWinApp)
	//{{AFX_MSG_MAP(Cttf_parser_appApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cttf_parser_appApp construction

Cttf_parser_appApp::Cttf_parser_appApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only Cttf_parser_appApp object

Cttf_parser_appApp theApp;

/////////////////////////////////////////////////////////////////////////////
// Cttf_parser_appApp initialization

BOOL Cttf_parser_appApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
  // GDI+
  GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
  
	Cttf_parser_appDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
