//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("Main.cpp", MainForm);
USEFORM("ChildWin.cpp", MDIChild);
USEFORM("about.cpp", AboutBox);
USEFORM("FullScr.cpp", FullScrForm);
USEFORM("Config.cpp", ConfigForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application->Initialize();
	Application->Title = "Glacier Screen Spy";
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->CreateForm(__classid(TFullScrForm), &FullScrForm);
                 Application->CreateForm(__classid(TConfigForm), &ConfigForm);
                 Application->Run();

	return 0;
}
//---------------------------------------------------------------------
