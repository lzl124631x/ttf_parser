// ttf_parser_appDlg.h : header file
//

#if !defined(AFX_TTF_PARSER_APPDLG_H__DEED3987_9F46_4D86_9B17_8183E5EDAB58__INCLUDED_)
#define AFX_TTF_PARSER_APPDLG_H__DEED3987_9F46_4D86_9B17_8183E5EDAB58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// A customized slider control. The default horizontal slider control
// will move right and increment the 'Pos' value when the DOWN arrow key
// is pressed. I want it to work in the opposite way: slider moves right/left
// as the UP/DOWN key is pressed. It's done by overriding the function
// 'PreTranslateMessage'.
class CMySliderCtrl : public CSliderCtrl{
private:
  BOOL PreTranslateMessage(MSG* pMsg);
};
/////////////////////////////////////////////////////////////////////////////
// Cttf_parser_appDlg dialog

class Cttf_parser_appDlg : public CDialog
{
// Construction
public:
	Cttf_parser_appDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Cttf_parser_appDlg)
	enum { IDD = IDD_TTF_PARSER_APP_DIALOG };
	CSpinButtonCtrl	m_spin_glyph_index;
	CEdit	m_edit_glyph_index;
	CMySliderCtrl	m_slider_glyph_index;
	CButton	m_btn_view;
	CEdit	m_edit_char;
	CStatic	m_text_file_name;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cttf_parser_appDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(Cttf_parser_appDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileOpen();
	afx_msg void OnFileExit();
	afx_msg void OnBnClickedView();
	afx_msg void OnToolDumpXml();
	afx_msg void OnBnClickedShowPoint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditGlyphIndex();
	afx_msg void OnDeltaposSpinGlyphIndex(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
  ttf_dll::GLYPH_ID glyph_index;
  bool render_point;
  HBITMAP charBmp;
  void refresh_glyph();
  void enable_controls(bool b);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTF_PARSER_APPDLG_H__DEED3987_9F46_4D86_9B17_8183E5EDAB58__INCLUDED_)
