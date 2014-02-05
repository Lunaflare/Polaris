//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "prototype.h"
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//hides Form2 from view when this form is shown, sets up form for certain input level
void __fastcall TForm3::FormShow(TObject *Sender)
{
	//hide welcome form
	Form2->Hide();

	//show user input screen based on input level (i.e. default or advanced)
	if (Form1->getInputLevel() == 0)
	{
		//input level is "default"
		SQLQuery2->SQL->Text =	"SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_marriot_long_beach' AND TABLE_NAME = 'raw_input';";

		//"select * from user_info where username='"+usernameEdit->Text+"' and password=md5('"+passwordEdit->Text+"');";

		//open query and temporarily skip first two column headings
		SQLQuery2->Open();
		SQLQuery2->First();
		SQLQuery2->Next();
		SQLQuery2->Next();

		if(!SQLQuery2->Eof)
		{

			String temp = SQLQuery2->Fields->Fields[0]->AsString;


			dbFieldLabel->Text = StringReplace(temp, "_", " ",
				TReplaceFlags() << rfReplaceAll);

			dbFieldLabel->Visible = true;
        }
	}
	else
	{
     	//input level is "advanced"
	}
}
//---------------------------------------------------------------------------

//avoids a never ending process by closing Form2 when this form is closed
void __fastcall TForm3::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form2->Close();
}
//---------------------------------------------------------------------------

//take user back to main menu when clicked
void __fastcall TForm3::homeImageButton3Click(TObject *Sender)
{
	Form3->Hide();
	Form2->Show();
}
//---------------------------------------------------------------------------



