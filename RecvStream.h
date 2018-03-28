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
        TImage *RemoteScreen;  // 显示图象的对象指针
        //TStatusBar *StatusBar;  // 显示状态信息的对象指针
        AnsiString RemoteAddress;  // 远程主机IP
        int CL, CQ;  // 色深和图象品质
};
//---------------------------------------------------------------------------
#endif
