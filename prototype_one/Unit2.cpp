//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit2.h"
#include "prototype.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
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

	//display username hello message, get username from form1
	Form2->welcomeUserLabel->Text = "Hello " + Form1->getUsername() + "!";
	Form2->welcomeUserLabel->Visible = true;
}
//---------------------------------------------------------------------------

//avoids a never ending process by closing main form when this form is closed
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form1->Close();
}
//---------------------------------------------------------------------------

//shows Form3 (input form) when input button is clicked
void __fastcall TForm2::inputSelectButtonClick(TObject *Sender)
{
	Form3->Show();
}
//---------------------------------------------------------------------------

//shows Form4 (read form) when read button is clicked
void __fastcall TForm2::readSelectButtonClick(TObject *Sender)
{
	Form4->Show();
}
//---------------------------------------------------------------------------

//shows Form5 (settings form) when settings button is clicked
void __fastcall TForm2::settingsSelectButtonClick(TObject *Sender)
{
	Form5->Show();
}
//---------------------------------------------------------------------------

