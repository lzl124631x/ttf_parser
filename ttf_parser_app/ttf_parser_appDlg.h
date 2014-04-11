
// ttf_parser_appDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "True_Type_Font.h"
using namespace ttf_dll;

// Cttf_parser_appDlg dialog
class Cttf_parser_appDlg : public CDialogEx
{
// Construction
public:
	Cttf_parser_appDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TTF_PARSER_APP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileExit();
	CStatic m_fileNameText;
	HBITMAP m_charBmp;
	True_Type_Font m_ttf;
	void render_glyph(HBITMAP bmp, ttf_dll::USHORT ch);
	CEdit m_char;
	afx_msg void OnBnClickedView();
	afx_msg void OnToolDumpinfo();
};
