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
		//input level is "default", query db schema
		SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_marriot_long_beach' AND TABLE_NAME = 'raw_input';";

		//open query and temporarily skip first two column headings
		SQLQuery2->Open();
		SQLQuery2->First();
		SQLQuery2->Next();
		SQLQuery2->Next();

		//strings used in if for holdling column headings
		String originalHeading = "";
		String editedHeading = "";

		//set label to represent first editable column heading
		if(!SQLQuery2->Eof)
		{
			//get initial column heading without formatting (i.e. removing "_")
			originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

			//replace underscores with spaces and set label
			editedHeading = StringReplace(originalHeading, "_", " ",
				TReplaceFlags() << rfReplaceAll);
			dbFieldLabel->Text = editedHeading;
			dbFieldLabel->Visible = true;

			//add to inputValues struct
			vector<String> temp;
			temp.push_back(originalHeading);
			temp.push_back(editedHeading);
			//inputValues inputObject;
			inputObject.currentIndex = 0;
			inputObject.size = 0;
			inputObject.valueMap[inputObject.currentIndex] = temp;
			//(inputObject.currentIndex)++;
			(inputObject.size)++;

			//dbFieldEdit->Text = inputObject.currentIndex;
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

//advance column headings as desired by the user
void __fastcall TForm3::nextImageButtonClick(TObject *Sender)
{
	//will run when it is time to get the next column from the db
	if (inputObject.currentIndex + 1 == inputObject.size)
	{
		//strings used in if for holdling column headings
		String originalHeading = "";
		String editedHeading = "";
		SQLQuery2->Next();

		if(!SQLQuery2->Eof)
		{
			//get initial column heading without formatting (i.e. removing "_")
			originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

			//replace underscores with spaces and set label
			editedHeading = StringReplace(originalHeading, "_", " ",
				TReplaceFlags() << rfReplaceAll);
			dbFieldLabel->Text = editedHeading;

			//add to inputValues struct (including edit text from last add)
			vector<String> temp;
			temp.push_back(originalHeading);
			temp.push_back(editedHeading);
			temp.push_back(dbFieldEdit->Text);
			inputObject.valueMap[inputObject.currentIndex + 1] = temp;
			dbFieldEdit->Text = "";
			(inputObject.currentIndex)++;
			(inputObject.size)++;

			//dbFieldEdit->Text = inputObject.currentIndex;
			backImageButton->Visible = true;
		}
		else
		{
			//spawn next part that displays desired changes and asks if sure
			//will also need to add the last item from the edit

			//hide all other items on form besides home button
			dbFieldLabel->Visible = false;
			dbFieldEdit->Visible = false;
			backImageButton->Visible = false;
			nextImageButton->Visible = false;

			//show and populate displayGrid
			/*for (int i = 0; i < inputObject.size; ++i)
			{
				displayGrid->Cells[0][i] = "n/a"; //inputObject.valueMap[i][1];
				displayGrid->Cells[1][i] = "n/a"; //inputObject.valueMap[i][2];
			}

			grdTimeSheet->Visible = true;*/
		}
	}
	else
	{
		//don't query db, only go to next item in struct, replace item before advancing
		inputObject.valueMap[inputObject.currentIndex].pop_back();
		inputObject.valueMap[inputObject.currentIndex].push_back(dbFieldLabel->Text);
		dbFieldEdit->Text = "";

		(inputObject.currentIndex)++;
		//dbFieldEdit->Text = inputObject.currentIndex;
		dbFieldLabel->Text = inputObject.valueMap[inputObject.currentIndex][1];
		backImageButton->Visible = true;
    }
}
//---------------------------------------------------------------------------

//go to previous item in struct, show in label field
void __fastcall TForm3::backImageButtonClick(TObject *Sender)
{
	(inputObject.currentIndex)--;
	if (inputObject.currentIndex == 0)
		backImageButton->Visible = false;

	//dbFieldEdit->Text = inputObject.currentIndex;
	dbFieldLabel->Text = inputObject.valueMap[inputObject.currentIndex][1];
}
//---------------------------------------------------------------------------

