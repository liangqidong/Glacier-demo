//---------------------------------------------------------------------------
#ifndef FullScrH
#define FullScrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFullScrForm : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TPopupMenu *PopupMenu1;
        TMenuItem *N3;
        TMenuItem *N2;
        TMenuItem *N1;
        void __fastcall FormDblClick(TObject *Sender);
        void __fastcall N3Click(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFullScrForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFullScrForm *FullScrForm;
//---------------------------------------------------------------------------
#endif
