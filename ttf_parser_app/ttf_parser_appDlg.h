
// ttf_parser_appDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// Cttf_parser_appDlg dialog
class Cttf_parser_appDlg : public CDialogEx{
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
	CEdit m_char;
  CButton m_btn_view;
  CStatic m_fileNameText;
  CSliderCtrl m_slider_glyph_index;

  ttf_dll::USHORT glyph_index;
  bool render_point;
  HBITMAP charBmp;

  void refresh_glyph();
  afx_msg void OnFileOpen();
  afx_msg void OnFileExit();
  afx_msg void OnBnClickedView();
	afx_msg void OnToolDumpXml();
  afx_msg void OnBnClickedShowPoint();
  BOOL PreTranslateMessage(MSG* pMsg);
  afx_msg void OnCbnSelchangeGlyphIndex();
  afx_msg void OnNMCustomdrawSliderGlyphIndex(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
