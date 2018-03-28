//---------------------------------------------------------------------------
#include <vcl.h>
#include <jpeg.hpp>
#pragma hdrstop

#include "Main.h"
#include "Config.h"
#include "About.h"
#include "Socket.cpp"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

#define CM_ENTIRESCREEN 201   //捕获全屏
#define CM_ACTIVEWINDOW 202   //捕获当前窗口

TMainForm *MainForm;
//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

// 捕获当前屏幕并保存到imagestream中
void CaptureImage(int options, int level, int cq, TMemoryStream* imgstream)
{
	LONG width,height;
	RECT capRect;
	HDC DesktopDC;
	switch (options) {
		case CM_ENTIRESCREEN:  // 捕获整个屏幕
			// 取得桌面的矩形区域范围
			GetWindowRect(GetDesktopWindow(),&capRect);  
		break;
		case CM_ACTIVEWINDOW:  // 捕获当前窗口
			HWND ForegWin;
			ForegWin = GetForegroundWindow();  // 取得当前窗口句柄
			if (!ForegWin)
				ForegWin = GetDesktopWindow();
			GetWindowRect(ForegWin,&capRect);  // 取得当前窗口的矩形区域范围
		break;
	}

	DesktopDC = GetDC(GetDesktopWindow());  // 创建内存设备描述表
	width = capRect.right - capRect.left;
	height = capRect.bottom - capRect.top;
	Graphics::TBitmap *bBitmap;  // 定义位图变量
	try {
		bBitmap = new Graphics::TBitmap(); // 创建位图
		bBitmap->Width=width;
		bBitmap->Height=height;
		if ((level>0)&&(level<8))
			bBitmap->PixelFormat = TPixelFormat(level);  // 设定色深
		// 拷贝屏幕的指定区域到位图
		BitBlt(bBitmap->Canvas->Handle,0,0,width,height,DesktopDC,
			capRect.left,capRect.top,SRCCOPY);

		if (cq>=0) {
			TJPEGImage *jpeg;
			try {
				jpeg = new TJPEGImage;  // 创建JPEG图象
				jpeg->CompressionQuality = cq;  // 设定图象品质
				jpeg->Assign(bBitmap);  // 将位图转化为JPEG格式
				jpeg->SaveToStream(imgstream);  // 保存JPEG图象信息
			}
			 __finally {
				delete jpeg;  // 释放资源
			}
		}
		else {
			bBitmap->SaveToStream(imgstream);  // 保存位图信息
		}
	}
	__finally {
		delete bBitmap;  // 释放资源
	}
}

// 新建观察窗口
void __fastcall TMainForm::CreateMDIChild(AnsiString sAddress, int CL, int CQ)
{
	TMDIChild *Child;

	//--- create a new MDI child window ----
	Child = new TMDIChild(Application);
	Child->RemoteAddress = sAddress;  // 主机IP
	Child->CL = CL;  // 色深
	Child->CQ = CQ;  // 品质
	Child->Caption = "来自'" + sAddress+"'";
	Child->Image1DblClick(Owner);  // 捕获第一屏
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileNew1Execute(TObject *Sender)
{
	ConfigForm->ShowModal();
	if (ConfigForm->Execute) {
		// 新建观察窗口
		CreateMDIChild(ConfigForm->ComboBox1->Text,  // 主机IP
		ConfigForm->TrackBar1->Position,  // 色深
		ConfigForm->TrackBar2->Position*10);  // 品质
	}
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::HelpAbout1Execute(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileExit1Execute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

// 捕获并发送自己的屏幕图象
void __fastcall TMainForm::ServerSocket1ClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
	AnsiString sRecvString = Socket->ReceiveText();  // 保存接收到的字符串
	AnsiString sRemoteAddress = Socket->RemoteAddress;  // 保存对方IP
	int CL,CQ;
	u_short port;

	// 将接收到的字符串分解为接收端口、色深、品质3个参数
	int pos = sRecvString.Pos("\n");
	// 接收端口
	port = u_short(StrToIntDef(sRecvString.SubString(1,pos-1),0));
	sRecvString = sRecvString.SubString(pos+1,sRecvString.Length()-pos);

	pos = sRecvString.Pos("\n");
	// 色深
	CL = StrToIntDef(sRecvString.SubString(1,pos-1),0);
	sRecvString = sRecvString.SubString(pos+1,sRecvString.Length()-pos);

	pos = sRecvString.Pos("\n");
	// 品质
	CQ = StrToIntDef(sRecvString.SubString(1,pos-1),0);

	if (port) {
		TMemoryStream *ImageStream;  // 定义数据流
		try {
			ImageStream = new TMemoryStream;  // 分配内存
			// 捕获当前屏幕并保存到ImageStream中
			CaptureImage(CM_ENTIRESCREEN, CL, CQ, ImageStream);
			// 发送ImageStream到接收端口
			if (!SendStream(sRemoteAddress, port, ImageStream))
				MessageBox(0,"发送数据流失败","冰河",MB_ICONERROR);
		}
		__finally {
			delete ImageStream;  // 释放资源
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ServerSocket1ClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;
	MessageBox(0,"远程连接出错","冰河",MB_ICONERROR);  // 显示出错信息
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	ServerSocket1->Port = LISTENPORT;  // 指定监听端口
	ServerSocket1->Open();  // 开始监听
}
//---------------------------------------------------------------------------






void __fastcall TMainForm::N2Click(TObject *Sender)
{
	// 是否打开端口允许别人抓屏
	N2->Checked = !N2->Checked;
	ServerSocket1->Active = N2->Checked;
}
//---------------------------------------------------------------------------

