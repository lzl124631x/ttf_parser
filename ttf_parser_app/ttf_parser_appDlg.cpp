
// ttf_parser_appDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ttf_parser_app.h"
#include "ttf_parser_appDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cttf_parser_appDlg dialog
Cttf_parser_appDlg::Cttf_parser_appDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cttf_parser_appDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cttf_parser_appDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NAME, m_fileNameText);
}

BEGIN_MESSAGE_MAP(Cttf_parser_appDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_FILE_OPEN, &Cttf_parser_appDlg::OnFileOpen)
	ON_COMMAND(IDM_FILE_EXIT, &Cttf_parser_appDlg::OnFileExit)
END_MESSAGE_MAP()


// Cttf_parser_appDlg message handlers

BOOL Cttf_parser_appDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_fileNameText.SetWindowTextW(_T("File Name:"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cttf_parser_appDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cttf_parser_appDlg::OnPaint()
{
	if (IsIconic()) // Return TRUE if the dialog is minimized.
	{
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
	}
	else
	{
		CDialogEx::OnPaint();
		render_glyph();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cttf_parser_appDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cttf_parser_appDlg::OnFileOpen()
{
	CString path_name;
	CFileDialog dlg(TRUE); // TRUE for "open" dialog; FALSE for "save as" dialog.
	if(dlg.DoModal() == IDOK){
		path_name = dlg.GetPathName();
		m_fileNameText.SetWindowTextW(_T("File Name: ") + path_name);
		CStringA path_nameS(path_name.GetBuffer(0));
		std::string str = path_nameS.GetBuffer(0);
		m_ttf.load_path(str);
	}
}


void Cttf_parser_appDlg::OnFileExit()
{
	EndDialog(0);
}

/*
<flag, next_flag>
<0, 0>: AddQuadraticBezier(last_point, control_point, cur_point)
<0, 1>: AddQuadraticBezier(last_point, control_point, cur_point) and skip next point.
<1, *>: AddLine(last_point, cur_point)
*/
void AddQuadraticBezier(GraphicsPath &path, const Point &q0, const Point &q1, const Point &q2){
	Point c1, c2;
	//三次贝塞尔的控制点c1 = (q0 + 2 * q1)/3 = q0 + 2 * (q1 - q0)/3
	c1.X = (q0.X + 2 * q1.X) / 3.0f;
	c1.Y = (q0.Y + 2 * q1.Y) / 3.0f;

	//三次贝塞尔的控制点c2 = (2 * q1 + q2)/3 = c1 + (q2 - q0)/3
	c2.X = (2 * q1.X + q2.X)/3.0f;
	c2.Y = (2 * q1.Y + q2.Y)/3.0f;

	path.AddBezier(q0, c1, c2, q2);
}
#define ON_CURVE			0x1 // FIXME: the following code is in test mode.
void Cttf_parser_appDlg::render_glyph(){
	Graphics g(GetSafeHwnd());
	GraphicsPath path;
	Simple_Glyph_Description *glyph_data = (Simple_Glyph_Description*)m_ttf.glyph_data_array[10];
	FWORD width = glyph_data->x_max - glyph_data->x_min;
	FWORD height = glyph_data->y_max - glyph_data->y_min;
	double x_ratio = 300.0 / width;
	double y_ratio = 300.0 / height;
	Point start_point, last_point, cur_point;

	ttf_dll::BYTE flag = 0, next_flag = 0;
	bool new_contour = true;
	for(int i = 0, j = 0; i < glyph_data->pt_num; ++i){
		flag = glyph_data->flags[i] & ON_CURVE;
		cur_point = Point(glyph_data->x_coordinates[i], glyph_data->y_coordinates[i]);
		if(new_contour){
			path.StartFigure();
			new_contour = false;
		}else if(flag == 1){
			path.AddLine(last_point, cur_point);
		}else{
			Point next_point;
			if(i == glyph_data->end_pts_of_contours[j]){// This is the last point of this jth contour.
				next_flag = 0;
			}else{
				next_flag = glyph_data->flags[i + 1];
				if(next_flag == 1){
					next_point = Point(glyph_data->x_coordinates[i + 1], glyph_data->y_coordinates[i + 1]);
					++i;// skip the next on-curve point
				}else{
					next_point = Point((cur_point.X + glyph_data->x_coordinates[i + 1]) >> 1,
						(cur_point.Y + glyph_data->y_coordinates[i + 1]) >> 1);
				}
			}
			AddQuadraticBezier(path, last_point, cur_point, next_point);
		}
		if(i == glyph_data->end_pts_of_contours[j]){
			if(flag == 0){
				if(j == 0){
					printf("%d, %d", glyph_data->x_coordinates[0], glyph_data->y_coordinates[0]);
				}else{
					printf("%d, %d", glyph_data->x_coordinates[glyph_data->end_pts_of_contours[j - 1] + 1],
						glyph_data->y_coordinates[glyph_data->end_pts_of_contours[j - 1] + 1]);
				}
			}
			path.CloseFigure();
			new_contour = true;
			++j;
		}
		next_flag = flag;
	}
	g.DrawPath(&Pen(Color::Black, 1), &path);
}