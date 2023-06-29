
// ChattingDlg.h : header file
//
#pragma once
#include "SocketClient.h"
#include <vector>
class SocketClient;

struct MessageStruct {
	CString targetName;
	std::vector<CString> msg;
};

// CChattingDlg dialog
class CChattingDlg : public CDialogEx
{
// Construction
public:
	CChattingDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATTING_DIALOG };
#endif

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
	//ClientSocket client;
	CListBox m_list_msg;
	void GetMessageFromSocket(CString msg);
	void MsgBox(CString msg);
	afx_msg void OnBnClickedButtonSignin();
	SocketClient* m_sockclient;
	void GetMsgBox(CString msg);
	afx_msg void OnBnClickedButtonSend();
	CButton m_btn_signin;
	CButton m_btn_signup;
	CEdit m_edt_username;
	CEdit m_edt_password;
	CListBox m_list_user;
	CEdit m_edt_msg;
	CButton m_btn_send;
	void LoginSuccess();
	afx_msg void OnBnClickedButtonSignup();
	CStatic m_txt_us;
	CStatic m_txt_ps;
	CStatic m_txt_login;
	CString yourname;
	CString targetName;
	afx_msg void OnLbnSelchangeListUser();

	std::vector<MessageStruct> msgLogs;
	void F5IfNewMSG();
	afx_msg void OnClose();
	void PrintMsg(CString targetName);
	void BoldUsernameNewMsg(CString username);
	void PlaySoundIfNewMsg();
};
