//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit5.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//hides Form2 from view when this form is shown
void __fastcall TForm5::FormShow(TObject *Sender)
{
	Form2->Hide();
}
//---------------------------------------------------------------------------

//avoids a never ending process by closing Form2 when this form is closed
void __fastcall TForm5::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form2->Close();
}
//---------------------------------------------------------------------------
