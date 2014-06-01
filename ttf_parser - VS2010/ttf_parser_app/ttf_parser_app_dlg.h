
// ttf_parser_appdlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// A customized slider control. The default horizontal slider control
// will move right and increment the 'Pos' value when the DOWN arrow key
// is pressed. I want it to work in the opposite way: slider moves right/left
// as the UP/DOWN key is pressed. It's done by overriding the function
// 'PreTranslateMessage'.
class CMySliderCtrl : public CSliderCtrl {
 private:
  BOOL PreTranslateMessage(MSG* pMsg);
};

// CTtfParserAppDlg dialog
class CTtfParserAppDlg : public CDialogEx {
 public:
  enum { IDD = IDD_TTF_PARSER_APP_DIALOG };

  CTtfParserAppDlg(CWnd* pParent = NULL);	// standard constructor
  BOOL PreTranslateMessage(MSG* pMsg);
  afx_msg void OnFileOpen();
  afx_msg void OnFileExit();
  afx_msg void OnBnClickedView();
  afx_msg void OnToolDumpXml();
  afx_msg void OnBnClickedShowPoint();
  afx_msg void OnCbnSelchangeGlyphIndex();
  afx_msg void OnNMCustomdrawSliderGlyphIndex(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnEnChangeEditGlyphIndex();
  afx_msg void OnDeltaposSpinGlyphIndex(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnDropFiles(HDROP hDropInfo);

 protected:
  virtual void DoDataExchange(CDataExchange* pDX);      // DDX/DDV support
  // Generated message map functions
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()

  HICON m_hIcon;
 private:
  void OpenTtf(LPCTSTR path);
  void RefreshGlyph();
  void EnableControls(bool b);
  void TraverseFolder(LPCTSTR dir);

  CEdit               cedit_char_;
  CButton             cbtn_view_;
  CMySliderCtrl       cslider_glyph_index_;
  CEdit               cedit_glyph_index_;
  CSpinButtonCtrl     cspin_glyph_index_;

  ttf_dll::GlyphId    glyph_index_;
  bool                render_point_;
  HBITMAP             char_bmp_;
public:
  afx_msg void OnStnClickedTextFileName();
  afx_msg void OnStnClickedTextFileNameTitle();
};