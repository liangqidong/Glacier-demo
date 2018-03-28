//---------------------------------------------------------------------------
#include <vcl.h>
#include <jpeg.hpp>
#pragma hdrstop

#include "RecvStream.h"
#include "ChildWin.h"
#include "Socket.cpp"
#include "Ping.cpp"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TRecvStreamThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
__fastcall TRecvStreamThread::TRecvStreamThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
bool __fastcall TRecvStreamThread::LoadImage(TImage *Image1)
{
	CPingReply reply;
	CPing PingHost;
	bool bRtn = false;  // 函数返回值初始为FALSE
	MainForm->StatusBar->SimpleText = "正在连接主机...";
	// 先PING主机，检测网络是否畅通
	bool rtn = PingHost.Ping(RemoteAddress.c_str(),reply,64,4000,32);
	if (rtn) {
		u_short RecvPort=0;
		int RecvSocket = BindSocket(&RecvPort);  // 动态分配接收端口

		if (RecvSocket) {
			// 将接收端口和色深、图象品质合成一条命令，参数之间以'\n'分隔
			AnsiString Msg = IntToStr(RecvPort) + "\n" +
				IntToStr(CL) + "\n" +
				IntToStr(CQ) + "\n";
			// 向远程主机发送命令
			if (SendMsg(RemoteAddress,LISTENPORT,Msg)) {
				TMemoryStream *Stream;
				try {
					// 定义一个数据流并分配内存
					Stream = new TMemoryStream;  
					MainForm->StatusBar->SimpleText = "正在接收数据...";
					Application->ProcessMessages();  // 处理系统消息
					// 开始接收图象到数据流中
					if (RecvStream(RecvSocket,Stream)) {
						TJPEGImage *jpeg;  // 定义JPEG图象
						try {
							jpeg = new TJPEGImage;  // 分配内存
							// 从数据流中载入图象
							jpeg->LoadFromStream(Stream);  
							// 显示图象
							Image1->Picture->Bitmap->Assign(jpeg);  
							MessageBeep(MB_OK);  // 发出提示声音
							// 返回值为TRUE，表示成功
							bRtn = true;  
						}
						__finally {
							delete jpeg;  // 释放资源
						}
					}
					else
						MessageBox(0,"接收数据流失败","冰河",MB_ICONERROR);
					}
				__finally {
					delete Stream;  // 释放资源
				}
			}
			else
				MessageBox(0,("无法与主机'"+ RemoteAddress +"'建立连接").c_str(),
					"冰河",MB_ICONERROR);
		}
		else
			MessageBox(0,"分配端口失败，无法继续接收数据","冰河",MB_ICONERROR);
	}
	else
		MessageBox(0,("主机'"+RemoteAddress+"'没有响应").c_str(),"冰河",MB_ICONERROR);
    
	return bRtn;
}

void __fastcall TRecvStreamThread::Execute()
{
        //---- Place thread code here ----
	if (LoadImage(RemoteScreen))
		MainForm->StatusBar->SimpleText = "数据接收完毕";
	else
		MainForm->StatusBar->SimpleText = "接收数据失败";
}
//---------------------------------------------------------------------------
