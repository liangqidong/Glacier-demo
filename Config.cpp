//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Config.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::Button1Click(TObject *Sender)
{
	Execute = true;  // ��ʾ�û�ѡ����ȷ����ť
  
	// ������µ�IP���򱣴浽�����б���
	TStringList *IPList = new TStringList;
	IPList->AddStrings(ComboBox1->Items);
	int Index;
	if (!IPList->Find(ComboBox1->Text,Index)) {
		IPList->Append(ComboBox1->Text);
		ComboBox1->Items->Clear();
		ComboBox1->Items->AddStrings(IPList);
	}
	delete IPList;
	Close();  // �رմ���
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::Button2Click(TObject *Sender)
{
	Execute = false;  // ��ʾ�û�ѡ����ȡ����ť
	Close();  // �رմ���
}
//---------------------------------------------------------------------------
