#include "pch.h"
#include "SocketClient.h"


SocketClient::SocketClient(CChattingDlg* cChattingDlg) {
	this->cChattingDlg = cChattingDlg;
}
SocketClient::~SocketClient() {

}

void SocketClient::OnConnect(int nErrorCode) {
	if (nErrorCode == 0) {
		cChattingDlg->GetMsgBox(_T("Kết nối thành công."));
		cChattingDlg->m_btn_signin.EnableWindow(TRUE); //Bật nút đăng nhập
		cChattingDlg->m_btn_signup.EnableWindow(TRUE); //bật nút đăng ký
		cChattingDlg->m_btn_connect.EnableWindow(FALSE); //Tắt nút kết nối
		cChattingDlg->m_edt_port.EnableWindow(FALSE);
	}
	else {
		cChattingDlg->GetMsgBox(_T("Kết nối không thành công. Hãy thử lại."));
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void SocketClient::OnReceive(int nErrorCode) {

	char buff[512];
	ZeroMemory(buff, 512);
	int len = this->Receive(buff, 512, 0);
	if (len > 0) {
		CString msg(buff, len);
		if (msg == _T("SIGNINACP")) {
			cChattingDlg->LoginSuccess();
		}
		else if (msg == _T("WRONG")) {
			cChattingDlg->GetMsgBox(_T("Sai username hoặc password"));
		}
		else if (GetSignal(msg) == _T("NEWCL")) {
			CString newUser = GetNewClient(msg);
			cChattingDlg->m_list_user.AddString(newUser);
		}
		else if (GetSignal(msg) == _T("OLDCL")) {
			GetOldClient(msg);
			//CString newUser = GetNewClient(msg);
			//cChattingDlg->m_list_user.AddString(newUser);
		}
		else if (msg == _T("LOGIN2")) {
			cChattingDlg->GetMsgBox(_T("Ai đó đã đăng nhập vào tài khoản của bạn rồi."));
		}
		else if (GetSignal(msg) == _T("NEWMSG")) {
			int iCheck=0; //biến iChheck kiểm tra xem đã có tin nhắn nào chưa
			SendMsgStruct temp = GetSendMsgStruct(msg); 
			for (int i = 0; i < cChattingDlg->msgLogs.size(); i++) {
				if (cChattingDlg->msgLogs.at(i).targetName == temp.yourName) { //kiểm tra xem người nhận là ai
					CString newMsg;
					CString a, b;
					a = temp.yourName;
					b = temp.msg;
					newMsg.Format(_T("%s:%s"), a, b); //username: message (Ex: phuong: hello)
					cChattingDlg->msgLogs.at(i).msg.push_back(newMsg); //đẩy tin nhắn vào vector để hiển thị
					iCheck++; // tăng biến icheck
					int indexOfUsername = cChattingDlg->m_list_user.FindString(-1, a);
					if (indexOfUsername != LB_ERR) {
						int indexItem = cChattingDlg->m_list_user.GetCurSel();
						if (indexItem == indexOfUsername) {
							cChattingDlg->OnLbnSelchangeListUser(); //gọi đến hàm này để in lại tin nhắn
						}
					}
					//cChattingDlg->PrintMsg(a);
					//cChattingDlg->OnLbnSelchangeListUser(); //gọi đến hàm này để in lại tin nhắn
					cChattingDlg->BoldUsernameNewMsg(a); //Hàm này để đưa username mới gửi tin nhắn đến lên đầu danh sách
					cChattingDlg->SetTextDialogWhenNewMsg(a); //Hàm này để set textt : tk1 đã gửi tin nhắn cho bạn
					break;
				}
			}
			if (iCheck == 0) {
				MessageStruct tempMsg;
				CString newMsg;
				newMsg.Format(_T("%s:%s\r\n"), temp.yourName, temp.msg);
				tempMsg.targetName = temp.yourName;
				tempMsg.msg.push_back(newMsg);
				cChattingDlg->msgLogs.push_back(tempMsg);
				cChattingDlg->OnLbnSelchangeListUser();
				cChattingDlg->BoldUsernameNewMsg(temp.yourName);
				cChattingDlg->SetTextDialogWhenNewMsg(temp.yourName);
			}
			cChattingDlg->PlaySoundIfNewMsg();
		}
		else if (msg == _T("ACPSIGNUP")) {
			cChattingDlg->MessageBox(_T("Đăng ký thành công. Vui lòng đăng nhập để bắt đầu sử dụng"));
			cChattingDlg->m_edt_username.SetWindowTextW(_T(""));
			cChattingDlg->m_edt_password.SetWindowTextW(_T(""));
		}
		else if (msg == _T("NOACPSIGNUP")) {
			cChattingDlg->MessageBox(_T("Username đã được sử dụng. Vui lòng sử dụng Username khác."));
		}
		else if (GetSignal(msg) == _T("CLOFF")) {
			CString clOff = GetUsernameLogout(msg);
			int index = cChattingDlg->m_list_user.FindString(-1, clOff);
			if (index != LB_ERR) {
				cChattingDlg->m_list_user.DeleteString(index); // xóa phần tử
				cChattingDlg->m_list_msg.ResetContent();
			}
		}
		
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

CString SocketClient::GetSignal(CString msg) {
	CString signal = NULL;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		else {
			signal += msg[j];
		}
		if (wordCount == 1) {
			break;
		}
	}
	return signal;
}

CString SocketClient::GetNewClient(CString msg) {
	CString username = NULL;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		if (wordCount == 1 && msg[j] != ' ') {
			username += msg[j];
		}
		if (wordCount == 2) {
			break;
		}
	}
	return username;
}

void SocketClient::GetOldClient(CString msg) {
	CString username = NULL;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
			if (wordCount > 1) {
				cChattingDlg->m_list_user.AddString(username);
			}
			username = _T("");
			
		}
		if (wordCount >= 1 && msg[j] != ' ') {
			username += msg[j];
		}
	}
}

SendMsgStruct SocketClient::GetSendMsgStruct(CString msg) {
	SendMsgStruct temp;
	CString targetName = NULL;
	CString yourName = NULL;
	CString msgA;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		if (wordCount == 1 && msg[j] != ' ') {
			targetName += msg[j];
		}
		else if (wordCount == 2 && msg[j] != ' ') {
			yourName += msg[j];
		}
		else if (wordCount > 2) {
			msgA += msg[j];
		}
	}
	temp.msg = msgA;
	temp.targetName = targetName;
	temp.yourName = yourName;
	return temp;
}

CString SocketClient::GetUsernameLogout(CString msg) {
	CString username = NULL;
	int wordCount = 0;
	for (int j = 0; j < msg.GetLength(); j++) {
		if (msg[j] == ' ') {
			wordCount++;
		}
		if (wordCount == 3) {
			break;
		}
		if (wordCount == 1 && msg[j] != ' ') {
			username += msg[j];
		}
	}
	return username;
}
