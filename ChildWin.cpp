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
	Action = caFree;  // 关闭后自动释放资源
}
//---------------------------------------------------------------------

void __fastcall TMDIChild::Image1DblClick(TObject *Sender)
{
	// 双击表示继续捕获下一幅图象，创建一个新的线程发送命令并接收图象数据流
	TRecvStreamThread *RecvThread = new TRecvStreamThread(true);
	RecvThread->FreeOnTerminate = true;  // 线程执行完毕自动释放资源
	RecvThread->RemoteAddress = RemoteAddress;  // 指定主机IP
	RecvThread->CL = CL;  // 色深
	RecvThread->CQ = CQ;  // 品质
	RecvThread->RemoteScreen = Image1;
	//RecvThread->StatusBar = StatusBar1;
	RecvThread->Resume();  // 开始执行线程
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::FormShow(TObject *Sender)
{
	// 设置初始的窗口大小
	ClientWidth = 320;
	ClientHeight = 240;
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::N3Click(TObject *Sender)
{
	// 将图象保存为文件
	if (MainForm->SPDialog->Execute())
		Image1->Picture->Bitmap->SaveToFile(MainForm->SPDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::N2Click(TObject *Sender)
{
	// 全屏显示
	FullScrForm->Image1->Picture = Image1->Picture;
	FullScrForm->Show();
}
//---------------------------------------------------------------------------

