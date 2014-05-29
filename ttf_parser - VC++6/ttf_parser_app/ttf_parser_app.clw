; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=Cttf_parser_appDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ttf_parser_app.h"

ClassCount=3
Class1=Cttf_parser_appApp
Class2=Cttf_parser_appDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TTF_PARSER_APP_DIALOG
Resource4=IDM_MAIN_MENU

[CLS:Cttf_parser_appApp]
Type=0
HeaderFile=ttf_parser_app.h
ImplementationFile=ttf_parser_app.cpp
Filter=N
LastObject=Cttf_parser_appApp

[CLS:Cttf_parser_appDlg]
Type=0
HeaderFile=ttf_parser_appDlg.h
ImplementationFile=ttf_parser_appDlg.cpp
Filter=D
LastObject=Cttf_parser_appDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ttf_parser_appDlg.h
ImplementationFile=ttf_parser_appDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TTF_PARSER_APP_DIALOG]
Type=1
Class=Cttf_parser_appDlg
ControlCount=11
Control1=IDC_TEXT_FILE_NAME,static,1342308364
Control2=IDC_TEXT_CHAR,static,1342308352
Control3=IDC_EDIT_CHAR,edit,1350631552
Control4=IDC_VIEW,button,1342242816
Control5=IDC_CHECK_SHOW_POINT,button,1342242819
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_GLYPH_INDEX,edit,1350639744
Control8=IDC_SPIN_GLYPH_INDEX,msctls_updown32,1342177442
Control9=IDC_SLIDER_GLYPH_INDEX,msctls_trackbar32,1342242832
Control10=IDC_TEXT_GLYPH_INFO_TITLE,static,1342308352
Control11=IDC_TEXT_GLYPH_INFO,static,1342312448

[MNU:IDM_MAIN_MENU]
Type=1
Class=Cttf_parser_appDlg
Command1=IDM_FILE_OPEN
Command2=IDM_FILE_EXIT
Command3=IDM_TOOL_DUMPXML
CommandCount=3

