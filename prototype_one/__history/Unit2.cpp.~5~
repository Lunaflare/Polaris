//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit2.h"
#include "prototype.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//hides the main form once this form is shown
void __fastcall TForm2::FormShow(TObject *Sender)
{
	Form1->Hide();
}
//---------------------------------------------------------------------------

//avoids a never ending process by closing main form when this form is closed
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form1->Close();
}
//---------------------------------------------------------------------------
