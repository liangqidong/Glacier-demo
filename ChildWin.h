//----------------------------------------------------------------------------
#ifndef ChildWinH
#define ChildWinH
//----------------------------------------------------------------------------
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include "RecvStream.h"
#include <ComCtrls.hpp>
#include <Menus.hpp>
//----------------------------------------------------------------------------
class TMDIChild : public TForm
{
__published:
        TImage *Image1;
        TStatusBar *StatusBar1;
        TPopupMenu *PopupMenu1;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N4;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Image1DblClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall N3Click(TObject *Sender);
        void __fastcall N2Click(TObject *Sender);
private:
public:
        //TStatusBar *StatusBar;  // ��ʾ״̬��Ϣ�Ķ���ָ��
	virtual __fastcall TMDIChild(TComponent *Owner);
        bool __fastcall LoadImage(void);  // ����ͼ��
        AnsiString RemoteAddress;  // Զ������IP
        int CL, CQ;  // ɫ���ͼ��Ʒ��

};
//----------------------------------------------------------------------------
#endif
