//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "prototype.h"
#include "Unit2.h"
#include "Unit3.h"
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
void __fastcall TForm6::backImageButtonClick(TObject *Sender)
{
	Form6->Hide();
	Form5->Show();

	//set things back to empty
	roleNameEdit->Text = "";
	roleWagesEdit->Text = "";
	referencePopupBox->Items->Clear();
	referencePopupBox->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm6::saveChangesButtonClick(TObject *Sender)
{
	//get info from edit boxes and popup box
	String roleName = roleNameEdit->Text;
	String roleWages = roleWagesEdit->Text;
	String roleReference = StringReplace(referencePopupBox->Items->operator [](referencePopupBox->ItemIndex), " ", "_", TReplaceFlags() << rfReplaceAll);

	//get different forms of role name
	String bareRoleName = StringReplace(roleName, " ", "_", TReplaceFlags() << rfReplaceAll);
	String roleHoursPaid = bareRoleName + "_Hours_Paid";
	String roleStandardHours = bareRoleName + "_Standard_Hours";
	String rolePercentPerformance = bareRoleName + "_Percent_Performance";

	//add to role table
	/*String roleValues = "'" + currentHotelID + "', '" + roleHoursPaid + "', '" + bareRoleName + "', '" + roleWages + "', '" + roleReference + "'";
	Form3->SQLQuery2->SQL->Text = "INSERT INTO baldwins_hotel_data.role_table (hotelID, Role_Name, Bare_Role_Name, Role_Wages, Standard_Hours_Reference) VALUES ("+roleValues+");";
	Form3->SQLQuery2->ExecSQL();*/

	//add to input table
	Form3->SQLQuery2->SQL->Text = "ALTER TABLE baldwins_hotel_data."+inputTable+" ADD COLUMN "+roleHoursPaid+" DOUBLE NOT NULL AFTER "+StringReplace(referencePopupBox->Items->operator [](referencePopupBox->Items->Count-1) + "_Hours_Paid", " ", "_", TReplaceFlags() << rfReplaceAll)+";";
    Form3->SQLQuery2->ExecSQL();

	//take back to settings page
	backImageButton->OnClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm6::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form5->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm6::homeImageButton6Click(TObject *Sender)
{
	Form6->Hide();
	Form2->Show();

	//set things back to empty
	roleNameEdit->Text = "";
	roleWagesEdit->Text = "";
	referencePopupBox->Items->Clear();
	referencePopupBox->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm6::FormShow(TObject *Sender)
{
	//fill the rolePopupBox with possible labor standards references
	//query hotel ref to see which labor standards to pull references from
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

	//query information schema of labor standards table to get possible reference names
	Form3->SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+laborTable+"';";
	Form3->SQLQuery2->Open();
	Form3->SQLQuery2->First();
	Form3->SQLQuery2->Next();
	Form3->SQLQuery2->Next();
	Form3->SQLQuery2->Next();
	Form3->SQLQuery2->Next();
	vector<String> referenceVector;
	while (!Form3->SQLQuery2->Eof)
	{
		referenceVector.push_back(Form3->SQLQuery2->Fields->Fields[0]->AsString);
		Form3->SQLQuery2->Next();
	}
	for (int i = 0; i < referenceVector.size() - 2; ++i)
		referencePopupBox->Items->Add(StringReplace(referenceVector[i], "_", " ", TReplaceFlags() << rfReplaceAll));
}
//---------------------------------------------------------------------------

