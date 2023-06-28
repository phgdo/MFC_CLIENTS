/*
#include "pch.h"
#include "ClientSocket.h"
#include "ChattingDlg.h"

ClientSocket::ClientSocket(CChattingDlg* cChattingDlg) {
	this->cChattingDlg = cChattingDlg;
}

ClientSocket::~ClientSocket() {

}

void ClientSocket::OnReceive(int nErrorCode) {
    if (nErrorCode == 0) {
        // Nhận được dữ liệu từ server, xử lý và hiển thị nó trên giao diện người dùng
        char buff[512];
        ZeroMemory(buff, 512);
        int len = Receive(buff, 512, 0);
        if (len > 0) {
            CString msg(buff, len);
            if (cChattingDlg != NULL) {
                cChattingDlg->GetMessageFromSocket(msg);
            }
        }
    }
    else {
        // Lỗi nhận dữ liệu
    }
}

void ClientSocket::OnConnect(int nErrorCode) {
    if (nErrorCode == 0) {
        this->Connect(_T("127.0.0.1"), 8888);
        cChattingDlg->MsgBox(_T("Không thể kết nối"));
    }
}
*/