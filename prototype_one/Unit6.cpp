//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit2.h"
#include "Unit5.h"
#include "Unit6.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm6 *Form6;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm6::homeImageButton6DblClick(TObject *Sender)
{
	Form6->Hide();
	Form2->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm6::backImageButtonClick(TObject *Sender)
{
	Form6->Hide();
	Form5->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm6::saveChangesButtonClick(TObject *Sender)
{
	//todo
}
//---------------------------------------------------------------------------