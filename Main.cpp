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

#define CM_ENTIRESCREEN 201   //����ȫ��
#define CM_ACTIVEWINDOW 202   //����ǰ����

TMainForm *MainForm;
//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

// ����ǰ��Ļ�����浽imagestream��
void CaptureImage(int options, int level, int cq, TMemoryStream* imgstream)
{
	LONG width,height;
	RECT capRect;
	HDC DesktopDC;
	switch (options) {
		case CM_ENTIRESCREEN:  // ����������Ļ
			// ȡ������ľ�������Χ
			GetWindowRect(GetDesktopWindow(),&capRect);  
		break;
		case CM_ACTIVEWINDOW:  // ����ǰ����
			HWND ForegWin;
			ForegWin = GetForegroundWindow();  // ȡ�õ�ǰ���ھ��
			if (!ForegWin)
				ForegWin = GetDesktopWindow();
			GetWindowRect(ForegWin,&capRect);  // ȡ�õ�ǰ���ڵľ�������Χ
		break;
	}

	DesktopDC = GetDC(GetDesktopWindow());  // �����ڴ��豸������
	width = capRect.right - capRect.left;
	height = capRect.bottom - capRect.top;
	Graphics::TBitmap *bBitmap;  // ����λͼ����
	try {
		bBitmap = new Graphics::TBitmap(); // ����λͼ
		bBitmap->Width=width;
		bBitmap->Height=height;
		if ((level>0)&&(level<8))
			bBitmap->PixelFormat = TPixelFormat(level);  // �趨ɫ��
		// ������Ļ��ָ������λͼ
		BitBlt(bBitmap->Canvas->Handle,0,0,width,height,DesktopDC,
			capRect.left,capRect.top,SRCCOPY);

		if (cq>=0) {
			TJPEGImage *jpeg;
			try {
				jpeg = new TJPEGImage;  // ����JPEGͼ��
				jpeg->CompressionQuality = cq;  // �趨ͼ��Ʒ��
				jpeg->Assign(bBitmap);  // ��λͼת��ΪJPEG��ʽ
				jpeg->SaveToStream(imgstream);  // ����JPEGͼ����Ϣ
			}
			 __finally {
				delete jpeg;  // �ͷ���Դ
			}
		}
		else {
			bBitmap->SaveToStream(imgstream);  // ����λͼ��Ϣ
		}
	}
	__finally {
		delete bBitmap;  // �ͷ���Դ
	}
}

// �½��۲촰��
void __fastcall TMainForm::CreateMDIChild(AnsiString sAddress, int CL, int CQ)
{
	TMDIChild *Child;

	//--- create a new MDI child window ----
	Child = new TMDIChild(Application);
	Child->RemoteAddress = sAddress;  // ����IP
	Child->CL = CL;  // ɫ��
	Child->CQ = CQ;  // Ʒ��
	Child->Caption = "����'" + sAddress+"'";
	Child->Image1DblClick(Owner);  // �����һ��
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileNew1Execute(TObject *Sender)
{
	ConfigForm->ShowModal();
	if (ConfigForm->Execute) {
		// �½��۲촰��
		CreateMDIChild(ConfigForm->ComboBox1->Text,  // ����IP
		ConfigForm->TrackBar1->Position,  // ɫ��
		ConfigForm->TrackBar2->Position*10);  // Ʒ��
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

// ���񲢷����Լ�����Ļͼ��
void __fastcall TMainForm::ServerSocket1ClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
	AnsiString sRecvString = Socket->ReceiveText();  // ������յ����ַ���
	AnsiString sRemoteAddress = Socket->RemoteAddress;  // ����Է�IP
	int CL,CQ;
	u_short port;

	// �����յ����ַ����ֽ�Ϊ���ն˿ڡ�ɫ�Ʒ��3������
	int pos = sRecvString.Pos("\n");
	// ���ն˿�
	port = u_short(StrToIntDef(sRecvString.SubString(1,pos-1),0));
	sRecvString = sRecvString.SubString(pos+1,sRecvString.Length()-pos);

	pos = sRecvString.Pos("\n");
	// ɫ��
	CL = StrToIntDef(sRecvString.SubString(1,pos-1),0);
	sRecvString = sRecvString.SubString(pos+1,sRecvString.Length()-pos);

	pos = sRecvString.Pos("\n");
	// Ʒ��
	CQ = StrToIntDef(sRecvString.SubString(1,pos-1),0);

	if (port) {
		TMemoryStream *ImageStream;  // ����������
		try {
			ImageStream = new TMemoryStream;  // �����ڴ�
			// ����ǰ��Ļ�����浽ImageStream��
			CaptureImage(CM_ENTIRESCREEN, CL, CQ, ImageStream);
			// ����ImageStream�����ն˿�
			if (!SendStream(sRemoteAddress, port, ImageStream))
				MessageBox(0,"����������ʧ��","����",MB_ICONERROR);
		}
		__finally {
			delete ImageStream;  // �ͷ���Դ
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ServerSocket1ClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;
	MessageBox(0,"Զ�����ӳ���","����",MB_ICONERROR);  // ��ʾ������Ϣ
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	ServerSocket1->Port = LISTENPORT;  // ָ�������˿�
	ServerSocket1->Open();  // ��ʼ����
}
//---------------------------------------------------------------------------






void __fastcall TMainForm::N2Click(TObject *Sender)
{
	// �Ƿ�򿪶˿��������ץ��
	N2->Checked = !N2->Checked;
	ServerSocket1->Active = N2->Checked;
}
//---------------------------------------------------------------------------

