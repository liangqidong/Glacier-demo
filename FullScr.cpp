//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FullScr.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFullScrForm *FullScrForm;
//---------------------------------------------------------------------------
__fastcall TFullScrForm::TFullScrForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFullScrForm::FormDblClick(TObject *Sender)
{
	Close();  // ���˫��ȫ��������ر�
}
//---------------------------------------------------------------------------
void __fastcall TFullScrForm::N3Click(TObject *Sender)
{
	// ��ͼ���Ϊ�ļ�
	if (MainForm->SPDialog->Execute())
		Image1->Picture->Bitmap->SaveToFile(MainForm->SPDialog->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TFullScrForm::N1Click(TObject *Sender)
{
	// �Ƿ��Զ�����
	N1->Checked = !N1->Checked;
	Image1->Transparent = N1->Checked;        
}
//---------------------------------------------------------------------------
