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
	Execute = true;  // 表示用户选择了确定按钮
  
	// 如果是新的IP，则保存到下拉列表中
	TStringList *IPList = new TStringList;
	IPList->AddStrings(ComboBox1->Items);
	int Index;
	if (!IPList->Find(ComboBox1->Text,Index)) {
		IPList->Append(ComboBox1->Text);
		ComboBox1->Items->Clear();
		ComboBox1->Items->AddStrings(IPList);
	}
	delete IPList;
	Close();  // 关闭窗口
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::Button2Click(TObject *Sender)
{
	Execute = false;  // 表示用户选择了取消按钮
	Close();  // 关闭窗口
}
//---------------------------------------------------------------------------
