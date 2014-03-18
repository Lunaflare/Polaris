//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "prototype.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit5.h"
#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm7::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form5->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm7::FormShow(TObject *Sender)
{
	//initially set state to zero
	state = 0;

	//query hotel_ref to get id, input, read, and labor
    currentHotelID = Form1->getHotelID();
	Form3->SQLQuery2->SQL->Text = "SELECT input_table, read_table, labor_table FROM hotel_ref WHERE hotelID = '"+currentHotelID+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	if (!Form3->SQLQuery2->Eof)
	{
		inputTable = Form3->SQLQuery2->Fields->Fields[0]->AsString;
		readTable = Form3->SQLQuery2->Fields->Fields[1]->AsString;
		laborTable = Form3->SQLQuery2->Fields->Fields[2]->AsString;
	}

	//populate rolePopupBox from role_table
	Form3->SQLQuery2->SQL->Text = "SELECT Bare_Role_Name FROM role_table WHERE hotelID = '"+currentHotelID+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	while (!Form3->SQLQuery2->Eof)
	{
		rolePopupBox->Items->Add(StringReplace(Form3->SQLQuery2->Fields->Fields[0]->AsString, "_", " ", TReplaceFlags() << rfReplaceAll));
        Form3->SQLQuery2->Next();
	}
	rolePopupBox->ItemIndex = 0;
	rolePopupBox->Text = rolePopupBox->Items->operator [](rolePopupBox->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TForm7::homeImageButton7Click(TObject *Sender)
{
	Form7->Hide();
	Form2->Show();

	//set things back to empty
	chooseRoleLabel->Text = "Choose a role to edit: ";
	rolePopupBox->Items->Clear();
	rolePopupBox->ItemIndex = 0;
	rolePopupBox->Text = "";
	roleNameLabel->Visible = false;
	roleNameEdit->Visible = false;
	roleWagesLabel->Visible = false;
	roleWagesEdit->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::backImageButtonClick(TObject *Sender)
{
	//should run this when in phase 0
	if (state == 0)
	{
		Form7->Hide();
		Form5->Show();

		//set things back to empty
		chooseRoleLabel->Text = "Choose a role to edit: ";
		rolePopupBox->Items->Clear();
		rolePopupBox->ItemIndex = 0;
		rolePopupBox->Text = "";
	}
	else
	{
		//state is 1, show state zero
		state--;
		chooseRoleLabel->Text = "Choose a role to edit: ";
		nextImageButton->Visible = true;
		saveChangesButton->Visible = false;
		rolePopupBox->Visible = true;
		roleNameLabel->Visible = false;
		roleNameEdit->Visible = false;
		roleWagesLabel->Visible = false;
		roleWagesEdit->Visible = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::nextImageButtonClick(TObject *Sender)
{
	//hide/make visible certain buttons
	nextImageButton->Visible = false;
	saveChangesButton->Visible = true;
	rolePopupBox->Visible = false;
	roleNameLabel->Visible = true;
	roleNameEdit->Visible = true;
	roleWagesLabel->Visible = true;
	roleWagesEdit->Visible = true;

	//change state value
	state++;

	//get chosen item from rolePopupBox
	roleChosen = rolePopupBox->Items->operator [](rolePopupBox->ItemIndex);

	//get wages for selected role
	Form3->SQLQuery2->SQL->Text = "SELECT Role_Wages FROM role_table WHERE hotelID = '"+currentHotelID+"' AND Bare_Role_Name = '"+StringReplace(roleChosen, " ", "_", TReplaceFlags() << rfReplaceAll)+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	if (!Form3->SQLQuery2->Eof)
		roleWages = Form3->SQLQuery2->Fields->Fields[0]->AsFloat;
	else
		roleWages = 0.0;

	//change items to represent current role information
	chooseRoleLabel->Text = "Currently editing " + roleChosen + ":";
	roleNameEdit->Text = roleChosen;
	roleWagesEdit->Text = roleWages;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::saveChangesButtonClick(TObject *Sender)
{
	//get values from edit boxes
	String roleNameNew = roleNameEdit->Text;
	String roleWagesNew = roleWagesEdit->Text;

	//update query for role table


	//update query for labor standards table


	//update query for read table


	//update query for input table

}

//---------------------------------------------------------------------------
