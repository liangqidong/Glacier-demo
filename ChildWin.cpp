//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChildWin.h"
#include "Main.h"
#include "FullScr.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------
__fastcall TMDIChild::TMDIChild(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------

void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;  // �رպ��Զ��ͷ���Դ
}
//---------------------------------------------------------------------

void __fastcall TMDIChild::Image1DblClick(TObject *Sender)
{
	// ˫����ʾ����������һ��ͼ�󣬴���һ���µ��̷߳����������ͼ��������
	TRecvStreamThread *RecvThread = new TRecvStreamThread(true);
	RecvThread->FreeOnTerminate = true;  // �߳�ִ������Զ��ͷ���Դ
	RecvThread->RemoteAddress = RemoteAddress;  // ָ������IP
	RecvThread->CL = CL;  // ɫ��
	RecvThread->CQ = CQ;  // Ʒ��
	RecvThread->RemoteScreen = Image1;
	//RecvThread->StatusBar = StatusBar1;
	RecvThread->Resume();  // ��ʼִ���߳�
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::FormShow(TObject *Sender)
{
	// ���ó�ʼ�Ĵ��ڴ�С
	ClientWidth = 320;
	ClientHeight = 240;
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::N3Click(TObject *Sender)
{
	// ��ͼ�󱣴�Ϊ�ļ�
	if (MainForm->SPDialog->Execute())
		Image1->Picture->Bitmap->SaveToFile(MainForm->SPDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::N2Click(TObject *Sender)
{
	// ȫ����ʾ
	FullScrForm->Image1->Picture = Image1->Picture;
	FullScrForm->Show();
}
//---------------------------------------------------------------------------

