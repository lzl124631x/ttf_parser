#include "stdafx.h"
#include "ttf_parser_app.h"
#include "ttf_parser_app_dlg.h"
#include "ttf_parser_gdiplus.h"
#include "afxdialogex.h"
#include <io.h> // For `_findclose`

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void SetEditGlyphIndex(CEdit &edit, ttf_dll::GlyphId glyph_index);

BOOL CMySliderCtrl::PreTranslateMessage(MSG* pMsg) {
  if(pMsg->message == WM_KEYDOWN) {
    if(pMsg->wParam == VK_UP) {
      pMsg->wParam = VK_DOWN;
    } else if(pMsg->wParam == VK_DOWN) {
      pMsg->wParam = VK_UP;
    }
  }
  return CSliderCtrl::PreTranslateMessage(pMsg);
}

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx {
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CTTFParserAppDlg dialog
CTtfParserAppDlg::CTtfParserAppDlg(CWnd* pParent /*=NULL*/)
  : CDialogEx(CTtfParserAppDlg::IDD, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTtfParserAppDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_CHAR, cedit_char_);
  DDX_Control(pDX, IDC_VIEW, cbtn_view_);
  DDX_Control(pDX, IDC_SLIDER_GLYPH_INDEX, cslider_glyph_index_);
  DDX_Control(pDX, IDC_EDIT_GLYPH_INDEX, cedit_glyph_index_);
  DDX_Control(pDX, IDC_SPIN_GLYPH_INDEX, cspin_glyph_index_);
}

BEGIN_MESSAGE_MAP(CTtfParserAppDlg, CDialogEx)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_COMMAND(IDM_FILE_OPEN, &CTtfParserAppDlg::OnFileOpen)
  ON_COMMAND(IDM_FILE_EXIT, &CTtfParserAppDlg::OnFileExit)
  ON_BN_CLICKED(IDC_VIEW, &CTtfParserAppDlg::OnBnClickedView)
  ON_COMMAND(IDM_TOOL_DUMPXML, &CTtfParserAppDlg::OnToolDumpXml)
  ON_BN_CLICKED(IDC_CHECK_SHOW_POINT, &CTtfParserAppDlg::OnBnClickedShowPoint)
  ON_WM_HSCROLL()
  ON_EN_CHANGE(IDC_EDIT_GLYPH_INDEX, &CTtfParserAppDlg::OnEnChangeEditGlyphIndex)
  ON_WM_DROPFILES()
END_MESSAGE_MAP()

// Cttf_parser_appDlg message handlers
BOOL CTtfParserAppDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL) {
    BOOL bNameValid;
    CString strAboutMenu;
    bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
    ASSERT(bNameValid);
    if (!strAboutMenu.IsEmpty()) {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);      // Set big icon
  SetIcon(m_hIcon, FALSE);     // Set small icon

  cspin_glyph_index_.SetBuddy(&cedit_glyph_index_);
  // No glyph points are rendered by default.
  render_point_ = false;
  EnableControls(false);

  HDC hdc = ::GetDC(m_hWnd);
  // `char_bmp_` will be deleted in `CTtfParserAppDlg::DestroyWindow`.
  char_bmp_ = CreateCompatibleBitmap(hdc, 500, 500);
  ::ReleaseDC(m_hWnd, hdc);

  // TEST:
  // TraverseFolder(_T("D:/fonts"));

  return TRUE;
}

void CTtfParserAppDlg::OnSysCommand(UINT nID, LPARAM lParam) {
  if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  } else {
    CDialogEx::OnSysCommand(nID, lParam);
  }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CTtfParserAppDlg::OnPaint() {
  if (IsIconic()) { // Return TRUE if the dialog is minimized.
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
  } else {
    CDialogEx::OnPaint();
    HDC hdc = ::GetDC(m_hWnd);
    HDC memdc = CreateCompatibleDC(hdc);
    HBITMAP old_bmp = (HBITMAP)SelectObject(memdc, char_bmp_);
    BitBlt(hdc, 100, 100, 500, 500, memdc, 0, 0, SRCCOPY);
    SelectObject(memdc, old_bmp);
    DeleteObject(memdc);
    ::ReleaseDC(m_hWnd, hdc);
  }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTtfParserAppDlg::OnQueryDragIcon() {
  return static_cast<HCURSOR>(m_hIcon);
}

void CTtfParserAppDlg::OnFileOpen() {
  CFileDialog dlg(TRUE); // TRUE for "open" dialog; FALSE for "save as" dialog.
  if(dlg.DoModal() == IDOK) {
    OpenTtf(dlg.GetPathName());
  }
}

void CTtfParserAppDlg::OnDropFiles(HDROP hDropInfo) {
  TCHAR path_name[MAX_PATH] = {0};
  DragQueryFile(hDropInfo, 0, path_name, _countof(path_name));
  OpenTtf(path_name);
  CDialog::OnDropFiles(hDropInfo);
}

void CTtfParserAppDlg::OpenTtf(LPCTSTR path) {
  GetDlgItem(IDC_TEXT_FILE_NAME)->SetWindowText(path);
  ttf.Close();
  ttf.Open(path);
  if (ttf.is_open()) {
    EnableControls(true);
    cslider_glyph_index_.SetRange(0, ttf.maxp().num_glyphs() - 1);
    // Must use `SetRange32` otherwise overflow might occur.
    cspin_glyph_index_.SetRange32(0, ttf.maxp().num_glyphs() - 1);
  } else {
    EnableControls(false);
    MessageBox(_T("Oops! The file is corrupt!"), _T("Error"), MB_ICONERROR | MB_OK);
  }
}

void CTtfParserAppDlg::OnFileExit() {
  EndDialog(0);
}

void CTtfParserAppDlg::OnBnClickedView() {
  CString char_string;
  cedit_char_.GetWindowText(char_string);
  if(!char_string.IsEmpty() && char_bmp_) {
    glyph_index_ = ttf.cmap().GetGlyphIndex(ttf_dll::kWindows,
                                               ttf_dll::kUnicodeBMP,
                                               char_string[0]);
    SetEditGlyphIndex(cedit_glyph_index_, glyph_index_);
  }
}

void CTtfParserAppDlg::OnToolDumpXml() {
  if(ttf.DumpTtf(_T("info.xml"))) {
    MessageBox(_T("Dumped successfully!"), _T("Message"));
  } else {
    MessageBox(_T("Failed to dump info!"), _T("Message"));
  }
}

void CTtfParserAppDlg::OnBnClickedShowPoint() {
  // Renew the status of 'show point' and refresh glyph.
  render_point_ = (IsDlgButtonChecked(IDC_CHECK_SHOW_POINT) == BST_CHECKED);
  RefreshGlyph();
}

void CTtfParserAppDlg::RefreshGlyph() {
  HDC hdc = ::GetDC(m_hWnd);
  const ttf_dll::Glyph &glyph = ttf.GetGlyph(glyph_index_);
  RenderGlyph(hdc, char_bmp_, glyph, 500, 500, render_point_);
  // FIXME: test if ttf is loaded before render.
  Invalidate();
  TCHAR buf[300] = {0};
  ttf.GlyphInfo(glyph, buf, 300);
  GetDlgItem(IDC_TEXT_GLYPH_INFO)->SetWindowText(buf);
  ::ReleaseDC(m_hWnd, hdc);
}

BOOL CTtfParserAppDlg::PreTranslateMessage(MSG* pMsg) {
  if(pMsg->message == WM_KEYDOWN) {
    switch(pMsg->wParam)  {
      case VK_RETURN:
        // omit Enter
        return true;
      case VK_ESCAPE:
        // omit Esc
        return true;
    }
  }
  return CDialogEx::PreTranslateMessage(pMsg);
}

void CTtfParserAppDlg::EnableControls(bool b) {
  // Button "view".
  cbtn_view_.EnableWindow(b);
  // Menu Button "Dump Xml".
  GetMenu()->EnableMenuItem(IDM_TOOL_DUMPXML, b ? MF_ENABLED : MF_DISABLED);
  // Check Box "show point".
  GetDlgItem(IDC_CHECK_SHOW_POINT)->EnableWindow(b);
  // Slider "Glyph Index".
  cslider_glyph_index_.EnableWindow(b);
  // Edit "Glyph Index".
  cedit_glyph_index_.EnableWindow(b);
  // Glyph 0 is rendered by default.
  glyph_index_ = 0;
  cedit_glyph_index_.SetWindowText(_T("0"));
}

void CTtfParserAppDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
  int tmp = cslider_glyph_index_.GetPos();
  if(tmp != glyph_index_) {
    glyph_index_ = (ttf_dll::GlyphId)tmp;
    SetEditGlyphIndex(cedit_glyph_index_, glyph_index_);
  }
  CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTtfParserAppDlg::OnEnChangeEditGlyphIndex() {
  if(ttf.maxp().num_glyphs() == 0) return;
  CString glyph_index_string;
  cedit_glyph_index_.GetWindowText(glyph_index_string);
  int tmp = _ttoi(glyph_index_string);
  if(tmp >= ttf.maxp().num_glyphs()) {
    glyph_index_ = ttf.maxp().num_glyphs() - 1;
    SetEditGlyphIndex(cedit_glyph_index_, glyph_index_);
  } else {
    glyph_index_ = tmp;
    cslider_glyph_index_.SetPos(glyph_index_);
    RefreshGlyph();
  }
}

void CTtfParserAppDlg::TraverseFolder(LPCTSTR dir) {
  TCHAR old_dir[MAX_PATH] = {0};
  if (!_tgetcwd(old_dir, MAX_PATH)) {
    MessageBox(_T("Failed to locate current folder!"), _T("Message"));
    return;
  }
  _tchdir(dir);
  _tfinddata_t file_info;
  long handle = _tfindfirst(_T("*.ttf"), &file_info);
  if (handle == -1L) {
    MessageBox(_T("Failed to open the folder!"), _T("Message"));
    return;
  }
  TCHAR file_name[MAX_PATH] = {0};
  do {
    _tcscpy_s(file_name, dir);
    _tcscat_s(file_name, _T("/"));
    _tcscat_s(file_name, file_info.name);
    OpenTtf(file_name);
    // `_tfindnext` returns 0 if successful.
  } while(_tfindnext(handle, &file_info) == 0);
  _findclose(handle);
  _tchdir(old_dir);
  MessageBox(_T("Traversal done!"), _T("Message"));
}

BOOL CTtfParserAppDlg::DestroyWindow() {
  ttf.Close();
  DeleteObject(char_bmp_);
  return CDialogEx::DestroyWindow();
}

/****************************************************************************/
static void SetEditGlyphIndex(CEdit &edit, ttf_dll::GlyphId glyph_index) {
  CString glyph_index_string;
  glyph_index_string.Format(_T("%u"), glyph_index);
  edit.SetWindowText(glyph_index_string);
}