//---------------------------------------------------------------------------
#ifndef RecvStreamH
#define RecvStreamH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <Classes.hpp>
#include "Main.h"
//---------------------------------------------------------------------------
class TRecvStreamThread : public TThread
{
private:
protected:
        void __fastcall Execute();
public:
        __fastcall TRecvStreamThread(bool CreateSuspended);
        bool __fastcall LoadImage(TImage *Image1);
        TImage *RemoteScreen;  // ��ʾͼ��Ķ���ָ��
        //TStatusBar *StatusBar;  // ��ʾ״̬��Ϣ�Ķ���ָ��
        AnsiString RemoteAddress;  // Զ������IP
        int CL, CQ;  // ɫ���ͼ��Ʒ��
};
//---------------------------------------------------------------------------
#endif
