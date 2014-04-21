
// ttf_parser_appDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ttf_parser_app.h"
#include "ttf_parser_appDlg.h"
#include "ttf_parser_gdiplus.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD){
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cttf_parser_appDlg dialog
Cttf_parser_appDlg::Cttf_parser_appDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cttf_parser_appDlg::IDD, pParent){
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cttf_parser_appDlg::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NAME, m_fileNameText);
	DDX_Control(pDX, IDC_CHAR, m_char);
	DDX_Control(pDX, IDC_VIEW, m_btn_view);
}

BEGIN_MESSAGE_MAP(Cttf_parser_appDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_FILE_OPEN, &Cttf_parser_appDlg::OnFileOpen)
	ON_COMMAND(IDM_FILE_EXIT, &Cttf_parser_appDlg::OnFileExit)
	ON_BN_CLICKED(IDC_VIEW, &Cttf_parser_appDlg::OnBnClickedView)
	ON_COMMAND(IDM_TOOL_DUMPXML, &Cttf_parser_appDlg::OnToolDumpXml)
  ON_BN_CLICKED(IDC_POINT, &Cttf_parser_appDlg::OnBnClickedHint)
END_MESSAGE_MAP()


// Cttf_parser_appDlg message handlers
BOOL Cttf_parser_appDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL){
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()){
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
  render_point = false;        // No glyph hint by default.
	m_fileNameText.SetWindowTextW(_T("File Name:"));
	// TEST: Following lines are for test.
	// std::string str("C:\\Fonts\\Mathematica6.ttf");
	// ttf.load_path(str);
	// m_char.SetWindowText(_T("A"));

	m_btn_view.EnableWindow(false);									// disable button "View"
	GetMenu()->EnableMenuItem(IDM_TOOL_DUMPXML, MF_DISABLED);		// disable menu button "Dump XML"
	
	HDC hdc = ::GetDC(m_hWnd);
	m_charBmp = CreateCompatibleBitmap(hdc, 500, 500); // FIXME: need to DeleteObject(m_charBmp)
	::ReleaseDC(m_hWnd, hdc);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cttf_parser_appDlg::OnSysCommand(UINT nID, LPARAM lParam){
	if ((nID & 0xFFF0) == IDM_ABOUTBOX){
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cttf_parser_appDlg::OnPaint(){
	Invalidate(1);
	if (IsIconic()){ // Return TRUE if the dialog is minimized.
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}else{
		CDialogEx::OnPaint();
		HDC hdc = ::GetDC(m_hWnd);
		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP old_bmp = (HBITMAP)SelectObject(memdc, m_charBmp);
		BitBlt(hdc, 100, 100, 500, 500, memdc, 0, 0, SRCCOPY);
		SelectObject(memdc, old_bmp);
		DeleteObject(memdc);
		::ReleaseDC(m_hWnd, hdc);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cttf_parser_appDlg::OnQueryDragIcon(){
	return static_cast<HCURSOR>(m_hIcon);
}

void Cttf_parser_appDlg::OnFileOpen(){
	CString path_name;
	CFileDialog dlg(TRUE); // TRUE for "open" dialog; FALSE for "save as" dialog.
	if(dlg.DoModal() == IDOK){
		path_name = dlg.GetPathName();
		m_fileNameText.SetWindowTextW(_T("File Name: ") + path_name);
		CStringA path_nameS(path_name.GetBuffer(0));
		std::string str = path_nameS.GetBuffer(0);
    ttf.~True_Type_Font();
		ttf.load_path(str);
		m_btn_view.EnableWindow(true);									// enable button "View"
		GetMenu()->EnableMenuItem(IDM_TOOL_DUMPXML, MF_ENABLED);		// enable menu button "Dump XML"
	}
}


void Cttf_parser_appDlg::OnFileExit(){
	EndDialog(0);
}

void Cttf_parser_appDlg::OnBnClickedView(){
  refresh_glyph();
}


void Cttf_parser_appDlg::OnToolDumpXml(){
	if(ttf.dump_ttf("info.xml")){
		MessageBox(_T("Dumped successfully!"), _T("Message"));
	}else{
		MessageBox(_T("Failed to dump info!"), _T("Message"));
	}
}

void Cttf_parser_appDlg::OnBnClickedHint(){
  // Renew the status of 'show hint' and refresh glyph.
  render_point = (IsDlgButtonChecked(IDC_POINT) == BST_CHECKED);
  refresh_glyph();
}

void Cttf_parser_appDlg::refresh_glyph(){
  CString char_string;
  m_char.GetWindowText(char_string);
  if(!char_string.IsEmpty() && m_charBmp){
    HDC hdc = ::GetDC(m_hWnd);
    render_glyph(hdc, m_charBmp, char_string[0], 500, 500, render_point); // FIXME: test if ttf is loaded before render.
    Invalidate();
    ::ReleaseDC(m_hWnd, hdc);
  }
}

BOOL Cttf_parser_appDlg::PreTranslateMessage(MSG* pMsg){
  if(pMsg->message == WM_KEYDOWN) { 
    switch(pMsg->wParam)  {  
    case VK_RETURN:   // omit Enter 
      return true;  
    case VK_ESCAPE:   // omit Esc  
      return true;  
    } 
  }
  return CDialogEx::PreTranslateMessage(pMsg);
}