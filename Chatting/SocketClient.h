#pragma once
#include <afxsock.h>
#include "ChattingDlg.h"


struct SendMsgStruct {
    CString targetName;
    CString yourName;
    CString msg;
};

class CChattingDlg;
class SocketClient:public CAsyncSocket
{
public:
    CChattingDlg* cChattingDlg;
	SocketClient(CChattingDlg* cChattingDlg);
	~SocketClient();
    // Xử lý sự kiện kết nối
    virtual void OnConnect(int nErrorCode);
    // Xử lý sự kiện nhận tin nhắn từ server
    virtual void OnReceive(int nErrorCode);
    // Xử lý sự kiện đóng kết nối
   // void OnClose(int nErrorCode);
//    virtual void OnSend(int nErrorCode);
public:
    CString GetNewClient(CString msg);
    CString GetSignal(CString msg);
    SendMsgStruct GetSendMsgStruct(CString msg);
    CString GetUsernameLogout(CString msg);
    void GetOldClient(CString msg);
};

