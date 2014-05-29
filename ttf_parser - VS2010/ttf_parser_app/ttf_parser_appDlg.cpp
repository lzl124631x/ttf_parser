#include "stdafx.h"
#include "ttf_parser_app.h"
#include "ttf_parser_appDlg.h"
#include "ttf_parser_gdiplus.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void set_edit_glyph_index(CEdit &edit, ttf_dll::GLYPH_ID glyph_index);

BOOL CMySliderCtrl::PreTranslateMessage(MSG* pMsg){
  if(pMsg->message == WM_KEYDOWN){
    if(pMsg->wParam == VK_UP){
      pMsg->wParam = VK_DOWN;
    }else if(pMsg->wParam == VK_DOWN){
      pMsg->wParam = VK_UP;
    }
  }
  return CSliderCtrl::PreTranslateMessage(pMsg);
}

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
  DDX_Control(pDX, IDC_EDIT_CHAR, m_edit_char);
  DDX_Control(pDX, IDC_VIEW, m_btn_view);
  DDX_Control(pDX, IDC_SLIDER_GLYPH_INDEX, m_slider_glyph_index);
  DDX_Control(pDX, IDC_TEXT_FILE_NAME, m_text_file_name);
  DDX_Control(pDX, IDC_EDIT_GLYPH_INDEX, m_edit_glyph_index);
  DDX_Control(pDX, IDC_SPIN_GLYPH_INDEX, m_spin_glyph_index);
}

BEGIN_MESSAGE_MAP(Cttf_parser_appDlg, CDialogEx)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_COMMAND(IDM_FILE_OPEN, &Cttf_parser_appDlg::OnFileOpen)
  ON_COMMAND(IDM_FILE_EXIT, &Cttf_parser_appDlg::OnFileExit)
  ON_BN_CLICKED(IDC_VIEW, &Cttf_parser_appDlg::OnBnClickedView)
  ON_COMMAND(IDM_TOOL_DUMPXML, &Cttf_parser_appDlg::OnToolDumpXml)
  ON_BN_CLICKED(IDC_CHECK_SHOW_POINT, &Cttf_parser_appDlg::OnBnClickedShowPoint)
  ON_WM_HSCROLL()
  ON_EN_CHANGE(IDC_EDIT_GLYPH_INDEX, &Cttf_parser_appDlg::OnEnChangeEditGlyphIndex)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GLYPH_INDEX, &Cttf_parser_appDlg::OnDeltaposSpinGlyphIndex)
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
  SetIcon(m_hIcon, TRUE);      // Set big icon
  SetIcon(m_hIcon, FALSE);    // Set small icon

  // TEST: Following lines are for test.
  // std::string str("C:\\Fonts\\Mathematica6.ttf");
  // ttf.load_path(str);
  // m_char.SetWindowText(_T("A"));

  m_spin_glyph_index.SetBuddy(&m_edit_glyph_index);
  render_point = false;                               // No glyph points are rendered by default
  enable_controls(false);

  HDC hdc = ::GetDC(m_hWnd);
  charBmp = CreateCompatibleBitmap(hdc, 500, 500); // FIXME: need to DeleteObject(m_charBmp)
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
    HBITMAP old_bmp = (HBITMAP)SelectObject(memdc, charBmp);
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
    m_text_file_name.SetWindowText(_T("File Name: ") + path_name);
    CStringA path_nameS(path_name.GetBuffer(0));
    std::string str = path_nameS.GetBuffer(0);
    ttf.~True_Type_Font();
    ttf.load_path(str);
    enable_controls(true);
    m_slider_glyph_index.SetRange(0, ttf.maxp.num_glyphs - 1);
    m_spin_glyph_index.SetRange(0, ttf.maxp.num_glyphs - 1);
  }
}

void Cttf_parser_appDlg::OnFileExit(){
  EndDialog(0);
}

void Cttf_parser_appDlg::OnBnClickedView(){
  CString char_string;
  m_edit_char.GetWindowText(char_string);
  if(!char_string.IsEmpty() && charBmp){
    glyph_index = ttf.cmap.get_glyph_index(ttf_dll::Windows, ttf_dll::Unicode_BMP, char_string[0]);
    set_edit_glyph_index(m_edit_glyph_index, glyph_index);
  }
}

void Cttf_parser_appDlg::OnToolDumpXml(){
  if(ttf.dump_ttf("info.xml")){
    MessageBox(_T("Dumped successfully!"), _T("Message"));
  }else{
    MessageBox(_T("Failed to dump info!"), _T("Message"));
  }
}

void Cttf_parser_appDlg::OnBnClickedShowPoint(){
  // Renew the status of 'show point' and refresh glyph.
  render_point = (IsDlgButtonChecked(IDC_CHECK_SHOW_POINT) == BST_CHECKED);
  refresh_glyph();
}

void Cttf_parser_appDlg::refresh_glyph(){
    HDC hdc = ::GetDC(m_hWnd);
    ttf_dll::Glyph *glyph = ttf.get_glyph(glyph_index);
    render_glyph(hdc, charBmp, glyph, 500, 500, render_point); // FIXME: test if ttf is loaded before render.
    Invalidate();
    TCHAR buf[300] = {0};
    ttf.glyph_info(glyph, buf, 300);
    GetDlgItem(IDC_TEXT_GLYPH_INFO)->SetWindowText(buf);
    ::ReleaseDC(m_hWnd, hdc);
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

void Cttf_parser_appDlg::enable_controls(bool b){
  m_btn_view.EnableWindow(b);                                                   // button "View"
  GetMenu()->EnableMenuItem(IDM_TOOL_DUMPXML, b ? MF_ENABLED : MF_DISABLED);    // menu button "Dump XML"
  GetDlgItem(IDC_CHECK_SHOW_POINT)->EnableWindow(b);                            // check box "show point"
  m_slider_glyph_index.EnableWindow(b);                                         // slider "Glyph Index"
  m_edit_glyph_index.EnableWindow(b);                                           // edit "Glyph Index"
  glyph_index = 0;                                                              // glyph 0 is rendered by default.
  m_edit_glyph_index.SetWindowText(_T("0"));
}

void Cttf_parser_appDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
  int tmp = m_slider_glyph_index.GetPos();
  if(tmp != glyph_index){
    glyph_index = (ttf_dll::GLYPH_ID)tmp;
    set_edit_glyph_index(m_edit_glyph_index, glyph_index);
  }
  CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void Cttf_parser_appDlg::OnEnChangeEditGlyphIndex(){
  if(ttf.maxp.num_glyphs == 0) return;
  CString glyph_index_string;
  m_edit_glyph_index.GetWindowText(glyph_index_string);
  int tmp = _ttoi(glyph_index_string);
  if(tmp >= ttf.maxp.num_glyphs){
    glyph_index = ttf.maxp.num_glyphs - 1;
    set_edit_glyph_index(m_edit_glyph_index, glyph_index);
  }else{
    glyph_index = tmp;
    m_slider_glyph_index.SetPos(glyph_index);
    refresh_glyph();
  }
}


void Cttf_parser_appDlg::OnDeltaposSpinGlyphIndex(NMHDR *pNMHDR, LRESULT *pResult){
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  if(pNMUpDown->iDelta == -1){          // The down arrow is pressed.
    if(glyph_index > 0){
      --glyph_index;
    }
  }else if(pNMUpDown->iDelta == 1){     // The up arrow is pressed.
    if(glyph_index < ttf.maxp.num_glyphs - 1){
      ++glyph_index;
    }
  }
  *pResult = 0;
}

static void set_edit_glyph_index(CEdit &edit, ttf_dll::GLYPH_ID glyph_index){
  CString glyph_index_string;
  glyph_index_string.Format(_T("%u"), glyph_index);
  edit.SetWindowText(glyph_index_string);
}