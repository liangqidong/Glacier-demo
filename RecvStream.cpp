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
	bool bRtn = false;  // ��������ֵ��ʼΪFALSE
	MainForm->StatusBar->SimpleText = "������������...";
	// ��PING��������������Ƿ�ͨ
	bool rtn = PingHost.Ping(RemoteAddress.c_str(),reply,64,4000,32);
	if (rtn) {
		u_short RecvPort=0;
		int RecvSocket = BindSocket(&RecvPort);  // ��̬������ն˿�

		if (RecvSocket) {
			// �����ն˿ں�ɫ�ͼ��Ʒ�ʺϳ�һ���������֮����'\n'�ָ�
			AnsiString Msg = IntToStr(RecvPort) + "\n" +
				IntToStr(CL) + "\n" +
				IntToStr(CQ) + "\n";
			// ��Զ��������������
			if (SendMsg(RemoteAddress,LISTENPORT,Msg)) {
				TMemoryStream *Stream;
				try {
					// ����һ���������������ڴ�
					Stream = new TMemoryStream;  
					MainForm->StatusBar->SimpleText = "���ڽ�������...";
					Application->ProcessMessages();  // ����ϵͳ��Ϣ
					// ��ʼ����ͼ����������
					if (RecvStream(RecvSocket,Stream)) {
						TJPEGImage *jpeg;  // ����JPEGͼ��
						try {
							jpeg = new TJPEGImage;  // �����ڴ�
							// ��������������ͼ��
							jpeg->LoadFromStream(Stream);  
							// ��ʾͼ��
							Image1->Picture->Bitmap->Assign(jpeg);  
							MessageBeep(MB_OK);  // ������ʾ����
							// ����ֵΪTRUE����ʾ�ɹ�
							bRtn = true;  
						}
						__finally {
							delete jpeg;  // �ͷ���Դ
						}
					}
					else
						MessageBox(0,"����������ʧ��","����",MB_ICONERROR);
					}
				__finally {
					delete Stream;  // �ͷ���Դ
				}
			}
			else
				MessageBox(0,("�޷�������'"+ RemoteAddress +"'��������").c_str(),
					"����",MB_ICONERROR);
		}
		else
			MessageBox(0,"����˿�ʧ�ܣ��޷�������������","����",MB_ICONERROR);
	}
	else
		MessageBox(0,("����'"+RemoteAddress+"'û����Ӧ").c_str(),"����",MB_ICONERROR);
    
	return bRtn;
}

void __fastcall TRecvStreamThread::Execute()
{
        //---- Place thread code here ----
	if (LoadImage(RemoteScreen))
		MainForm->StatusBar->SimpleText = "���ݽ������";
	else
		MainForm->StatusBar->SimpleText = "��������ʧ��";
}
//---------------------------------------------------------------------------
