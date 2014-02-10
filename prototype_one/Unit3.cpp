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

	//get the hotelID and query hotel_ref to find out which table to input to
	String currentHotelID = Form1->getHotelID();
	inputTable = "";
	SQLQuery2->SQL->Text = "SELECT input_table FROM hotel_ref WHERE hotelID = '"+currentHotelID+"';";
	SQLQuery2->Open();
	SQLQuery2->First();
	if (!SQLQuery2->Eof)
	{
		inputTable = SQLQuery2->Fields->Fields[0]->AsString;
	}

	//show user input screen based on input level (i.e. default or advanced)
	if (Form1->getInputLevel() == 0)
	{
		//input level is "default", query db schema
		SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+inputTable+"';";

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
			dbFieldEdit->Visible = true;
			nextImageButton->Visible = true;

			//initially set index and size to zero
			inputObject.currentIndex = 0;
			inputObject.size = 0;
		}
	}
	else
	{
		//hide form items
		dbFieldLabel->Visible = false;
		dbFieldEdit->Visible = false;
		nextImageButton->Visible = false;

		//input level is "advanced"
		SQLQuery2->SQL->Text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'baldwins_hotel_data' AND TABLE_NAME = '"+inputTable+"';";

		//open query and temporarily skip first two column headings
		SQLQuery2->Open();
		SQLQuery2->First();
		SQLQuery2->Next();
		SQLQuery2->Next();

        displayGrid->Cells[0][0] = "Heading";
		displayGrid->Cells[1][0] = "Value";
		int count = 1;
		//strings used in if for holdling column headings
		String originalHeading = "";
		String editedHeading = "";

		while (!SQLQuery2->Eof)
		{
			//get initial column heading without formatting (i.e. removing "_")
			originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

			//replace underscores with spaces and set label
			editedHeading = StringReplace(originalHeading, "_", " ",
				TReplaceFlags() << rfReplaceAll);

			//show and populate displayGrid
			displayGrid->Cells[0][count] = editedHeading;
			displayGrid->Cells[1][count] = "";

			++count;
			SQLQuery2->Next();
		}

		displayGrid->RowCount = count;
		displayGrid->Visible = true;
		submitButton->Visible = true;
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
	//hide certain items
	backImageButton->Visible = false;
	displayGrid->Visible = false;
	submitButton->Visible = false;

	Form3->Hide();
	Form2->Show();
}
//---------------------------------------------------------------------------

//go through column headings as desired by the user
void __fastcall TForm3::nextImageButtonClick(TObject *Sender)
{
	if (inputObject.currentIndex == 0 && inputObject.size == 0)
	{
		//run when displayObject is empty

        //strings used in if for holdling column headings
		String originalHeading = "";
		String editedHeading = "";

		//get initial column heading without formatting (i.e. removing "_")
		originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

		//replace underscores with spaces and set label
		editedHeading = StringReplace(originalHeading, "_", " ",
			TReplaceFlags() << rfReplaceAll);

		//add data to inputObject and increase index/size
		vector<String> temp;
		temp.push_back(originalHeading);
		temp.push_back(editedHeading);
		temp.push_back(dbFieldEdit->Text);
		inputObject.valueMap[inputObject.currentIndex] = temp;
		dbFieldEdit->Text = "";
		(inputObject.currentIndex)++;
		(inputObject.size)++;
		originalHeading = "";
		editedHeading = "";

		//advance query
		SQLQuery2->Next();

		//get initial column heading without formatting (i.e. removing "_")
		originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

		//replace underscores with spaces and set label
		editedHeading = StringReplace(originalHeading, "_", " ",
			TReplaceFlags() << rfReplaceAll);
		dbFieldLabel->Text = editedHeading;

		backImageButton->Visible = true;
	}
	else if (inputObject.currentIndex == inputObject.size)
	{
		//will run when it is time to get the next column from the db

		//strings used in if for holdling column headings
		String originalHeading = "";
		String editedHeading = "";

		if(!SQLQuery2->Eof)
		{
			//get initial column heading without formatting (i.e. removing "_")
			originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

			//replace underscores with spaces and set label
			editedHeading = StringReplace(originalHeading, "_", " ",
				TReplaceFlags() << rfReplaceAll);

			//fill inputObject with data and increse index/size
			vector<String> temp;
			temp.push_back(originalHeading);
			temp.push_back(editedHeading);
			temp.push_back(dbFieldEdit->Text);
			inputObject.valueMap[inputObject.currentIndex] = temp;
			dbFieldEdit->Text = "";
			(inputObject.currentIndex)++;
			(inputObject.size)++;
			originalHeading = "";
			editedHeading = "";

			//advance query cursor
			SQLQuery2->Next();

			//check if now at the end of the query
			if(SQLQuery2->Eof)
			{
				//spawn next part that displays desired changes and asks if sure

				//hide all other items on form besides home button
				dbFieldLabel->Visible = false;
				dbFieldEdit->Visible = false;
				nextImageButton->Visible = false;

				//show and populate displayGrid
				displayGrid->RowCount = inputObject.valueMap.size() + 1;
				displayGrid->Cells[0][0] = "Heading";
				displayGrid->Cells[1][0] = "Value";
				for (int i = 0; i < inputObject.size; ++i)
				{
					displayGrid->Cells[0][i+1] = inputObject.valueMap[i][1];
					displayGrid->Cells[1][i+1] = inputObject.valueMap[i][2];
				}

				displayGrid->Visible = true;
				submitButton->Visible = true;
			}
			else
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);
				dbFieldLabel->Text = editedHeading;
				backImageButton->Visible = true;
			}
		}
	}
	else
	{
		//don't query db, only go to next item in struct, replace item before advancing
		inputObject.valueMap[inputObject.currentIndex].pop_back();
		inputObject.valueMap[inputObject.currentIndex].push_back(dbFieldEdit->Text);
		dbFieldEdit->Text = "";
		(inputObject.currentIndex)++;

		if (inputObject.currentIndex == inputObject.size)
		{
			//accounts for case when coming back after pressing previous if data not previously captured

			//strings used in if for holdling column headings
			String originalHeading = "";
			String editedHeading = "";

			if(!SQLQuery2->Eof)
			{
				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = "";
				editedHeading = "";

				//get initial column heading without formatting (i.e. removing "_")
				originalHeading = SQLQuery2->Fields->Fields[0]->AsString;

				//replace underscores with spaces and set label
				editedHeading = StringReplace(originalHeading, "_", " ",
					TReplaceFlags() << rfReplaceAll);
				dbFieldLabel->Text = editedHeading;

				backImageButton->Visible = true;
			}
			else
			{
              	//spawn next part that displays desired changes and asks if sure

				//hide all other items on form besides home button
				dbFieldLabel->Visible = false;
				dbFieldEdit->Visible = false;
				nextImageButton->Visible = false;

				//show and populate displayGrid
				for (int i = 0; i < inputObject.size; ++i)
				{
					displayGrid->Cells[0][i+1] = inputObject.valueMap[i][1];
					displayGrid->Cells[1][i+1] = inputObject.valueMap[i][2];
				}

				submitButton->Visible = true;
				displayGrid->Visible = true;
			}
		}
		else
		{
			//when next pressed and data for this index has previously been captured
			dbFieldEdit->Text = inputObject.valueMap[inputObject.currentIndex][2];
			dbFieldLabel->Text = inputObject.valueMap[inputObject.currentIndex][1];
			backImageButton->Visible = true;
        }

    }
}
//---------------------------------------------------------------------------

//go to previous item in struct, show in label field
void __fastcall TForm3::backImageButtonClick(TObject *Sender)
{
	//for when grid displayed and back button pressed
	if (displayGrid->Visible)
	{
		displayGrid->Visible = false;
		submitButton->Visible = false;
		dbFieldLabel->Visible = true;
		dbFieldEdit->Visible = true;
		nextImageButton->Visible = true;
	}

	(inputObject.currentIndex)--;
	if (inputObject.currentIndex == 0)
		backImageButton->Visible = false;

	dbFieldEdit->Text = inputObject.valueMap[inputObject.currentIndex][2];
	dbFieldLabel->Text = inputObject.valueMap[inputObject.currentIndex][1];
}
//---------------------------------------------------------------------------

//ask user if sure, yes == submit and no == do nothing
void __fastcall TForm3::submitButtonClick(TObject *Sender)
{
	int msgboxID = MessageBox(
		NULL,
		L"Would you really like to submit?",
		L"Confirm Input Submission",
		MB_ICONEXCLAMATION | MB_YESNO
	);

	if (msgboxID == IDYES)
	{
		//yes selected, save information in grid to database

		//create empty strings to add to query once populated from grid
		String headings = "";
		String values = "";

		//get today's date
		TDateTime d = Now();
		String currentDate = d.FormatString(L"yyyy-mm-dd");

		//get day of week
		TDateTime Value = Date();
		int Day = DayOfWeek(Value);
		String DayName;
		switch(Day)
		{
		case 1:
			DayName = L"Sunday";
			break;
		case 2:
			DayName = L"Monday";
			break;
		case 3:
			DayName = L"Tuesday";
			break;
		case 4:
			DayName = L"Wednesday";
			break;
		case 5:
			DayName = L"Thursday";
			break;
		case 6:
			DayName = L"Friday";
			break;
		case 7:
			DayName = L"Saturday";
		}

		//populate date and day of week (probably temporary solution)
		headings += "Date, Day_Of_Week, ";
		values += "'" + currentDate + "', '" + DayName + "', ";

		//populate rest of strings from grid
		for (int i = 0; i < inputObject.size; ++i)
		{
			if (i + 1 == inputObject.size)
			{
				headings += ("" + inputObject.valueMap[i][0] + "");
				values += ("'" + displayGrid->Cells[1][i+1] + "'");
			}
			else
			{
				headings += ("" + inputObject.valueMap[i][0] + ", ");
				values += ("'" + displayGrid->Cells[1][i+1] + "', ");
			}
		}

		//query db and insert new row
		SQLQuery2->SQL->Text = "INSERT INTO baldwins_hotel_data."+inputTable+" ("+headings+") VALUES ("+values+");";
		SQLQuery2->ExecSQL();

		//Take back to welcome screen (form2)
		Form3->Hide();
		Form2->Show();
	}
	else
	{
		//no selected, save nothing and show form again (do nothing)
	}
}
//---------------------------------------------------------------------------

//check if enter button is pressed from inside dbFieldEdit
void __fastcall TForm3::dbFieldEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
	//if enter button pressed, click next button
	if (Key == 13)
	{
     	nextImageButton->OnClick(NULL);
	}
}
//---------------------------------------------------------------------------

