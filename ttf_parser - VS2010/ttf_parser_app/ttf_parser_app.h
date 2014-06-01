
// ttf_parser_app.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
#Error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CTtfParserAppApp : public CWinApp
{
 public:
  CTtfParserAppApp();

// Overrides
 public:
  virtual BOOL InitInstance();
  // GDI+
  ULONG_PTR m_gdiplusToken;
  GdiplusStartupInput m_gdiplusStartupInput;
// Implementation

  DECLARE_MESSAGE_MAP()
};

extern CTtfParserAppApp theApp;